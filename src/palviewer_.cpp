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
 * palviewer_.cpp
 *
*/

#include <vcl.h>
#pragma hdrstop

#include "palviewer_.h"

#include "main_.h"
#include "coleco.h"
#include "colecoconfig.h"
#include "tms9928a.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tpaletteviewer *paletteviewer;
//---------------------------------------------------------------------------
__fastcall Tpaletteviewer::Tpaletteviewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    ini = new TIniFile(coleco.inipath);
    LoadSettings(ini);
    delete ini;
}

//---------------------------------------------------------------------------
void __fastcall Tpaletteviewer::ExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall Tpaletteviewer::FormClose(TObject *Sender, TCloseAction &Action)
{
    Form1->PaletteViewer1->Checked=false;
}


//---------------------------------------------------------------------------
void __fastcall Tpaletteviewer::ChangeCurrentColor(int ColorNum)
{
    pCurCol->Color = TColor(RGB(cv_palette[ColorNum*3],cv_palette[ColorNum*3+1],cv_palette[ColorNum*3+2]));
    eCurColR->Caption = "$"+IntToHex(cv_palette[ColorNum*3],2);
    eCurColG->Caption = "$"+IntToHex(cv_palette[ColorNum*3+1],2);
    eCurColB->Caption = "$"+IntToHex(cv_palette[ColorNum*3+2],2);
    eCurColIdx->Caption = "$"+IntToHex(ColorNum,2);
}

//---------------------------------------------------------------------------
void __fastcall Tpaletteviewer::pCurCol0MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    int tag = -1;

    if (dynamic_cast < TComponent * > (Sender))
    {
        TComponent *comp = (TComponent *) Sender;
        tag = comp->Tag;
    }
    if(tag!=-1)
        ChangeCurrentColor(tag);
}

//---------------------------------------------------------------------------
void __fastcall Tpaletteviewer::UpdateChanges()
{
    pCurCol0->Color = TColor(RGB(cv_palette[0*3],cv_palette[0*3+1],cv_palette[0*3+2]));
    pCurCol1->Color = TColor(RGB(cv_palette[1*3],cv_palette[1*3+1],cv_palette[1*3+2]));
    pCurCol2->Color = TColor(RGB(cv_palette[2*3],cv_palette[2*3+1],cv_palette[2*3+2]));
    pCurCol3->Color = TColor(RGB(cv_palette[3*3],cv_palette[3*3+1],cv_palette[3*3+2]));
    pCurCol4->Color = TColor(RGB(cv_palette[4*3],cv_palette[4*3+1],cv_palette[4*3+2]));
    pCurCol5->Color = TColor(RGB(cv_palette[5*3],cv_palette[5*3+1],cv_palette[5*3+2]));
    pCurCol6->Color = TColor(RGB(cv_palette[6*3],cv_palette[6*3+1],cv_palette[6*3+2]));
    pCurCol7->Color = TColor(RGB(cv_palette[7*3],cv_palette[7*3+1],cv_palette[7*3+2]));
    pCurCol8->Color = TColor(RGB(cv_palette[8*3],cv_palette[8*3+1],cv_palette[8*3+2]));
    pCurCol9->Color = TColor(RGB(cv_palette[9*3],cv_palette[9*3+1],cv_palette[9*3+2]));
    pCurCol10->Color = TColor(RGB(cv_palette[10*3],cv_palette[10*3+1],cv_palette[10*3+2]));
    pCurCol11->Color = TColor(RGB(cv_palette[11*3],cv_palette[11*3+1],cv_palette[11*3+2]));
    pCurCol12->Color = TColor(RGB(cv_palette[12*3],cv_palette[12*3+1],cv_palette[12*3+2]));
    pCurCol13->Color = TColor(RGB(cv_palette[13*3],cv_palette[13*3+1],cv_palette[13*3+2]));
    pCurCol14->Color = TColor(RGB(cv_palette[14*3],cv_palette[14*3+1],cv_palette[14*3+2]));
    pCurCol15->Color = TColor(RGB(cv_palette[15*3],cv_palette[15*3+1],cv_palette[15*3+2]));
}

void __fastcall Tpaletteviewer::do_refresh()
{
    if (AutoRefresh1->Checked) {
        UpdateChanges();
    }
}

void Tpaletteviewer::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("PALVIEW","Top",Top);
    Left = ini->ReadInteger("PALVIEW","Left",Left);
    AutoRefresh1->Checked = ini->ReadBool("PALVIEW", "AutoRefresh", true);
}

void Tpaletteviewer::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("PALVIEW","Top",Top);
    ini->WriteInteger("PALVIEW","Left",Left);
    ini->WriteBool("PALVIEW", "AutoRefresh", AutoRefresh1->Checked);
}


void __fastcall Tpaletteviewer::Refresh1Click(TObject *Sender)
{
    UpdateChanges();    
}
//---------------------------------------------------------------------------

void __fastcall Tpaletteviewer::AutoRefresh1Click(TObject *Sender)
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

void __fastcall Tpaletteviewer::FormShow(TObject *Sender)
{
    AutoRefresh1->Enabled = true;
    UpdateChanges();
}
//---------------------------------------------------------------------------


