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
    TStaticText *eCurColR;
    TStaticText *eCurColG;
    TStaticText *eCurColB;
    TStaticText *eCurColIdx;
    TPaintBox *paCurCol;
    TGroupBox *gTMS;
    TLabel *lVVPal4;
    TLabel *lVVPal3;
    TLabel *lVVPPal2;
    TLabel *lVVPal1;
    TPaintBox *paTCol1;
    TPaintBox *paTCol2;
    TPaintBox *paTCol3;
    TPaintBox *paTCol4;
    TPaintBox *paTCol5;
    TPaintBox *paTCol6;
    TPaintBox *paTCol7;
    TPaintBox *paTCol8;
    TPaintBox *paTCol9;
    TPaintBox *paTCol10;
    TPaintBox *paTCol11;
    TPaintBox *paTCol12;
    TPaintBox *paTCol13;
    TPaintBox *paTCol14;
    TPaintBox *paTCol15;
    TPaintBox *paTCol16;
    TGroupBox *gF18A;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TPaintBox *paFCol1;
    TPaintBox *paFCol2;
    TPaintBox *paFCol3;
    TPaintBox *paFCol4;
    TPaintBox *paFCol5;
    TPaintBox *paFCol6;
    TPaintBox *paFCol7;
    TPaintBox *paFCol8;
    TPaintBox *paFCol9;
    TPaintBox *paFCol10;
    TPaintBox *paFCol11;
    TPaintBox *paFCol12;
    TPaintBox *paFCol13;
    TPaintBox *paFCol14;
    TPaintBox *paFCol15;
    TPaintBox *paFCol16;
    TPaintBox *paFCol32;
    TPaintBox *paFCol31;
    TPaintBox *paFCol30;
    TPaintBox *paFCol29;
    TPaintBox *paFCol28;
    TPaintBox *paFCol27;
    TPaintBox *paFCol26;
    TPaintBox *paFCol25;
    TPaintBox *paFCol24;
    TPaintBox *paFCol23;
    TPaintBox *paFCol22;
    TPaintBox *paFCol21;
    TPaintBox *paFCol20;
    TPaintBox *paFCol19;
    TPaintBox *paFCol18;
    TPaintBox *paFCol17;
    TPaintBox *paFCol48;
    TPaintBox *paFCol47;
    TPaintBox *paFCol46;
    TPaintBox *paFCol45;
    TPaintBox *paFCol44;
    TPaintBox *paFCol43;
    TPaintBox *paFCol42;
    TPaintBox *paFCol41;
    TPaintBox *paFCol40;
    TPaintBox *paFCol39;
    TPaintBox *paFCol38;
    TPaintBox *paFCol37;
    TPaintBox *paFCol36;
    TPaintBox *paFCol35;
    TPaintBox *paFCol34;
    TPaintBox *paFCol33;
    TPaintBox *paFCol64;
    TPaintBox *paFCol63;
    TPaintBox *paFCol62;
    TPaintBox *paFCol61;
    TPaintBox *paFCol60;
    TPaintBox *paFCol59;
    TPaintBox *paFCol58;
    TPaintBox *paFCol57;
    TPaintBox *paFCol56;
    TPaintBox *paFCol55;
    TPaintBox *paFCol54;
    TPaintBox *paFCol53;
    TPaintBox *paFCol52;
    TPaintBox *paFCol51;
    TPaintBox *paFCol50;
    TPaintBox *paFCol49;
    void __fastcall ExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall pCurCol0MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall Refresh1Click(TObject *Sender);
    void __fastcall AutoRefresh1Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormPaint(TObject *Sender);
private:	// User declarations
    void __fastcall ChangeCurrentColor(int ColorNum);
    void __fastcall UpdateChanges();
    void __fastcall SetTColor(TPaintBox *Sender, int r, int g, int b);
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
