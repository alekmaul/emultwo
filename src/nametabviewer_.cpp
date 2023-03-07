/* EmulTwo  - A Windows Colecovision emulator.
 * Copyright (C) 2018-2023 Alekmaul
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
#include "f18a.h"
#include "tms9928a.h"

#include "editvalue_.h"
#include "utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma resource "*.dfm"
Tnametabviewer *nametabviewer;
//---------------------------------------------------------------------------
__fastcall Tnametabviewer::Tnametabviewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    mVramTile = 0x0000;

    ini = new TIniFile(emul2.inipath);
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
    dlgSVPic->DefaultExt="png";
    dlgSVPic->FileName="*.png";
    dlgSVPic->Filter="Portable Network Graphic (.png)|*.png|Windows Bitmap (.bmp)|*.bmp";
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
    int ix, iy,value,vcol,ofsvram, fgcol,bgcol;
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

    for (iy=0;iy<8;iy++) {
        LinePtr = (unsigned int *) mOfftileBitmap->ScanLine[iy];

        if ( (mVramTile >=coleco_gettmsaddr(CHRGEN,0,0)) && (mVramTile<coleco_gettmsaddr(CHRGEN,0,0)+0x1800) ) {
            ofsvram=mVramTile-coleco_gettmsaddr(CHRGEN,0,0); // get offset in VRAM
            if ( (tms.Mode==1) || (f18a.Mode==F18A_MODE_GRAPHICS)) {
                vcol =coleco_gettmsval(CHRCOL,mVramTile,tms.Mode,iy>>3);
            }
            else {
                if (ofsvram >=0x1000)
                    vcol =coleco_gettmsval(CHRCOL,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x80);
                else if (ofsvram >=0x800)
                    vcol =coleco_gettmsval(CHRCOL,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x40);
                else
                    vcol =coleco_gettmsval(CHRCOL,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x00);
            }
            // Get fg and bg color
            if (chkBW->Checked==true) {
                fgcol=(DWORD)(0x00FFFFFF);
                bgcol=cv_pal32[0];
            }
            else {
                fgcol=cv_pal32[tms.IdxPal[vcol>>4]];
                bgcol=cv_pal32[tms.IdxPal[vcol & 0xf]];
            }
            if (ofsvram >=0x1000)
                value =coleco_gettmsval(CHRGEN,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x80);
            else if (ofsvram >=0x800)
                value =coleco_gettmsval(CHRGEN,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x40);
            else
                value =coleco_gettmsval(CHRGEN,(mVramTile &0x7ff)+(iy&7),tms.Mode,0x00);
        }
        // not bg, vram or sprite
        else {
            if (chkBW->Checked==true) {
                fgcol=(DWORD)(0x00FFFFFF);
                bgcol=cv_pal32[0];
            }
            else {
                fgcol=cv_pal32[15];
                bgcol=cv_pal32[0];
            }
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
    StretchBlt(TileAlone->Canvas->Handle, 0,0, 128,128,
        mOfftileBitmap->Canvas->Handle, 0, 0, 8, 8,SRCCOPY);

    // Draw a grid of 16x16 pix if needed
    if (chkGrid->Checked)
    {
        TileAlone->Canvas->Brush->Style = bsClear;
        TileAlone->Canvas->Pen->Style = psSolid;
        TileAlone->Canvas->Pen->Color = cl3DDkShadow;
        for (ix=0;ix<Width;ix+=128/8)
        {
            TileAlone->Canvas->MoveTo(ix,0);
            TileAlone->Canvas->LineTo(ix,128);
        }
        for (ix=0;ix<Height;ix+=128/8)
        {
            TileAlone->Canvas->MoveTo(0,ix);
            TileAlone->Canvas->LineTo(128,ix);
        }
    }
}

//---------------------------------------------------------------------------
void Tnametabviewer::CreateMap(TCanvas *Acanvas, int w, int h)
{
    int ix,iy,it,value,fgcol,bgcol;
    BYTE memtile[32*24];
    unsigned int *LinePtr;
    BYTE ic;
    int Offset;

    // Prepare a 8x8 bitmap each the tile (no need of border)
    Graphics::TBitmap *bmpTile = new Graphics::TBitmap;
    bmpTile->Width = 256;
    bmpTile->Height = 192;
	bmpTile->PixelFormat = pf32bit;
	bmpTile->IgnorePalette = true;

    // Scan all lines
    for (iy=0;iy<192;iy++) {
/*        for (ix=0;ix<32;ix++) {
            // Get Pattern
            it=coleco_gettmsval(iMode,ix+((iy & 0xF8)<<2),0,0);
            memtile[ix+(iy>>3)*32]=it;
        }*/

        LinePtr = (unsigned int *) bmpTile->ScanLine[iy];
        Offset = (iy&7);
        for (ix=0;ix<32;ix++) {
            // Get Pattern
            it=coleco_gettmsval(VRAM,nNamTabVal+ix+((iy & 0xF8)<<2),0,0);
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
            if (chkBW->Checked==true) {
                fgcol=(DWORD)(0x00FFFFFF);
                bgcol=cv_pal32[0];
            }
            else {
                fgcol=cv_pal32[(ic>>4) & 0x0f];
                bgcol=cv_pal32[(ic & 0x0f)];
            }

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
#if 0
    if (Form1->RenderMode==RENDERDDRAW) {
        StretchBlt(Acanvas->Handle, 0, 0, w, h,
            Form1->Canvas->Handle,8,8, Form1->ClientWidth-16,Form1->ClientHeight-16-Form1->StatusBar1->Height,SRCCOPY);
    }
    else {
        StretchBlt(Acanvas->Handle, 0, 0, w, h,
            GDIFrame->Canvas->Handle,BDW/2,BDH/2, TVW-BDW,TVH-BDH,SRCCOPY); // BDW,BDH because of border
    }
#endif
    StretchBlt(Acanvas->Handle, 0,0, w, h,bmpTile->Canvas->Handle, 0, 0, 256, 192,SRCCOPY);
    delete bmpTile;

    // Draw a grid of 16x16 pix if needed
    if (chkGrid->Checked) {
        Acanvas->Brush->Style = bsClear;
        Acanvas->Pen->Style = psSolid;
        Acanvas->Pen->Color = cl3DDkShadow;
        for (ix=0;ix<Width;ix+=16) {
            Acanvas->MoveTo(ix,0);
            Acanvas->LineTo(ix,h);
        }
        for (ix=0;ix<Height;ix+=16) {
            Acanvas->MoveTo(0,ix);
            Acanvas->LineTo(w,ix);
        }
    }

    // if tile number, draw them
    if (chkTiles->Checked) {
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
        for (iy=0;iy<24;iy++) {
            for (ix=0;ix<32;ix++) {
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
    nNamTabVal = coleco_gettmsaddr(CHRMAP,0,0);
    eGVVCMAddr->Caption="$"+IntToStr(nNamTabVal);
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
    if (mOffscreenBitmap) {
        delete mOffscreenBitmap;
        mOffscreenBitmap = NULL;
    }

    CreateMap(VRam->Canvas,512,384);
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
    if (AutoRefresh1->Enabled) {
        AutoRefresh1->Checked=!AutoRefresh1->Checked;
        if (AutoRefresh1->Checked) {
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
    it=coleco_gettmsval(VRAM,nNamTabVal+ix+32*iy,0,0);
    eGVVCloc->Caption="$"+IntToHex(ix,2)+",$"+IntToHex(iy,2);
    eGVVCTNo->Caption="$"+IntToHex(it,2);
    eGVVCMAddr->Caption="$"+IntToHex(nNamTabVal+ix+32*iy,4);
    bgti=it*8+coleco_gettmsaddr(CHRGEN,tms.Mode,iy*8);
    eGVVCTAddr->Caption="$"+IntToHex(bgti,4);
    if (tms.Mode==1)
        eGVVCCAddr->Caption="$"+IntToHex((it>>3)+coleco_gettmsaddr(CHRCOL,0,0),4);
    else
        eGVVCCAddr->Caption="$"+IntToHex((it<<3)+coleco_gettmsaddr(CHRCOL,tms.Mode,iy*8),4);

    // Show current tile
    if (bgti != mVramTile) {
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
    SmallUpdateChanges();
}
//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::chkTilesClick(TObject *Sender)
{
    UpdateChanges();
}
//---------------------------------------------------------------------------


void __fastcall Tnametabviewer::eGVVCMAddrClick(TObject *Sender)
{
    editvalue->CentreOn(this);

    int n = StrToInt(nNamTabVal);
    if (editvalue->Edit2(n, 2))
    {
        eGVVCMAddr->Caption = "$"+IntToHex(n,4);
        nNamTabVal=n;
        UpdateChanges();
    }
}
//---------------------------------------------------------------------------

void __fastcall Tnametabviewer::bRestCHMapClick(TObject *Sender)
{
    nNamTabVal = coleco_gettmsaddr(CHRMAP,0,0);
    eGVVCMAddr->Caption="$"+IntToHex(nNamTabVal,4);
}
//---------------------------------------------------------------------------

