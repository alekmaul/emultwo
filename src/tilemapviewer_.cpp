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
 * tilemapviewer_.cpp
 *
 */
#include <vcl.h>
#pragma hdrstop

#include "tilemapviewer_.h"

#include "main_.h"
#include "coleco.h"
#include "colecoconfig.h"
#include "tms9928a.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PaintBoxGrid"
#pragma resource "*.dfm"
TTilemapViewer *TilemapViewer;
//---------------------------------------------------------------------------
__fastcall TTilemapViewer::TTilemapViewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    mBaseVram = 0x0000;    // default show 0
    mVramTile = 0x0000;

    ini = new TIniFile(machine.inipath);
    LoadSettings(ini);
    delete ini;

    mOffscreenBitmap=NULL;
    mOfftileBitmap=NULL;

    VRam->FbmpPaint->Width=VRam->ClientWidth;VRam->FbmpPaint->Height=VRam->ClientHeight;
    TileAlone->FbmpPaint->Width=TileAlone->ClientWidth;TileAlone->FbmpPaint->Height=TileAlone->ClientHeight;
}

//---------------------------------------------------------------------------

void TTilemapViewer::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("TMVIEW","Top",Top);
    Left = ini->ReadInteger("TMVIEW","Left",Left);

    mBaseVram = ini->ReadInteger("TMVIEW","VRAMBASE",mBaseVram);

    AutoRefresh1->Checked = ini->ReadBool("TMVIEW", "AutoRefresh", true);
}

void TTilemapViewer::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("TMVIEW","Top",Top);
    ini->WriteInteger("TMVIEW","Left",Left);
    ini->WriteInteger("TMVIEW","VRAMBASE",mBaseVram);
    ini->WriteBool("TMVIEW", "AutoRefresh", AutoRefresh1->Checked);
}

//---------------------------------------------------------------------------
void __fastcall TTilemapViewer::SaveasBMP1Click(TObject *Sender)
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
        CreateMap(bmp);
        bmp->SaveToFile(dlgSVPic->FileName);
    }
    delete bmp;
}

//---------------------------------------------------------------------------
void TTilemapViewer::CreateTile(void)
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
        idTiValue->Caption=idTiValue->Caption+IntToHex(value,2);
        if (iy!=7)
            idTiValue->Caption=idTiValue->Caption+",";
    }
    StretchBlt(TileAlone->FbmpPaint->Canvas->Handle, 0,0, 136,136,
        mOfftileBitmap->Canvas->Handle, 0, 0, 8, 8,SRCCOPY);
}

