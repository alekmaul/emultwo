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
 * main_.h
 *
 */

//---------------------------------------------------------------------------

#ifndef main_H
#define main_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
//#include "AnimTimer.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <IniFiles.hpp>
#include <Dialogs.hpp>

#include "kbstatus_.h"

#define RENDERGDI 0
#define RENDERDDRAW 1

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TTimer *Timer2;
        TStatusBar *StatusBar1;
        TOpenDialog *OpenDialog;
        TMainMenu *MainMenu;
        TMenuItem *File1;
        TMenuItem *Open1;
        TMenuItem *mRU1;
        TMenuItem *mRU2;
        TMenuItem *mRU3;
        TMenuItem *mRU5;
        TMenuItem *N0;
        TMenuItem *SaveState1;
        TMenuItem *LoadState1;
        TMenuItem *Screenshot1;
        TMenuItem *N9;
        TMenuItem *MenuItem3;
        TMenuItem *Debug1;
        TMenuItem *Run1;
        TMenuItem *N11;
        TMenuItem *SoftReset1;
        TMenuItem *HardReset1;
        TMenuItem *N12;
        TMenuItem *Debugger1;
        TMenuItem *Tools1;
        TMenuItem *JoystickEditor1;
        TMenuItem *N6;
    TMenuItem *SoundViewer1;
        TMenuItem *N14;
        TMenuItem *TilemapViewer1;
        TMenuItem *TilesViewer1;
        TMenuItem *SpritesViewer1;
        TMenuItem *PaletteViewer1;
        TMenuItem *N16;
        TMenuItem *PrinterViewer1;
        TMenuItem *N15;
        TMenuItem *IOMapViewer1;
        TMenuItem *N13;
        TMenuItem *Cartprofile1;
        TMenuItem *Options1;
        TMenuItem *VideoSize1;
        TMenuItem *OVS1x;
        TMenuItem *OVS2x;
        TMenuItem *OVS3x;
        TMenuItem *N3;
        TMenuItem *Emulation1;
        TMenuItem *N5;
        TMenuItem *GatherWindows1;
        TMenuItem *MenuItem4;
        TMenuItem *Content1;
        TMenuItem *N8;
        TMenuItem *Donate1;
        TMenuItem *N7;
        TMenuItem *MenuItem5;
        TSaveDialog *SaveDialog;
        TMenuItem *OpenDA;
        TMenuItem *OpenDB;
        TMenuItem *DDAInsert;
        TMenuItem *DDAEject;
        TMenuItem *OpenRecent1;
        TMenuItem *mRU4;
        TMenuItem *OpenTA;
        TMenuItem *TDAInsert;
        TMenuItem *TDAEject;
        TMenuItem *DDBInsert;
        TMenuItem *DDBEject;
        TMenuItem *Reportabug1;
        TMenuItem *Chatwithcommunity1;
        TMenuItem *N1;
        void __fastcall GatherWindows1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall About1Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall OVS1xClick(TObject *Sender);
        void __fastcall OVS2xClick(TObject *Sender);
        void __fastcall OVS3xClick(TObject *Sender);
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall Emulation1Click(TObject *Sender);
        void __fastcall Donate1Click(TObject *Sender);
//        void __fastcall AnimTimer1Timer(TObject *Sender);
        void __fastcall SoftReset1Click(TObject *Sender);
        void __fastcall HardReset1Click(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall mRU1Click(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Screenshot1Click(TObject *Sender);
        void __fastcall TilesViewer1Click(TObject *Sender);
        void __fastcall Cartprofile1Click(TObject *Sender);
        void __fastcall TilemapViewer1Click(TObject *Sender);
        void __fastcall SpritesViewer1Click(TObject *Sender);
        void __fastcall Debugger1Click(TObject *Sender);
        void __fastcall Run1Click(TObject *Sender);
        void __fastcall DDAInsertClick(TObject *Sender);
        void __fastcall PrinterViewer1Click(TObject *Sender);
        void __fastcall DDAEjectClick(TObject *Sender);
        void __fastcall Reportabug1Click(TObject *Sender);
        void __fastcall Chatwithcommunity1Click(TObject *Sender);
        void __fastcall LoadState1Click(TObject *Sender);
        void __fastcall SaveState1Click(TObject *Sender);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall PaletteViewer1Click(TObject *Sender);
    void __fastcall Content1Click(TObject *Sender);
    void __fastcall JoystickEditor1Click(TObject *Sender);
    void __fastcall IOMapViewer1Click(TObject *Sender);
    void __fastcall SoundViewer1Click(TObject *Sender);
private:	// User declarations
        int fps;
        int StartUpWidth, StartUpHeight;
        TStringList* MruList;
        void __fastcall LoadProgram(AnsiString FileName);
        void __fastcall LoadDiskTape(int TypeMedia, int DiskTapeNum, AnsiString FileName);
        void UpdateStatusBar(void);

        void __fastcall OnJoyMove(TMessage &msg);
        void __fastcall OnJoyDown(TMessage &msg);
        void __fastcall OnJoyUp(TMessage &msg);

public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        virtual void __fastcall WndProc(TMessage &Msg);

        int BaseHeight;
        int BaseWidth;
        int RenderMode;
        void __fastcall AppMessage(TMsg &Msg, bool &Handled);
        void UpdateMruMenu(void);
        void LoadSettings(TIniFile *ini);
        void SaveSettings(TIniFile *ini);
        void GatherWindowsIfRequired();
        void __fastcall RunFrame();
        int RunFrameEnable;

        BEGIN_MESSAGE_MAP
                MESSAGE_HANDLER(MM_JOY1MOVE,TMessage,OnJoyMove);
                MESSAGE_HANDLER(MM_JOY1BUTTONDOWN,TMessage,OnJoyDown);
                MESSAGE_HANDLER(MM_JOY1BUTTONUP,TMessage,OnJoyUp);
        END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
