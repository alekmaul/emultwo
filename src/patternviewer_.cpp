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
 * tileviewer_.cpp
 *
 */

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "patternviewer_.h"

#include "main_.h"
#include "coleco.h"
#include "colecoconfig.h"
#include "tms9928a.h"
#include "utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PaintBoxGrid"
#pragma link "PaintBoxGrid"
#pragma link "PaintBoxGrid"
#pragma resource "*.dfm"
Tpatternviewer *patternviewer;
/*
#define chrgen  0x0000 -> size 0x1800 = 256 char
#define chrtab  0x1800 -> size 0x0300 = 24 *32 NAME TABLE
#define sprgen  0x1b00 -> size 0x0400 = 256*4 sprites
#define coltab  0x2000 -> size 0x1800
#define sprtab  0x3800 -> size 0x0800 (2048)
end in          0x4000

The memory map usually goes by
0x0000 - char table 1
0x0800 - char table 2
0x1000 - char table 3
0x1800 - name table 1 - which is on at default
0x1b00 - sprite data which holds the x,y,pattern, and color information for up to 64 sprites, but only 32 sprite maximum can be displayed on screen
0x1c00 - name table 2 - you can swap to this table.  I think this would be useful for inventory screen, and act like double buffer if you don't want to change anything in name table 1.
0x2000 - color table 1
0x2800 - color table 2
0x3000 - color table 3
0x3800 - sprite tileset/generator

*/

extern int coleco_updatetms;                         // 1 if TMS is updated

//---------------------------------------------------------------------------
__fastcall Tpatternviewer::Tpatternviewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    mBaseVram = 0x0000;    // default show 0
    mVramTile = 0x0000;

    ini = new TIniFile(coleco.inipath);
    LoadSettings(ini);
    delete ini;

    mOffscreenBitmap=NULL;
    mOfftileBitmap=NULL;
}

//---------------------------------------------------------------------------

void Tpatternviewer::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("PATVIEW","Top",Top);
    Left = ini->ReadInteger("PATVIEW","Left",Left);

    AutoRefresh1->Checked = ini->ReadBool("PATVIEW", "AutoRefresh", true);
}

void Tpatternviewer::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("PATVIEW","Top",Top);
    ini->WriteInteger("PATVIEW","Left",Left);
    ini->WriteBool("TVIEW", "AutoRefresh", AutoRefresh1->Checked);
}

//---------------------------------------------------------------------------
void Tpatternviewer::UpdateOfsText(void)
{
    int bgtil=coleco_gettmsaddr(CHRGEN,0,0);
    int bgmap=coleco_gettmsaddr(CHRMAP,0,0);
    int bgcol=coleco_gettmsaddr(CHRCOL,0,0);
    int sprtil=coleco_gettmsaddr(SPRGEN,0,0);
    int spratr=coleco_gettmsaddr(SPRATTR,0,0);

    if ( (mBaseVram >=bgmap) && (mBaseVram<bgmap+0x0300) )
    {
        eVRAMTxt->Caption="BG Map";
    }
    else if ( (mBaseVram >=bgtil) && (mBaseVram<bgtil+0x1800) )
    {
        eVRAMTxt->Caption="BG Tile";
    }
    else if ( (mBaseVram >=bgcol) && (mBaseVram<bgcol+0x1800) )
    {
        eVRAMTxt->Caption="BG Col";
    }
    else if ( (mBaseVram >=sprtil) && (mBaseVram<sprtil+0x0800) )
    {
        eVRAMTxt->Caption="SPR Tile";
    }
    else if ( (mBaseVram >=spratr) && (mBaseVram<spratr+0x0400) )
    {
        eVRAMTxt->Caption="SPR Attr";
    }
}

//---------------------------------------------------------------------------

void __fastcall Tpatternviewer::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Form1->TilesViewer1->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall Tpatternviewer::FormShow(TObject *Sender)
{
    AutoRefresh1->Enabled = true;
    UpdateChanges(); // to refresh screen the first time
}

//---------------------------------------------------------------------------