//---------------------------------------------------------------------------
void TTilemapViewer::CreateMap(Graphics::TBitmap *Abitmap)
{
    int ix,iy,it,value,fgcol,bgcol;
    byte ic;
    unsigned int *LinePtr;
    int Offset;
    byte memtile[32*24];

    // Prepare a 8x8 bitmap for the tile (no need of border)
    Graphics::TBitmap *bmpTile = new Graphics::TBitmap;
    bmpTile->Width = 256;
    bmpTile->Height = 192;
	bmpTile->PixelFormat = pf32bit;
	bmpTile->IgnorePalette = true;

    // Scan all lines
    for (iy=0;iy<192;iy++) {
        LinePtr = (unsigned int *) bmpTile->ScanLine[iy];
        Offset = (iy&7);
        for (ix=0;ix<32;ix++) {
            // Get Pattern
            it=coleco_gettmsval(CHRMAP,ix+((iy & 0xF8)<<2),0,0);
            memtile[ix+(iy>>3)*32]=it;
            it=it*8+Offset;

            if (tms.Mode==1) {
                ic=coleco_gettmsval(CHRCOL,it,0,0);
                //it=it*8+Offset;
            }
            else
            {
                //it=it*8+Offset;
                ic=coleco_gettmsval(CHRCOL,it,tms.Mode,iy);
            }

            // Get fg and bg color
            fgcol=cv_pal32[(ic>>4) & 0x0f];
            bgcol=cv_pal32[(ic & 0x0f)];

            // Draw bitmap
            value=coleco_gettmsval(CHRGEN,it,tms.Mode,iy);
            *(LinePtr++)= (value & 0x80) ? fgcol : bgcol;
            *(LinePtr++)= (value & 0x40) ? fgcol : bgcol;
            *(LinePtr++)= (value & 0x20) ? fgcol : bgcol;
            *(LinePtr++)= (value & 0x10) ? fgcol : bgcol;
            *(LinePtr++)= (value & 0x08) ? fgcol : bgcol;
            *(LinePtr++)= (value & 0x04) ? fgcol : bgcol;
            *(LinePtr++)= (value & 0x02) ? fgcol : bgcol;
            *(LinePtr++)= (value & 0x01) ? fgcol : bgcol;
        }
    }
    StretchBlt(Abitmap->Canvas->Handle, 0,0, Abitmap->Width, Abitmap->Height,bmpTile->Canvas->Handle, 0, 0, 256, 192,SRCCOPY);
    delete bmpTile;

    // if tile number, draw them
    if (ShowTiles1->Checked == true) {
        LOGFONT lf;
        ZeroMemory(&lf, sizeof(LOGFONT));
        lf.lfHeight = 12;
        //lf.lfWidth = 5;
        //lf.lfEscapement = 1;
        lf.lfCharSet = DEFAULT_CHARSET;
        lf.lfQuality = NONANTIALIASED_QUALITY;
        strcpy(lf.lfFaceName, "Courier New");
        Abitmap->Canvas->Font->Handle = CreateFontIndirect(&lf);
        Abitmap->Canvas->Font->Color = (TColor) RGB(64,255,0); //clRed;
        //Abitmap->Canvas->Font->Size = 7;
        Abitmap->Canvas->Brush->Style = bsClear;
        int stepX=Abitmap->Width/32;
        int stepY=Abitmap->Height/24;
        for (iy=0;iy<24;iy++) {
            for (ix=0;ix<32;ix++) {
                Abitmap->Canvas->TextOut(stepX*ix, stepY*iy, IntToHex(memtile[ix+iy*32],2));
            }
        }
    }
}

//---------------------------------------------------------------------------

void __fastcall TTilemapViewer::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Form1->TilemapViewer1->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall TTilemapViewer::FormShow(TObject *Sender)
{
    AutoRefresh1->Enabled = true;
    UpdateChanges();
}
//---------------------------------------------------------------------------

void __fastcall TTilemapViewer::ExitClick(TObject *Sender)
{
    Close();
}

//---------------------------------------------------------------------------

void __fastcall TTilemapViewer::SmallUpdateChanges()
{
    if (mOfftileBitmap)
    {
        delete mOfftileBitmap;
        mOfftileBitmap = NULL;
    }

    CreateTile();
    TileAlone->Paint();
}

