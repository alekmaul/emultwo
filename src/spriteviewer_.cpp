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
 * spriteviewer_.cpp
 *
 */

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "spriteviewer_.h"

#include "accdraw_.h"
#include "main_.h"
#include "coleco.h"
#include "colecoconfig.h"
#include "tms9928a.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "PaintBoxGrid"
#pragma resource "*.dfm"
Tspriteviewer *spriteviewer;

#define SPXCOR 0
#define SPYCOR 1
#define SPPATR 2
#define SPATTR 3
#define SPDESC 4
#define SPEARL 5

//---------------------------------------------------------------------------
__fastcall Tspriteviewer::Tspriteviewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    ini = new TIniFile(coleco.inipath);
    LoadSettings(ini);
    delete ini;

    LVSprites->Items->Clear();
    for (int i=0;i<32;i++)
    {
        TListItem* newItem = LVSprites->Items->Add();
        newItem->Caption = i+1;
        newItem->SubItems->Add("");
        newItem->SubItems->Add("");
        newItem->SubItems->Add("");
        newItem->SubItems->Add("");
        newItem->SubItems->Add("");
        newItem->SubItems->Add("");
    }
}

//---------------------------------------------------------------------------

void __fastcall Tspriteviewer::UpdateChanges()
{
    CreateSprite();
    //sprScreen->Invalidate();
}

void __fastcall Tspriteviewer::do_refresh()
{
    if (AutoRefresh1->Checked) {
      UpdateChanges();
    }
}

//---------------------------------------------------------------------------

void __fastcall Tspriteviewer::SaveasBMP1Click(TObject *Sender)
{
    Graphics::TBitmap *bmp,*bmps;

    // Get graphcis and copy to actual bitmap
    bmp = new Graphics::TBitmap;
    bmp->Width = 16;
    bmp->Height = 16;
    bmp->PixelFormat = pf32bit;

    bmps = new Graphics::TBitmap;
    bmps->Width = 16*8;
    bmps->Height = 16*4;
    bmps->PixelFormat = pf24bit;

    // Check save dialog to save effectively to disk
    if(dlgSVPic->Execute())
    {
        for (int i=0;i<32;i++)
        {
            RefreshSprite(i*4,bmp->Canvas,bmp->Width,bmp->Height);

            bmps->Canvas->Draw((i*16) % (16*8), (i<8 ? 0 : i<16 ? 16 : i < 24 ? 32 : 48), bmp);

        }
        bmps->SaveToFile(dlgSVPic->FileName);
    }
    delete bmp, bmps;
}

//---------------------------------------------------------------------------

void Tspriteviewer::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("SPVIEW","Top",Top);
    Left = ini->ReadInteger("SPVIEW","Left",Left);
    AutoRefresh1->Checked = ini->ReadBool("SPVIEW", "AutoRefresh", true);
}

void Tspriteviewer::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("SPVIEW","Top",Top);
    ini->WriteInteger("SPVIEW","Left",Left);
    ini->WriteBool("SPVIEW", "AutoRefresh", AutoRefresh1->Checked);
}


//---------------------------------------------------------------------------

void __fastcall Tspriteviewer::ExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall Tspriteviewer::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Form1->SpritesViewer1->Checked=false;
}
//---------------------------------------------------------------------------

void __fastcall Tspriteviewer::FormShow(TObject *Sender)
{
    AutoRefresh1->Enabled = true;
    sprAct=-1;
    UpdateChanges();
}

