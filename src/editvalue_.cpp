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
 * editvalue_.cpp
 *
 * Lots of parts inspired from EightyOne Sinclair Emulator
 *   https://sourceforge.net/projects/eightyone-sinclair-emulator/
 *
 */


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "editvalue_.h"
#include "symbolstore.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditValue *EditValue;
//---------------------------------------------------------------------------
__fastcall TEditValue::TEditValue(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEditValue::CancelClick(TObject *Sender)
{
    cancelled=true;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TEditValue::OKClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void TEditValue::CentreOn(TForm* parent)
{
    Top = parent->Top + (parent->Height - Height) /2;
    Left = parent->Left + (parent->Width - Width) /2;
}

// returns true or false depending on whether the gathering of a value was
// succesful. The data is both passed in and returned in value. This is
// unchanged in the case when gathering fails.
//
bool TEditValue::Edit2(int& value, int len)
{
        AnsiString NewVal;

        maxValue = (1 << (8*len)) - 1;

        cancelled=false;
        EditVal->Text = IntToHex(value, len*2);

        ActiveControl = EditVal;
        EditVal->SelectAll();
        ShowModal();

        return Translate(EditVal->Text, value);
}

bool TEditValue::Translate(AnsiString text, int& value)
{
    AnsiString result = text.Trim();

    if (cancelled || result.Length() == 0)
    {
        return false;
    }

    int addr;
    if (symbolstore::symbolToAddress(result, addr))
    {
        result = IntToHex(addr, 4);
    }

    char* endPtr;
    int tval;
    tval = int(strtol(result.c_str(), &endPtr, 16));

    if (*endPtr != 0)
    {
        // and we know when the user entered junk...
        return false;
    }

    value = tval;

    return (value <= maxValue);
}

//---------------------------------------------------------------------------
void __fastcall TEditValue::FormKeyPress(TObject *Sender, char &Key)
{
    if (Key == VK_ESCAPE)
    {
        cancelled = true;
        Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TEditValue::EditValChange(TObject *Sender)
{
    int value;
    bool valid = Translate(EditVal->Text, value);
    EditVal->Font->Color = valid ? clWindowText : clRed;

    OK->Enabled = valid;
}
//---------------------------------------------------------------------------