void __fastcall Tpatternviewer::ExitClick(TObject *Sender)
{
    Close();
}

//---------------------------------------------------------------------------

void __fastcall Tpatternviewer::SaveasBMP1Click(TObject *Sender)
{
    Graphics::TBitmap *bmp;

    // Get graphcis and copy to actual bitmap
    bmp = new Graphics::TBitmap;
    bmp->Width = 256; //128;
    bmp->Height = 256; // 128;
    bmp->PixelFormat = pf24bit;

    // Check save dialog to save effectively to disk
    if(dlgSVPic->Execute())
    {
        CreateBitmap(bmp->Canvas,bmp->Width, bmp->Height);
        if (dlgSVPic->FilterIndex==1)
                bmp->SaveToFile(dlgSVPic->FileName);
        else
                ImageToPNG(dlgSVPic->FileName, bmp);
    }
    delete bmp;
}

//---------------------------------------------------------------------------
void Tpatternviewer::CreateTile(void)
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

        // if no color, specific adressing
        /*if (addrcol == 0xffff) {
            // Get Pattern
            value=coleco_gettmsval(VRAM,iy+addrgen,0,0);
        }
        else*/ {
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

                // if B&W mode choosen, just change colors
                if (rBW->Checked==true) {
                    fgcol=cv_pal32[15];
                    bgcol=cv_pal32[0];
                }
            }
            // not bg, vram or sprite
            else {
                fgcol=cv_pal32[15];
                bgcol=cv_pal32[0];
                value = coleco_gettmsval(VRAM,mVramTile+(iy&7),0x00,0x00);
            }
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
}

