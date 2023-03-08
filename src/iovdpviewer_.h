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
 * iovdpviewer_.h
 *
 */

#ifndef iovdpviewer_H
#define iovdpviewer_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class Tiovdpviewer : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Exit;
    TMenuItem *View1;
    TMenuItem *AutoRefresh1;
    TMenuItem *N1;
    TMenuItem *Refresh1;
    TGroupBox *gVDP;
    TLabel *lVDPR0;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *lVDPStat;
    TLabel *lVDPlatch;
    TStaticText *eVDPlatch;
    TStaticText *eVDPR0;
    TStaticText *eVDPStat;
    TStaticText *eVDPR7;
    TStaticText *eVDPR1;
    TStaticText *eVDPR2;
    TStaticText *eVDPR3;
    TStaticText *eVDPR4;
    TStaticText *eVDPR5;
    TStaticText *eVDPR6;
    TGroupBox *gF18A;
    TLabel *lblReg8;
    TStaticText *StaticText1;
    TLabel *lblReg9;
    TStaticText *StaticText2;
    TLabel *Label10;
    TStaticText *StaticText3;
    TLabel *Label11;
    TStaticText *StaticText4;
    TLabel *Label12;
    TStaticText *StaticText5;
    TLabel *Label13;
    TStaticText *StaticText6;
    TLabel *Label14;
    TStaticText *StaticText7;
    TStaticText *StaticText8;
    TStaticText *StaticText9;
    TLabel *Label1;
    TStaticText *StaticText10;
    TLabel *Label9;
    TStaticText *StaticText11;
    TLabel *Label17;
    TStaticText *StaticText12;
    TStaticText *StaticText13;
    TLabel *Label18;
    TLabel *Label19;
    TStaticText *StaticText14;
    TLabel *Label20;
    TStaticText *StaticText15;
    TLabel *Label21;
    TStaticText *StaticText16;
    TLabel *Label22;
    TStaticText *StaticText17;
    TLabel *Label23;
    TStaticText *StaticText18;
    TLabel *Label24;
    TStaticText *StaticText19;
    TLabel *Label25;
    TStaticText *StaticText20;
    TLabel *Label26;
    TStaticText *StaticText21;
    TLabel *Label27;
    TStaticText *StaticText22;
    TLabel *Label28;
    TStaticText *StaticText23;
    TStaticText *StaticText24;
    TLabel *Label29;
    TLabel *Label30;
    TStaticText *StaticText25;
    TLabel *Label31;
    TStaticText *StaticText26;
    TGroupBox *GroupBox2;
    TStaticText *eVDPMode;
    TStaticText *eVDPOption;
    TStaticText *eVDPBGmap;
    TStaticText *eVDPBGcolor;
    TStaticText *eVDPBGtile;
    TStaticText *eVDPSPRdata;
    TStaticText *eVDPSPRtile;
    TStaticText *eVDPColF;
    TStaticText *eVDPColB;
    TLabel *lVDPMode;
    TStaticText *eVDPValMod;
    TStaticText *eVDPStatus;
    TLabel *Label32;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *Label36;
    TLabel *Label37;
    TLabel *Label38;
    TLabel *Label39;
    TLabel *Label40;
    TLabel *Label41;
    TStaticText *StaticText27;
    TStaticText *StaticText28;
    TStaticText *StaticText29;
    TStaticText *StaticText30;
    TStaticText *StaticText31;
    TStaticText *StaticText32;
    TStaticText *StaticText33;
    TStaticText *StaticText34;
    TStaticText *eVDPStat1;
    TStaticText *eVDPlatch1;
    TLabel *Label52;
    TBevel *Bevel1;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label42;
    TLabel *Label43;
    TLabel *Label44;
    TLabel *Label45;
    TLabel *Label46;
    TLabel *Label47;
    TLabel *Label48;
    TLabel *Label49;
    TStaticText *eF18ABGCol2;
    TLabel *Label51;
    TStaticText *eVDPBGmap2;
    TLabel *Label53;
    TBevel *Bevel2;
    TGroupBox *GroupBox1;
    TLabel *Label57;
    TStaticText *eF18ACntSt;
    TLabel *Start;
    TStaticText *eF18ACntSn;
    TLabel *Snap;
    TStaticText *eF18ACntEl;
    TLabel *Elapsed;
    TLabel *Label58;
    TStaticText *ePaSeTL1;
    TLabel *Label59;
    TStaticText *ePaSeTL2;
    TStaticText *ePaSeSP;
    TLabel *Label60;
    TLabel *Label61;
    TLabel *Label54;
    TStaticText *eF18ATCMo;
    TStaticText *eF18ASCMo;
    TLabel *Label56;
    TLabel *Label55;
    TCheckBox *chkT1On;
    TCheckBox *chkT2On;
    TCheckBox *chkBMOn;
    TStaticText *eF18ASelNo;
    TLabel *Label50;
    TStaticText *eF18ALock;
    TCheckBox *chkRow30;
    void __fastcall ExitClick(TObject *Sender);
    void __fastcall AutoRefresh1AdvancedDrawItem(TObject *Sender,
          TCanvas *ACanvas, TRect &ARect, TOwnerDrawState State);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall Tiovdpviewer(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);

    void __fastcall UpdateChanges();
    void __fastcall do_refresh();
};

//---------------------------------------------------------------------------
extern PACKAGE Tiovdpviewer *iovdpviewer;
//---------------------------------------------------------------------------
#endif
