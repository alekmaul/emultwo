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
 * printviewer_.cpp
 *
 */

#include <vcl.h>
#pragma hdrstop

#include "printviewer_.h"

#include "colecoconfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tprintviewer *printviewer;
//---------------------------------------------------------------------------
__fastcall Tprintviewer::Tprintviewer(TComponent* Owner)
    : TForm(Owner)
{
        TIniFile *ini;

        ini = new TIniFile(emul2.inipath);
        LoadSettings(ini);
        delete ini;
}

//---------------------------------------------------------------------------

void Tprintviewer::LoadSettings(TIniFile *ini)
{
        Top = ini->ReadInteger("PRNVIEW","Top",Top);
        Left = ini->ReadInteger("PRNVIEW","Left",Left);
}

void Tprintviewer::SaveSettings(TIniFile *ini)
{
        ini->WriteInteger("PRNVIEW","Top",Top);
        ini->WriteInteger("PRNVIEW","Left",Left);
}

//---------------------------------------------------------------------------
void Tprintviewer::SendPrint(byte Data)
{
    if (Data>=0x20) // acsii SPACE minimum
    {
        printdump->Text = printdump->Text+Data;
    }
    else
    {
        printdump->Text = printdump->Text + " $"+IntToHex(Data,2);
    }
}
//---------------------------------------------------------------------------

void __fastcall Tprintviewer::clearprintClick(TObject *Sender)
{
        printdump->Clear();
}
//---------------------------------------------------------------------------

void __fastcall Tprintviewer::Copytoclipboard1Click(TObject *Sender)
{
        printdump->SelectAll();
        printdump->CopyToClipboard();
}
//---------------------------------------------------------------------------