//---------------------------------------------------------------------------
#define ROWCNT 256
#define LINECNT 256
void Tpatternviewer::CreateBitmap(TCanvas *Acanvas, int w, int h)
{
        int ix,iy,it,vcol, value,fgcol,bgcol;
        byte ic;
        unsigned int *LinePtr;

        if (mOffscreenBitmap)
        {
                delete mOffscreenBitmap;
        }

        mOffscreenBitmap = new Graphics::TBitmap;
        mOffscreenBitmap->Width = ROWCNT;
        mOffscreenBitmap->Height =LINECNT;
	mOffscreenBitmap->PixelFormat = pf32bit;
	mOffscreenBitmap->IgnorePalette = true;

        // Scan all lines
        for (iy=0;iy<LINECNT;iy++) {
                LinePtr = (unsigned int *) mOffscreenBitmap->ScanLine[iy];
                for (ix=0;ix<(ROWCNT/8);ix++) {
                        // Get Pattern
                        it=(ix*8)+LINECNT*(iy>>3);//+mBaseVram;

                        // Compute colors only in chrgen area
                        if ( (it >=coleco_gettmsaddr(CHRGEN,0,0)) && (it<coleco_gettmsaddr(CHRGEN,0,0)+0x1800) ) {
                                if (tms.Mode==1) {
                                        vcol =coleco_gettmsval(CHRCOL,it,tms.Mode,iy>>3);
                                }
                                else
                                {
                                        if (it >=0x1000)
                                                vcol =coleco_gettmsval(CHRCOL,(it &0x7ff)+(iy&7),tms.Mode,0x80);
                                        else if (it >=0x800)
                                                vcol =coleco_gettmsval(CHRCOL,(it &0x7ff)+(iy&7),tms.Mode,0x40);
                                        else
                                                vcol =coleco_gettmsval(CHRCOL,(it &0x7ff)+(iy&7),tms.Mode,0x00);
                                }
                                // Get fg and bg color
                                fgcol=cv_pal32[vcol>>4];
                                bgcol=cv_pal32[vcol & 0xf];
                                if (it >=0x1000)
                                        value =coleco_gettmsval(CHRGEN,(it &0x7ff)+(iy&7),tms.Mode,0x80);
                                else if (it >=0x800)
                                        value =coleco_gettmsval(CHRGEN,(it &0x7ff)+(iy&7),tms.Mode,0x40);
                                else
                                        value =coleco_gettmsval(CHRGEN,(it &0x7ff)+(iy&7),tms.Mode,0x00);
                                // if B&W mode choosen, just change colors
                                if (rBW->Checked==true) {
                                        fgcol=cv_pal32[15];
                                        bgcol=cv_pal32[0];
                                }
                        }
                        // not bg, vram or sprite
                        else {
                                fgcol=cv_pal32[15];
                                bgcol=cv_pal32[0];
                                value = coleco_gettmsval(VRAM,it+(iy&7),0x00,0x00);
                        }

                        // Draw bitmap
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
        StretchBlt(Acanvas->Handle, 0,0, w, h,
                mOffscreenBitmap->Canvas->Handle, 0, 0, 256,LINECNT,SRCCOPY);

        // Draw a grid of 16x16 pix if needed
        if (chkGrid->Checked)
        {
                Acanvas->Brush->Style = bsClear;
                Acanvas->Pen->Style = psSolid;
                Acanvas->Pen->Color = cl3DDkShadow;
                for (ix=0;ix<Width;ix+=w/32) {
                        Acanvas->MoveTo(ix,0);
                        Acanvas->LineTo(ix,h);
                }
                for (ix=0;ix<Height;ix+=h/32) {
                        Acanvas->MoveTo(0,ix);
                        Acanvas->LineTo(w,ix);
                }
        }
}

//---------------------------------------------------------------------------

void __fastcall Tpatternviewer::SmallUpdateChanges()
{
    if (mOfftileBitmap)
    {
        delete mOfftileBitmap;
        mOfftileBitmap = NULL;
    }
    CreateTile();
}

void __fastcall Tpatternviewer::UpdateChanges()
{
    if (mOffscreenBitmap)
    {
        delete mOffscreenBitmap;
        mOffscreenBitmap = NULL;
    }

    CreateBitmap(VRam->Canvas,512,512);
}

void __fastcall Tpatternviewer::do_refresh()
{
    if (AutoRefresh1->Checked) {
        if (coleco_updatetms)
        {
            UpdateOfsText();
            UpdateChanges();
            coleco_updatetms=0;
        }
    }
}

//---------------------------------------------------------------------------

void __fastcall Tpatternviewer::AutoRefresh1Click(TObject *Sender)
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

void __fastcall Tpatternviewer::Refresh1Click(TObject *Sender)
{
    UpdateChanges();
}
//---------------------------------------------------------------------------

void __fastcall Tpatternviewer::VRamMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    int ix,iy,it;

    // Show current information in Details groupbox
    ix=(X)/(VRam->Width/32);
    iy=(Y)/(VRam->Height/32);
    mBaseVram=ix*8+256*iy;
    //it=ix*8+256*iy+mBaseVram;
    it=mBaseVram;
    eBGTTileNo->Caption="$"+IntToHex(ix+32*iy/*+(mBaseVram/8)*/,3);
    eBGTTileAdr->Caption="$"+IntToHex(coleco_gettmsaddr(VRAM,0,0)+it,4);
//    if (machine.F18A)
//        eGVVCF18No->Caption=IntToHex(coleco_gettmsval(CHRCOL,it,0,0),2);
//    else
        eGVVCF18No->Caption="N/A";
    if (it != mVramTile)
    {
        mVramTile=it;
        UpdateOfsText();
        SmallUpdateChanges();
    }
}

//---------------------------------------------------------------------------

void __fastcall Tpatternviewer::Copytoclipboard1Click(TObject *Sender)
{
    Graphics::TBitmap *bmp;

    // Get graphcis and copy to actual bitmap
    bmp = new Graphics::TBitmap;
    bmp->Width = 256; //128;
    bmp->Height = 256; // 128;
    bmp->PixelFormat = pf24bit;

    try
    {
        CreateBitmap(bmp->Canvas,bmp->Width, bmp->Height);
        Clipboard()->Assign(bmp);
    }
    __finally
    {
        delete bmp;
    }
}
//---------------------------------------------------------------------------

void __fastcall Tpatternviewer::rColClick(TObject *Sender)
{
    UpdateChanges();
}
//---------------------------------------------------------------------------


