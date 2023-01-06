/* EmulTwo  - A Windows ColecoVision emulator.
 * Copyright (C) 2014-2023 Alekmaul
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
 * about_.h
 *
 */

//---------------------------------------------------------------------------

#ifndef about_H
#define about_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------

class Tabout : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TButton *Button1;
        TLabel *Label5;
        TLabel *Version;
        TLabel *Label8;
        TLabel *Label10;
        TLabel *Label11;
        TLabel *Label15;
        TLabel *Label4;
        TLabel *Label6;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Label15Click(TObject *Sender);
        void __fastcall Label15MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Label15MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        
private:	// User declarations

public:		// User declarations
        __fastcall Tabout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tabout *about;
//---------------------------------------------------------------------------
#endif