void __fastcall TTilemapViewer::UpdateChanges()
{
    AnsiString texS;

    if (mOffscreenBitmap)
    {
        delete mOffscreenBitmap;
        mOffscreenBitmap = NULL;
    }

    CreateMap(VRam->FbmpPaint);
    VRam->Paint();

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
    eVDPBGmap->Caption=IntToHex(coleco_gettmsaddr(CHRMAP,0,0),4)+"-"+IntToHex(coleco_gettmsaddr(CHRMAP,0,0)+32*24-1,4);
    eVDPBGtile->Caption=IntToHex(coleco_gettmsaddr(CHRGEN,0,0),4);
    eVDPBGcolor->Caption=IntToHex(coleco_gettmsaddr(CHRCOL,0,0),4)+"-"+IntToHex(coleco_gettmsaddr(CHRCOL,0,0)+32*24-1,4);
    eVDPSPRdata->Caption=IntToHex(coleco_gettmsaddr(SPRATTR,0,0),4);
    eVDPSPRtile->Caption=IntToHex(coleco_gettmsaddr(SPRGEN,0,0),4);

    // Register
    eVDPR0->Caption=IntToHex(tms.VR[0],2); eVDPR1->Caption=IntToHex(tms.VR[1],2);
    eVDPR2->Caption=IntToHex(tms.VR[2],2); eVDPR3->Caption=IntToHex(tms.VR[3],2);
    eVDPR4->Caption=IntToHex(tms.VR[4],2); eVDPR5->Caption=IntToHex(tms.VR[5],2);
    eVDPR6->Caption=IntToHex(tms.VR[6],2); eVDPR7->Caption=IntToHex(tms.VR[7],2);
    eVDPStat->Caption=IntToHex(tms.SR,2); eVDPlatch->Caption=IntToHex(tms.DLatch,2);
    texS="";
    if (tms.SR & 0x80) texS=texS+"F,";
    if (tms.SR & 0x40) texS=texS+"5th S,";
    if (tms.SR & 0x20) texS=texS+"C,";
    texS=texS+"5thS#"+IntToHex(tms.SR&0x1F,2);
    eVDPStatus->Caption=texS;
    eVDPColF->Color=cv_pal32[tms.VR[7]>>4];   eVDPColB->Color=cv_pal32[tms.VR[7] & 0xF];
}

void __fastcall TTilemapViewer::do_refresh()
{
    if (AutoRefresh1->Checked) {
      UpdateChanges();
    }
}

//---------------------------------------------------------------------------

void __fastcall TTilemapViewer::AutoRefresh1Click(TObject *Sender)
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

void __fastcall TTilemapViewer::ShowGrid1Click(TObject *Sender)
{
    ShowGrid1->Checked = !ShowGrid1->Checked;
    VRam->Grid = ShowGrid1->Checked ? true : false;
}
//---------------------------------------------------------------------------

void __fastcall TTilemapViewer::ShowTiles1Click(TObject *Sender)
{
    ShowTiles1->Checked = !ShowTiles1->Checked;
    UpdateChanges();
}

//---------------------------------------------------------------------------

void __fastcall TTilemapViewer::VRamMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    int ix,iy,it,bgti;

    // Show current information in Details groupbox
    ix=(X)/(VRam->Width/32);
    iy=(Y)/(VRam->Height/24);
    it=coleco_gettmsval(CHRMAP,ix+32*iy,0,0);
    eGVVCloc->Caption=IntToHex(ix,2)+","+IntToHex(iy,2);
    eGVVCTNo->Caption=IntToHex(it,2);
    if (machine.F18A)
        eGVVCF18No->Caption=IntToHex(coleco_gettmsval(CHRCOL,it,0,0),2);
    else
        eGVVCF18No->Caption="N/A";
    eGVVCMAddr->Caption=IntToHex(coleco_gettmsaddr(CHRMAP,0,0)+ix+32*iy,4);
    bgti=it*8+coleco_gettmsaddr(CHRGEN,tms.Mode,iy*8);
    eGVVCTAddr->Caption=IntToHex(bgti,4);
    if (tms.Mode==1)
        eGVVCCAddr->Caption=IntToHex((it>>3)+coleco_gettmsaddr(CHRCOL,0,0),4);
    else
        eGVVCCAddr->Caption=IntToHex((it<<3)+coleco_gettmsaddr(CHRCOL,tms.Mode,iy*8),4);

    // Show current tile
    if (bgti != mVramTile)
    {
        mVramTile=bgti;
        SmallUpdateChanges();
    }
}
//---------------------------------------------------------------------------

void __fastcall TTilemapViewer::Copytoclipboard1Click(TObject *Sender)
{
    Graphics::TBitmap *bmp;

    // Get graphcis and copy to actual bitmap
    bmp = new Graphics::TBitmap;
    bmp->Width = 256*2;
    bmp->Height = 192*2;
    bmp->PixelFormat = pf24bit;

    try
    {
        CreateMap(bmp);
        Clipboard()->Assign(bmp);
    }
    __finally
    {
        delete bmp;
    }
}
//---------------------------------------------------------------------------