//---------------------------------------------------------------------------
void __fastcall Tspriteviewer::RefreshSprite(int address, TCanvas *Acanvas,int w, int h)
{
    int iy,it,value,fgcol,bgcol;
    unsigned int *LinePtr;

    // Prepare a 8x8 bitmap for the tile
    Graphics::TBitmap *bmpTile = new Graphics::TBitmap;
    bmpTile->Width = spr8x8 ? 8 : 16;
    bmpTile->Height = spr8x8 ? 8 : 16;
	bmpTile->PixelFormat = pf32bit;
	bmpTile->IgnorePalette = true;

    // Get fg and bg color
    fgcol=cv_pal32[coleco_gettmsval(SPRATTR,address+3,0,0)];
    bgcol=cv_pal32[0];
    if (bgcol == fgcol)
        bgcol=cv_pal32[15];

    // if sprite not on screen, draw a line
    iy=coleco_gettmsval(SPRATTR,address,0,0);
    if (iy>=192)
    {
        bmpTile->Canvas->Brush->Style = bsSolid;
        bmpTile->Canvas->Brush->Color = clBlack;
        if (spr8x8)
            bmpTile->Canvas->Rectangle(0,0,8,8);
        else
            bmpTile->Canvas->Rectangle(0,0,16,16);
        bmpTile->Canvas->Pen->Width = 2;
        bmpTile->Canvas->Pen->Color = clRed;
        bmpTile->Canvas->MoveTo(0,0);
        if (spr8x8)
        	bmpTile->Canvas->LineTo(7, 7);
        else
            bmpTile->Canvas->LineTo(15, 15);
        bmpTile->Canvas->Pen->Color = clBlack;
    }
    else
    {
        // do for 8 or 16 pix
        if (spr8x8)
        {
            // Scan all lines
            for (iy=0;iy<8;iy++) {
                LinePtr = (unsigned int *) bmpTile->ScanLine[iy];
                // Get Pattern
                it=iy+(coleco_gettmsval(SPRATTR,address+2,0,0)*8);

                // Draw bitmap
                value=coleco_gettmsval(SPRGEN,it,0,0);
                *(LinePtr++)= (value & 0x80) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x40) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x20) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x10) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x08) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x04) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x02) ? fgcol : bgcol;
                *(LinePtr)= (value & 0x01) ? fgcol : bgcol;
            }
        }
        else
        {
            // Scan all lines
            for (iy=0;iy<16;iy++) {
                LinePtr = (unsigned int *) bmpTile->ScanLine[iy];
                // Get Pattern
                it=iy+(coleco_gettmsval(SPRATTR,address+2,0,0)*8);

                // Draw bitmap
                value=coleco_gettmsval(SPRGEN,it,0,0);
                *(LinePtr++)= (value & 0x80) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x40) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x20) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x10) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x08) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x04) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x02) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x01) ? fgcol : bgcol;

                // Get Pattern
                it=16+iy+(coleco_gettmsval(SPRATTR,address+2,0,0)*8);

                // Draw bitmap
                value=coleco_gettmsval(SPRGEN,it,0,0);
                *(LinePtr++)= (value & 0x80) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x40) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x20) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x10) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x08) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x04) ? fgcol : bgcol;
                *(LinePtr++)= (value & 0x02) ? fgcol : bgcol;
                *(LinePtr)= (value & 0x01) ? fgcol : bgcol;
            }
        }
    }
    Acanvas->CopyMode = cmSrcCopy;
    Acanvas->StretchDraw(Rect(0,0,w, h), bmpTile);

    delete bmpTile;
}

//---------------------------------------------------------------------------

