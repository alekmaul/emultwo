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
 * nametabviewer_.h
 *
 */

#ifndef nametabviewer_H
#define nametabviewer_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <IniFiles.hpp>
#include <Clipbrd.hpp>

//---------------------------------------------------------------------------
class Tnametabviewer : public TForm
{
__published:	// Composants gérés par l'EDI
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Copytoclipboard1;
    TMenuItem *SaveasBMP1;
    TMenuItem *N2;
    TMenuItem *Exit;
    TMenuItem *View1;
    TMenuItem *AutoRefresh1;
    TMenuItem *N1;
    TMenuItem *Refresh1;
    TSaveDialog *dlgSVPic;
    TGroupBox *gCHRTile;
        TStaticText *eGVVCloc;
    TStaticText *eGVVCTNo;
    TStaticText *eGVVCMAddr;
    TStaticText *eGVVCCAddr;
    TStaticText *eGVVCTAddr;
    TLabel *lGVVCXloc;
    TLabel *lGVVCTNo;
    TLabel *lGVVCMAddr;
    TLabel *lGVVCTAddr;
    TLabel *lGVVCCAddr;
    TStaticText *idTiValue;
        TPaintBox *TileAlone;
        TCheckBox *chkGrid;
        TCheckBox *chkTiles;
        TPaintBox *VRam;
    void __fastcall SaveasBMP1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ExitClick(TObject *Sender);
    
    void __fastcall AutoRefresh1Click(TObject *Sender);
    void __fastcall VRamMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Copytoclipboard1Click(TObject *Sender);
        void __fastcall chkGridClick(TObject *Sender);
        void __fastcall chkTilesClick(TObject *Sender);
private:	// Déclarations de l'utilisateur
    Graphics::TBitmap *mOffscreenBitmap;
    Graphics::TBitmap *mOfftileBitmap;

    int mVramTile;
    void CreateMap(TCanvas *Acanvas, int w, int h);
    void CreateTile(void);
public:		// Déclarations de l'utilisateur
    __fastcall Tnametabviewer(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);

    void __fastcall SmallUpdateChanges();
    void __fastcall UpdateChanges();

    void __fastcall do_refresh();
};
//---------------------------------------------------------------------------
extern PACKAGE Tnametabviewer *nametabviewer;
//---------------------------------------------------------------------------
#endif
