/* EmulTwo  - A Windows Colecovision emulator.
 * Copyright (C) 2018-2019 Alekmaul
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 * nametabviewer_.cpp
 *
 */
#include <vcl.h>
#pragma hdrstop

#include "nametabviewer_.h"

#include "accdraw_.h"
#include "main_.h"
#include "coleco.h"
#include "colecoconfig.h"
#include "tms9928a.h"
#include "utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PaintBoxGrid"
#pragma resource "*.dfm"
Tnametabviewer *nametabviewer;
//---------------------------------------------------------------------------
__fastcall Tnametabviewer::Tnametabviewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    mVramTile = 0x0000;

    ini = new TIniFile(coleco.inipath);
    LoadSettings(ini);
    delete ini;

    mOffscreenBitmap=NULL;
    mOfftileBitmap=NULL;
}

//---------------------------------------------------------------------------

void Tnametabviewer::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("NAMVIEW","Top",Top);
    Left = ini->ReadInteger("NAMVIEW","Left",Left);

    AutoRefresh1->Checked = ini->ReadBool("NAMVIEW", "AutoRefresh", true);
}

void Tnametabviewer::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("NAMVIEW","Top",Top);
    ini->WriteInteger("NAMVIEW","Left",Left);
    ini->WriteBool("NAMVIEW", "AutoRefresh", AutoRefresh1->Checked);
}

//---------------------------------------------------------------------------
void __fastcall Tnametabviewer::SaveasBMP1Click(TObject *Sender)
{
    Graphics::TBitmap *bmp;

    // Get graphcis and copy to actual bitmap
    bmp = new Graphics::TBitmap;
    bmp->Width = 256*2;
    bmp->Height = 192*2;
    bmp->PixelFormat = pf24bit;

    // Check save dialog to save effectively to disk
    if(dlgSVPic->Execute())
    {
        CreateMap(bmp->Canvas,bmp->Width,bmp->Height);
        if (dlgSVPic->FilterIndex==1)
                bmp->SaveToFile(dlgSVPic->FileName);
        else
                ImageToPNG( dlgSVPic->FileName, bmp);
    }
    delete bmp;
}

//---------------------------------------------------------------------------
void Tnametabviewer::CreateTile(void)
{
    int iy,value,vcol, fgcol,bgcol;
    unsigned int *LinePtr;

    // Prepare a 8x8 bitmap for the tile
    if (mOfftileBitmap)
    {
        delete mOfftileBitmap;
    }

    mOfftileBitmap = new Graphics::TBitmap;
    mOfftileBitmap->Width = 8;
    mOfftileBitmap->Height = 8;
    mOfftileBitmap->PixelFormat = pf32bit;
    mOfftileBitmap->IgnorePalette = true;
    idTiValue->Caption="";

    for (iy=0;iy<8;iy++)
    {
        LinePtr = (unsigned int *) mOfftileBitmap->ScanLine[iy];

            if ( (mVramTile >=coleco_gettmsaddr(CHRGEN,0,0)) && (mVramTile<coleco_gettmsaddr(CHRGEN,0,0)+0x1800) ) {
                if (tms.Mode==1) {
                    vcol =coleco_gettmsval(CHRCOL,mVramTile,tms.Mode,iy>>3);
                }
                else
                {
                    if (mVramTile >=0x1000)
                        vcol =coleco_gettmsval(CHRCOL,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x80);
                    else if (mVramTile >=0x800)
                        vcol =coleco_gettmsval(CHRCOL,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x40);
                    else
                        vcol =coleco_gettmsval(CHRCOL,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x00);
                }
                // Get fg and bg color
                fgcol=cv_pal32[vcol>>4];
                bgcol=cv_pal32[vcol & 0xf];
                if (mVramTile >=0x1000)
                    value =coleco_gettmsval(CHRGEN,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x80);
                else if (mVramTile >=0x800)
                    value =coleco_gettmsval(CHRGEN,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x40);
                else
                    value =coleco_gettmsval(CHRGEN,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x00);
            }
            // not bg, vram or sprite
            else {
                fgcol=cv_pal32[15];
                bgcol=cv_pal32[0];
                value = coleco_gettmsval(VRAM,mVramTile+(iy&7),0x00,0x00);
            }


        // Draw bitmap
        *(LinePtr++)= (value & 0x80) ? fgcol : bgcol;
        *(LinePtr++)= (value & 0x40) ? fgcol : bgcol;
        *(LinePtr++)= (value & 0x20) ? fgcol : bgcol;
        *(LinePtr++)= (value & 0x10) ? fgcol : bgcol;
        *(LinePtr++)= (value & 0x08) ? fgcol : bgcol;
        *(LinePtr++)= (value & 0x04) ? fgcol : bgcol;
        *(LinePtr++)= (value & 0x02) ? fgcol : bgcol;
        *(LinePtr)= (value & 0x01) ?   fgcol : bgcol;

        // Add values
        idTiValue->Caption=idTiValue->Caption+"$"+IntToHex(value,2);
        if (iy!=7)
            idTiValue->Caption=idTiValue->Caption+",";
    }
    StretchBlt(TileAlone->Canvas->Handle, 0,0, 136,136,
        mOfftileBitmap->Canvas->Handle, 0, 0, 8, 8,SRCCOPY);
}

