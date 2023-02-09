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
#include "f18a.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tpaletteviewer *paletteviewer;
//---------------------------------------------------------------------------

__fastcall Tpaletteviewer::Tpaletteviewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    ini = new TIniFile(emul2.inipath);
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
    SetTColor(paCurCol,cv_palette[ColorNum*3],cv_palette[ColorNum*3+1],cv_palette[ColorNum*3+2]);
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

void __fastcall Tpaletteviewer::SetTColor(TPaintBox *Sender, int r, int g, int b)
{
    Sender->Canvas->Brush->Color=TColor(RGB(r,g,b));
    Sender->Canvas->Brush->Style=bsSolid;
    Sender->Canvas->FillRect(TRect(0,0,Sender->Width,Sender->Height));
}
//---------------------------------------------------------------------------

void __fastcall Tpaletteviewer::UpdateChanges()
{
    int index;

    //example grab from https://stackoverflow.com/questions/2391325/bcb-how-to-iterate-over-controls-on-a-form

    // TMS9918 Colors
    for (int index = 0; index < gTMS->ControlCount; index ++)
    {
        if(gF18A->Controls[index]->InheritsFrom(__classid(TPaintBox)))
        {
            TPaintBox *pbox = (TPaintBox*) gTMS->Controls[index];
            SetTColor(pbox,cv_palette[pbox->Tag*3+0],cv_palette[pbox->Tag*3+1],cv_palette[pbox->Tag*3+2]);
        }
    }

    // F18A Colors if enable
    if (emul2.F18A)
    {
        gF18A->Enabled=true;
        for (index = 0; index < gF18A->ControlCount; index ++)
        {
            if(gF18A->Controls[index]->InheritsFrom(__classid(TPaintBox)))
            {
                TPaintBox *pbox = (TPaintBox*) gF18A->Controls[index];
                SetTColor(pbox,cv_palette[pbox->Tag*3+0],cv_palette[pbox->Tag*3+1],cv_palette[pbox->Tag*3+2]);
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall Tpaletteviewer::do_refresh()
{
    if (AutoRefresh1->Checked) {
        UpdateChanges();
    }
}
//---------------------------------------------------------------------------

void Tpaletteviewer::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("PALVIEW","Top",Top);
    Left = ini->ReadInteger("PALVIEW","Left",Left);
    AutoRefresh1->Checked = ini->ReadBool("PALVIEW", "AutoRefresh", true);
}
//---------------------------------------------------------------------------

void Tpaletteviewer::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("PALVIEW","Top",Top);
    ini->WriteInteger("PALVIEW","Left",Left);
    ini->WriteBool("PALVIEW", "AutoRefresh", AutoRefresh1->Checked);
}
//---------------------------------------------------------------------------

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


void __fastcall Tpaletteviewer::FormPaint(TObject *Sender)
{
    UpdateChanges();
}
//---------------------------------------------------------------------------

