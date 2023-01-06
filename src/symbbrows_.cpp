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
 * symbbrows_.cpp
 *
 * Lots of parts inspired from EightyOne Sinclair Emulator
 *   https://sourceforge.net/projects/eightyone-sinclair-emulator/
 *
 */


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "symbbrows_.h"

#include "symbolstore.h"

#include "debug_.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSymbBrows *SymbBrows;
//---------------------------------------------------------------------------
__fastcall TSymbBrows::TSymbBrows(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSymbBrows::FormShow(TObject *Sender)
{
    RefreshContent();
}
//---------------------------------------------------------------------------

void __fastcall TSymbBrows::RefreshContent(void)
{
    ListBox1->Items->Clear();

    symbolstore::beginenumeration();
    AnsiString sym;
    char type;
    int val;
    while(symbolstore::enumerate(sym, val, type))
    {
        sym = "    " + sym + "                       ";
        sym.SetLength(28);
        sym[1] = type;
        sym += IntToHex(val, 4);
        if ( (val>0x6000) && (val<0x8000) ) sym +="   RAM";
        else sym+="   ROM";
        ListBox1->Items->Add(sym);
    }
}
//---------------------------------------------------------------------------

void __fastcall TSymbBrows::ListBox1DblClick(TObject *Sender)
{
    int addr, item = ListBox1->ItemIndex;
    AnsiString sym(ListBox1->Items->Strings[item].SubString(5,24));
    symbolstore::symbolToAddress(sym.Trim(), addr);
    breakpoint bp(addr, BP_EXE);
    debug->AddBreakPoint(bp);
}
//---------------------------------------------------------------------------
void __fastcall TSymbBrows::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    debug->ShowSymbols1->Caption="Show Symbols";
}
//---------------------------------------------------------------------------