//---------------------------------------------------------------------------
void Tnametabviewer::CreateMap(TCanvas *Acanvas, int w, int h)
{
    int ix,iy,it;
    byte memtile[32*24];

    // Scan all lines
    for (iy=0;iy<192;iy++)
    {
        for (ix=0;ix<32;ix++)
        {
            // Get Pattern
            it=coleco_gettmsval(CHRMAP,ix+((iy & 0xF8)<<2),0,0);
            memtile[ix+(iy>>3)*32]=it;
        }
    }
    if (Form1->RenderMode==RENDERDDRAW)
    {
        StretchBlt(Acanvas->Handle, 0, 0, w, h,
            Form1->Canvas->Handle,8,8, Form1->ClientWidth-16,Form1->ClientHeight-16-Form1->StatusBar1->Height,SRCCOPY);
    }
    else
    {
        StretchBlt(Acanvas->Handle, 0, 0, w, h, GDIFrame->Canvas->Handle,BDW/2,BDH/2, TVW-BDW,TVH-BDH,SRCCOPY); // BDW,BDH because of border
    }

    // Draw a grid of 16x16 pix if needed
    if (chkGrid->Checked)
    {
        Acanvas->Brush->Style = bsClear;
        Acanvas->Pen->Style = psSolid;
        Acanvas->Pen->Color = cl3DDkShadow;
        for (ix=0;ix<Width;ix+=16)
        {
            Acanvas->MoveTo(ix,0);
            Acanvas->LineTo(ix,h);
        }
        for (ix=0;ix<Height;ix+=16)
        {
            Acanvas->MoveTo(0,ix);
            Acanvas->LineTo(w,ix);
        }
    }

    // if tile number, draw them
    if (chkTiles->Checked)
    {
        LOGFONT lf;
        ZeroMemory(&lf, sizeof(LOGFONT));
        lf.lfHeight = 12;
        //lf.lfWidth = 5;
        //lf.lfEscapement = 1;
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfQuality = NONANTIALIASED_QUALITY;
        strcpy(lf.lfFaceName, "Courier New");
        Acanvas->Font->Handle = CreateFontIndirect(&lf);
        Acanvas->Font->Color = (TColor) RGB(64,255,0); //clRed;
        //Abitmap->Canvas->Font->Size = 7;
        Acanvas->Brush->Style = bsClear;
        int stepX=w/32;
        int stepY=h/24;
        for (iy=0;iy<24;iy++)
        {
            for (ix=0;ix<32;ix++)
            {
                Acanvas->TextOut(stepX*ix, stepY*iy, IntToHex(memtile[ix+iy*32],2));
            }
        }
    }
}

