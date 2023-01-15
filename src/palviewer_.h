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
 * palviewer_.h
 *
*/
//---------------------------------------------------------------------------

#ifndef palviewer_H
#define palviewer_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <Clipbrd.hpp>
#include <IniFiles.hpp>

//---------------------------------------------------------------------------
class Tpaletteviewer : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Exit;
    TMenuItem *View1;
    TMenuItem *AutoRefresh1;
    TMenuItem *N1;
    TMenuItem *Refresh1;
    TGroupBox *gCurCol;
    TLabel *lCurColR;
    TLabel *lCurColG;
    TLabel *lCurColB;
    TLabel *lCurColIdx;
    TPanel *pCurCol;
    TPanel *pCurCol15;
    TPanel *pCurCol14;
    TPanel *pCurCol13;
    TPanel *pCurCol12;
    TLabel *lVVPal4;
    TLabel *lVVPal3;
    TPanel *pCurCol8;
    TPanel *pCurCol9;
    TPanel *pCurCol10;
    TPanel *pCurCol11;
    TPanel *pCurCol7;
    TPanel *pCurCol6;
    TPanel *pCurCol5;
    TPanel *pCurCol4;
    TLabel *lVVPPal2;
    TLabel *lVVPal1;
    TPanel *pCurCol0;
    TPanel *pCurCol1;
    TPanel *pCurCol2;
    TPanel *pCurCol3;
    TLabel *lVVPBackPal;
    TLabel *lVVPF18APal;
    TLabel *Label1;
    TPanel *Panel1;
    TPanel *Panel2;
    TPanel *Panel3;
    TPanel *Panel4;
    TPanel *Panel5;
    TPanel *Panel6;
    TPanel *Panel7;
    TPanel *Panel8;
    TPanel *Panel9;
    TPanel *Panel10;
    TPanel *Panel11;
    TPanel *Panel12;
    TPanel *Panel13;
    TPanel *Panel14;
    TPanel *Panel15;
    TPanel *Panel16;
    TLabel *Label2;
    TPanel *Panel17;
    TPanel *Panel18;
    TPanel *Panel19;
    TPanel *Panel20;
    TPanel *Panel21;
    TPanel *Panel22;
    TPanel *Panel23;
    TPanel *Panel24;
    TPanel *Panel25;
    TPanel *Panel26;
    TPanel *Panel27;
    TPanel *Panel28;
    TPanel *Panel29;
    TPanel *Panel30;
    TPanel *Panel31;
    TPanel *Panel32;
    TLabel *Label3;
    TPanel *Panel33;
    TPanel *Panel34;
    TPanel *Panel35;
    TPanel *Panel36;
    TPanel *Panel37;
    TPanel *Panel38;
    TPanel *Panel39;
    TPanel *Panel40;
    TPanel *Panel41;
    TPanel *Panel42;
    TPanel *Panel43;
    TPanel *Panel44;
    TPanel *Panel45;
    TPanel *Panel46;
    TPanel *Panel47;
    TPanel *Panel48;
    TLabel *Label4;
    TPanel *Panel49;
    TPanel *Panel50;
    TPanel *Panel51;
    TPanel *Panel52;
    TPanel *Panel53;
    TPanel *Panel54;
    TPanel *Panel55;
    TPanel *Panel56;
    TPanel *Panel57;
    TPanel *Panel58;
    TPanel *Panel59;
    TPanel *Panel60;
    TPanel *Panel61;
    TPanel *Panel62;
    TPanel *Panel63;
    TPanel *Panel64;
    TStaticText *eCurColR;
    TStaticText *eCurColG;
    TStaticText *eCurColB;
    TStaticText *eCurColIdx;
    void __fastcall ExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall pCurCol0MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall Refresh1Click(TObject *Sender);
    void __fastcall AutoRefresh1Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    void __fastcall ChangeCurrentColor(int ColorNum);
    void __fastcall UpdateChanges();
public:		// User declarations
    __fastcall Tpaletteviewer(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);
    void __fastcall do_refresh();
};
//---------------------------------------------------------------------------
extern PACKAGE Tpaletteviewer *paletteviewer;
//---------------------------------------------------------------------------
#endif
