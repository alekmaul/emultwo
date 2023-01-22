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
 * joyconf_.cpp
 *
 */


 #ifndef joyconf_H
#define joyconf_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <IniFiles.hpp>
#include <Graphics.hpp>

//---------------------------------------------------------------------------
class Tjoyconf : public TForm
{
__published:	// Composants gérés par l'EDI
    TButton *bOK;
    TButton *bCancel;
    TPageControl *PageControl1;
    TTabSheet *TabSheet1;
    TTabSheet *TabSheet2;
    TImage *colecojoy;
    TComboBox *cboTypeJoy1;
    TEdit *eTUP;
    TLabel *Label1;
    TComboBox *cboTypeJoy2;
    TLabel *Label3;
    TLabel *Label2;
    TEdit *eTDOWN;
    TLabel *Label4;
    TEdit *eTLEFT;
    TLabel *Label5;
    TEdit *eTRIGHT;
    TLabel *Label6;
    TEdit *eTTR;
    TLabel *Label7;
    TEdit *eTTL;
    TLabel *Label8;
    TEdit *eTN0;
    TLabel *Label9;
    TEdit *eTN1;
    TLabel *Label10;
    TEdit *eTN2;
    TLabel *Label11;
    TEdit *eTN3;
    TLabel *Label12;
    TEdit *eTN4;
    TLabel *Label13;
    TEdit *eTN5;
    TLabel *Label14;
    TEdit *eTN6;
    TLabel *Label15;
    TEdit *eTN7;
    TLabel *Label16;
    TEdit *eTN8;
    TLabel *Label17;
    TEdit *eTN9;
    TLabel *Label18;
    TLabel *Label19;
    TEdit *eTDI;
    TEdit *eTMU;
    TLabel *Label20;
    TEdit *eTN02;
    TLabel *Label21;
    TEdit *eTN12;
    TLabel *Label22;
    TEdit *eTN22;
    TLabel *Label23;
    TEdit *eTN32;
    TLabel *Label24;
    TEdit *eTN42;
    TLabel *Label25;
    TEdit *eTN52;
    TLabel *Label26;
    TEdit *eTN62;
    TLabel *Label27;
    TEdit *eTN72;
    TLabel *Label28;
    TEdit *eTN82;
    TLabel *Label29;
    TEdit *eTN92;
    TImage *colecojoy2;
    TLabel *Label30;
    TEdit *eTUP2;
    TLabel *Label31;
    TEdit *eTDOWN2;
    TLabel *Label32;
    TEdit *eTLEFT2;
    TLabel *Label33;
    TEdit *eTRIGHT2;
    TLabel *Label34;
    TEdit *eTTR2;
    TLabel *Label35;
    TEdit *eTTL2;
    TLabel *Label36;
    TEdit *eTDI2;
    TLabel *Label37;
    TEdit *eTMU2;
    void __fastcall bOKClick(TObject *Sender);
    void __fastcall eTUPKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall eTUP2KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// Déclarations de l'utilisateur
public:		// Déclarations de l'utilisateur
    __fastcall Tjoyconf(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);
};
//---------------------------------------------------------------------------
extern PACKAGE Tjoyconf *joyconf;
//---------------------------------------------------------------------------
#endif