//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Form1->TilemapViewer1->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::FormShow(TObject *Sender)
{
    AutoRefresh1->Enabled = true;
    UpdateChanges();
}
//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::ExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::SmallUpdateChanges()
{
    if (mOfftileBitmap)
    {
        delete mOfftileBitmap;
        mOfftileBitmap = NULL;
    }

    CreateTile();
    //TileAlone->Paint();
}
//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::UpdateChanges()
{
        AnsiString texS;

        if (mOffscreenBitmap)
        {
                delete mOffscreenBitmap;
                mOffscreenBitmap = NULL;
        }

        CreateMap(VRam->Canvas,512,384);

    // Put VDP information
    // Mode and others features
    switch(tms.Mode)
    {
        case 0:eVDPMode->Caption="Gfx1 Mode 0 40x24"; break;
        case 1:eVDPMode->Caption="Txt1 Mode 1 32x24"; break;
        case 2:eVDPMode->Caption="Gfx2 Mode 2 32x24"; break;
        case 3:eVDPMode->Caption="Mult Mode 3 64x48x16"; break;
    }
    switch (tms.VR[1] & 0x3)
    {
        case 0:eVDPOption->Caption="8x8 not magnified"; break;
        case 1:eVDPOption->Caption="8x8 magnified"; break;
        case 2:eVDPOption->Caption="16x16 not magnified"; break;
        case 3:eVDPOption->Caption="16x16 magnified"; break;
    }

    // Address
    eVDPBGmap->Caption="$"+IntToHex(coleco_gettmsaddr(CHRMAP,0,0),4)+"-$"+IntToHex(coleco_gettmsaddr(CHRMAP,0,0)+32*24-1,4);
    eVDPBGtile->Caption="$"+IntToHex(coleco_gettmsaddr(CHRGEN,0,0),4);
    eVDPBGcolor->Caption="$"+IntToHex(coleco_gettmsaddr(CHRCOL,0,0),4)+"-$"+IntToHex(coleco_gettmsaddr(CHRCOL,0,0)+32*24-1,4);
    eVDPSPRdata->Caption="$"+IntToHex(coleco_gettmsaddr(SPRATTR,0,0),4);
    eVDPSPRtile->Caption="$"+IntToHex(coleco_gettmsaddr(SPRGEN,0,0),4);

    // Register
    eVDPR0->Caption="$"+IntToHex(tms.VR[0],2); eVDPR1->Caption="$"+IntToHex(tms.VR[1],2);
    eVDPR2->Caption="$"+IntToHex(tms.VR[2],2); eVDPR3->Caption="$"+IntToHex(tms.VR[3],2);
    eVDPR4->Caption="$"+IntToHex(tms.VR[4],2); eVDPR5->Caption="$"+IntToHex(tms.VR[5],2);
    eVDPR6->Caption="$"+IntToHex(tms.VR[6],2); eVDPR7->Caption="$"+IntToHex(tms.VR[7],2);
    eVDPStat->Caption="$"+IntToHex(tms.SR,2); eVDPlatch->Caption="$"+IntToHex(tms.DLatch,2);
    texS="";
    if (tms.SR & 0x80) texS=texS+"F,";
    if (tms.SR & 0x40) texS=texS+"5th S,";
    if (tms.SR & 0x20) texS=texS+"C,";
    texS=texS+"5thS#"+IntToHex(tms.SR&0x1F,2);
    eVDPStatus->Caption=texS;
    eVDPColF->Color=(TColor) cv_pal32[tms.VR[7]>>4];   eVDPColB->Color=(TColor) cv_pal32[tms.VR[7] & 0xF];

    texS=IntToStr(tms.VR[1]>>4)+" " +IntToStr(tms.VR[1]>>3)+" " +IntToStr(tms.VR[0]>>1);
    eVDPValMod->Caption=texS;
}
//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::do_refresh()
{
    if (AutoRefresh1->Checked) {
      UpdateChanges();
    }
}

//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::AutoRefresh1Click(TObject *Sender)
{
    if (AutoRefresh1->Enabled)
    {
        AutoRefresh1->Checked=!AutoRefresh1->Checked;
        if (AutoRefresh1->Checked)
        {
            UpdateChanges();
        }
    }
}
//---------------------------------------------------------------------------



void __fastcall Tnametabviewer::VRamMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    int ix,iy,it,bgti;

    // Show current information in Details groupbox
    ix=(X)/(VRam->Width/32);
    iy=(Y)/(VRam->Height/24);
    it=coleco_gettmsval(CHRMAP,ix+32*iy,0,0);
    eGVVCloc->Caption="$"+IntToHex(ix,2)+",$"+IntToHex(iy,2);
    eGVVCTNo->Caption="$"+IntToHex(it,2);
//    if (machine.F18A)
//        eGVVCF18No->Caption=IntToHex(coleco_gettmsval(CHRCOL,it,0,0),2);
//    else
        eGVVCF18No->Caption="N/A";
    eGVVCMAddr->Caption="$"+IntToHex(coleco_gettmsaddr(CHRMAP,0,0)+ix+32*iy,4);
    bgti=it*8+coleco_gettmsaddr(CHRGEN,tms.Mode,iy*8);
    eGVVCTAddr->Caption="$"+IntToHex(bgti,4);
    if (tms.Mode==1)
        eGVVCCAddr->Caption="$"+IntToHex((it>>3)+coleco_gettmsaddr(CHRCOL,0,0),4);
    else
        eGVVCCAddr->Caption="$"+IntToHex((it<<3)+coleco_gettmsaddr(CHRCOL,tms.Mode,iy*8),4);

    // Show current tile
    if (bgti != mVramTile)
    {
        mVramTile=bgti;
        SmallUpdateChanges();
    }
}
//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::Copytoclipboard1Click(TObject *Sender)
{
    Graphics::TBitmap *bmp;

    // Get graphcis and copy to actual bitmap
    bmp = new Graphics::TBitmap;
    bmp->Width = 256*2;
    bmp->Height = 192*2;
    bmp->PixelFormat = pf24bit;

    try
    {
        CreateMap(bmp->Canvas, bmp->Width, bmp->Height);
        Clipboard()->Assign(bmp);
    }
    __finally
    {
        delete bmp;
    }
}
//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::chkGridClick(TObject *Sender)
{
        UpdateChanges();
}
//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::chkTilesClick(TObject *Sender)
{
        UpdateChanges();
}
//---------------------------------------------------------------------------