void __fastcall Tspriteviewer::CreateSprite(void) {
    int ix,iy;

    // if sprites 8x8, change layout
    spr8x8 = ( ((tms.VR[1] & 0x3) == 0) || ((tms.VR[1] & 0x3) == 1) ) ? 1 : 0;
/*    if ( spr8x8 )
    {
        SpriteAlone->GridX=8;
        SpriteAlone->GridY=8;
    }
    else
    {
        SpriteAlone->GridX=16;
        SpriteAlone->GridY=16;
    }*/

    // Refresh table
    sprdisable=0;
    for (int i=0;i<32;i++)
    {
        iy=coleco_gettmsval(SPRATTR,i*4,0,0);
        if (iy==0xD0) sprdisable=1;
        TListItem* newItem = LVSprites->Items->Item[i];
        newItem->SubItems->Strings[SPXCOR] = IntToHex(coleco_gettmsval(SPRATTR,i*4+1,0,0),2);
        newItem->SubItems->Strings[SPYCOR] = IntToHex(iy,2);
        newItem->SubItems->Strings[SPPATR] = IntToHex(coleco_gettmsval(SPRATTR,i*4+2,0,0),2);
        newItem->SubItems->Strings[SPATTR] = IntToHex(coleco_gettmsval(SPRATTR,i*4+3,0,0),2);
        if (sprdisable) newItem->SubItems->Strings[SPDESC]="Disable";
        if (coleco_gettmsval(SPRATTR,i*4+3,0,0) & 0x80)
        {
            if (sprdisable) newItem->SubItems->Strings[SPEARL]="Set";
            else newItem->SubItems->Strings[SPEARL]="Set";
        }
    }

    // Show screen
    if (Form1->RenderMode==RENDERDDRAW)
    {
        StretchBlt(sprScreen->Canvas->Handle, 0, 0, sprScreen->Width, sprScreen->Height,
                    Form1->Canvas->Handle,8,8, Form1->ClientWidth-16,Form1->ClientHeight-16-Form1->StatusBar1->Height,SRCCOPY);
    }
    else {
        StretchBlt(sprScreen->Canvas->Handle, 0, 0, sprScreen->Width, sprScreen->Height,
                    GDIFrame->Canvas->Handle,0,0, TVW,TVH,SRCCOPY);
    }

    // If sprite is selected and ok, we display where it is on screen
    if (sprAct!=-1) {
        ix=coleco_gettmsval(SPRATTR,4*sprAct+1,0,0);
        iy=coleco_gettmsval(SPRATTR,4*sprAct,0,0);
        if ( (iy<192) && (ix) && (iy) ) { // y<192 = sprite on screen
            sprScreen->Canvas->Pen->Width = 2;
            sprScreen->Canvas->Pen->Color = clRed;
            sprScreen->Canvas->MoveTo(0,0);
      	    sprScreen->Canvas->LineTo(ix, iy);
            sprScreen->Canvas->Brush->Style=bsClear;
            if (spr8x8)
                sprScreen->Canvas->Rectangle(ix-1,iy-1,ix+8,iy+8);
            else
                sprScreen->Canvas->Rectangle(ix-1,iy-1,ix+16,iy+16);
        }
        eVVSprCurSAddr->Caption="$"+IntToHex(coleco_gettmsaddr(SPRATTR,0,0)+4*sprAct,4);
        eVVSprCurTAddr->Caption="$"+IntToHex(coleco_gettmsaddr(SPRGEN,0,0)+8*(coleco_gettmsval(SPRATTR,4*sprAct+2,0,0)),4);

        RefreshSprite(sprAct*4,SpriteAlone->Canvas,96,96);
        //SpriteAlone->Invalidate();
    }
}



void __fastcall Tspriteviewer::LVSpritesDrawItem(TCustomListView *Sender,
      TListItem *Item, TRect &Rect, TOwnerDrawState State)
{
    if (sprdisable)
       Sender->Canvas->Font->Color=clInactiveCaptionText;        
}
//---------------------------------------------------------------------------

void __fastcall Tspriteviewer::LVSpritesSelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
    int ix,iy;

    TListItem* selected = LVSprites->Selected;
    if (selected)
    {
        sprAct=selected->Index;
        UpdateChanges();
    }
    else
        sprAct=-1;
}
//---------------------------------------------------------------------------

void __fastcall Tspriteviewer::AutoRefresh1Click(TObject *Sender)
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

void __fastcall Tspriteviewer::Refresh1Click(TObject *Sender)
{
    UpdateChanges();
}

//---------------------------------------------------------------------------

void __fastcall Tspriteviewer::Copytoclipboard1Click(TObject *Sender)
{
    Graphics::TBitmap *bmp,*bmps;

    // Get graphcis and copy to actual bitmap
    bmp = new Graphics::TBitmap;
    bmp->Width = 16;
    bmp->Height = 16;
    bmp->PixelFormat = pf32bit;

    bmps = new Graphics::TBitmap;
    bmps->Width = 16*8;
    bmps->Height = 16*4;
    bmps->PixelFormat = pf24bit;

    // Save to clipboard
    try
    {
        for (int i=0;i<32;i++)
        {
            RefreshSprite(i*4,bmp->Canvas,bmp->Width,bmp->Height);

            bmps->Canvas->Draw((i*16) % (16*8), (i<8 ? 0 : i<16 ? 16 : i < 24 ? 32 : 48), bmp);

        }
        Clipboard()->Assign(bmps);
    }
    __finally
    {
        delete bmp, bmps;
    }
}
//---------------------------------------------------------------------------

