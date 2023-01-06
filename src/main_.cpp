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
 * main_.cpp
 *
 */


//---------------------------------------------------------------------------

#include <vcl.h>

#pragma hdrstop

#include "main_.h"

#include "coleco.h"
#include "colecoconfig.h"
#include "utils.h"
#include "accdraw_.h"
#include "accsound_.h"

#include "about_.h"
#include "cartprofile_.h"
#include "debug_.h"
#include "hardware_.h"
#include "kbstatus_.h"

#include "nametabviewer_.h"
#include "patternviewer_.h"
#include "spriteviewer_.h"
#include "printviewer_.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------
#define MRFUCOUNT 5

extern char **CommandLine;
static bool iniFileExists = false;

bool nosound;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        AnsiString IniPath;
        char path[256];
        int i;

        strcpy(coleco.cwd, (FileNameGetPath(Application->ExeName)).c_str());
        if (coleco.cwd[strlen(coleco.cwd)-1]!='\\')
        {
                coleco.cwd[strlen(coleco.cwd)-1]='\\';
                coleco.cwd[strlen(coleco.cwd)]='\0';
        }

        IniPath=ChangeFileExt(Application->ExeName, ".ini" );
        strcpy(coleco.inipath, IniPath.c_str());
        IniPath=FileNameGetPath(Application->ExeName);
        strcpy(coleco.configpath, IniPath.c_str());
        strcpy(coleco.mydocs, coleco.cwd);

        for(i=0; CommandLine[i]!=NULL; i++)
        {
                if (FileNameGetExt(CommandLine[i]) == ".INI")
                {
                        IniPath=CommandLine[i];
                        if (IniPath.Pos("\\")==0)
                                IniPath=coleco.configpath + IniPath;
                        strcpy(coleco.inipath, IniPath.c_str());
                }
        }

        MruList = new TStringList;

        RenderMode=RENDERGDI;
//    Application->OnDeactivate=FormDeactivate;
//        BuildLine=&Video[0];
//        DisplayLine=&Video[1];

}
//---------------------------------------------------------------------------

void TForm1::GatherWindowsIfRequired()
{
        if (!iniFileExists)
        {
                Form1->GatherWindows1Click(this);
        }
}

//---------------------------------------------------------------------------

void MoveWindow(TForm* form, int& x, int& y)
{
    form->Left = x;
    form->Top = y;
    form->BringToFront();

    if (form->Visible)
    {
        x += 20;
        y += 20;
    }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::GatherWindows1Click(TObject *Sender)
{
        int l = 0;
        int t = 0;
        MoveWindow(this, l, t);
        l = (this->Width / 2);
        t = (this->Height / 2);
        MoveWindow(hardware, l, t);
/*
        MoveWindow(Dbg, l, t);
        MoveWindow(MemoryWindow, l, t);
        MoveWindow(SymbolBrowser, l, t);
        MoveWindow(SoundOutput, l, t);
        MoveWindow(TZX, l, t);
        MoveWindow(WavLoad, l, t);
        MoveWindow(Speed, l, t);
        MoveWindow(Artifacts, l, t);
        MoveWindow(Keyboard, l, t);
        Keyboard->KbChange();
        MoveWindow(FSSettings, l, t);
        MoveWindow(Printer, l, t);
        MoveWindow(MidiForm, l, t);
        MoveWindow(MemSave, l, t);
        MoveWindow(SerialConfig, l, t);
        MoveWindow(IF1, l, t);
        MoveWindow(HistoryBox, l, t);
        MoveWindow(ZX97Dialog, l, t);
        MoveWindow(ParallelPort, l, t);
        MoveWindow(Kb, l, t);
        MoveWindow(ZipFile, l, t);
        MoveWindow(P3Drive, l, t);
        MoveWindow(CreateHDF, l, t);
        MoveWindow(Debug68k, l, t);
        MoveWindow(LiveMemoryWindow, l, t);
        MoveWindow(ProfilePlot, l, t);
        MoveWindow(Profiler, l, t);
        MoveWindow(BasicLister, l, t);     */
}

//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
        DragAcceptFiles(Handle, true);

        StatusBar1->Refresh();
        StatusBar1->Invalidate();
}

//---------------------------------------------------------------------------


void __fastcall TForm1::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
        switch(Button)
        {
        case mbLeft: mouse.buttons |= 2; break;
        case mbRight: mouse.buttons |= 1; break;
        case mbMiddle: mouse.buttons |= 4; break;
        }        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
        switch(Button)
        {
        case mbLeft: mouse.buttons &= ~2; break;
        case mbRight: mouse.buttons &= ~1; break;
        case mbMiddle: mouse.buttons &= ~4; break;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Exit1Click(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::About1Click(TObject *Sender)
{
        about->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Donate1Click(TObject *Sender)
{
        ShellExecute(NULL, "open", "https://www.paypal.com/donate/?cmd=_s-xclick&hosted_button_id=Y5USKF23DQVLC", "", NULL, SW_RESTORE);
}

//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        TIniFile *ini;

        // stop properly the current machine
        if (machine.exit) machine.exit();
        coleco.stop=1;
        AnimTimer1->Enabled=false;

        //if (!nosound) sound_end();

        ini = new TIniFile(coleco.inipath);
        SaveSettings(ini);
        delete ini;

        RenderEnd();

        SoundEnd();

        delete MruList;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
        TIniFile *ini;
        char soundfile[256];

        AnimTimer1->Enabled=false;

        nosound = true;

        Application->OnMessage = AppMessage;

        load_config();

        ini = new TIniFile(coleco.inipath);
        if (!SoundInit())
        {
                Application->MessageBox("Count not initialise DirectSound.\nPlease ensure DirectX 5 or greater is installed.",
				"Error",
	                        MB_OK | MB_ICONERROR);
                SoundEnd();
        }
        nosound = false;

        RenderMode=ini->ReadInteger("MAIN","RenderMode", RENDERGDI);

        if (!RenderInit())
        {
                RenderEnd();
                exit(0);
        }

        iniFileExists = FileExists(coleco.inipath);
        LoadSettings(ini);
        delete ini;

        AnimTimer1->Interval=20;
        Timer2->Interval=1000;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AppMessage(TMsg &Msg, bool &Handled)
{
        WORD BufferLength=255;
        AnsiString Filename, Ext;
        WORD FileIndex;
        WORD QtyDroppedFiles;
        char pDroppedFilename[255];

        if (Msg.message == WM_DROPFILES)
        {
                QtyDroppedFiles = DragQueryFile((void *)Msg.wParam, -1,
                                                pDroppedFilename, BufferLength);

                for(FileIndex=0; FileIndex<=(QtyDroppedFiles - 1); FileIndex++)
                {
                        DragQueryFile((void *)Msg.wParam, FileIndex, pDroppedFilename, BufferLength);

                        Filename = pDroppedFilename;
                        Ext = GetExt(Filename);
                        if (Ext==".COL" || Ext==".BIN" || Ext==".ROM")
                        {
                                LoadProgram(Filename);
                        }
                }

                DragFinish((void *)Msg.wParam);
                Handled = true;
        }
}
//---------------------------------------------------------------------------

void TForm1::UpdateMruMenu(void)
{
    // This is the OnClick handler for the top level File menu. Here we check each string
    // in the MRU list and show the associated MRU menu item if the string is not empty.
    for (int i=0;i< MRFUCOUNT; i++) {
        if (strlen(MruList->Strings[i].c_str()) != 0) {
            OpenRecent1->Enabled = true;
            int j=mRU1->MenuIndex + i;
            String buff=(i + 1)+": "+MruList->Strings[i];
            OpenRecent1->Items[j]->Caption = MruList->Strings[i];
            if (i==0)
                OpenRecent1->Items[j]->ShortCut=ShortCut(Word('R'), TShiftState() << ssCtrl);
            OpenRecent1->Items[j]->Visible = true;
        }
    }
}
//---------------------------------------------------------------------------

void TForm1::LoadSettings(TIniFile *ini)
{
        KeysRead(ini);

        AccurateInit(true);

        if (ini->ReadBool("MAIN","OVS1x",OVS1x->Checked)) OVS1xClick(NULL);
        if (ini->ReadBool("MAIN","OVS2x",OVS2x->Checked)) OVS2xClick(NULL);
        if (ini->ReadBool("MAIN","OVS3x",OVS3x->Checked)) OVS3xClick(NULL);

        Top = ini->ReadInteger("MAIN","Top",0);
        Left = ini->ReadInteger("MAIN","Left",0);

        // The start up height and width are transferred to the real height and width on the first timer event.
        // The height and width are also restored to these values after changing the configuration.
        StartUpHeight = Height;
        StartUpWidth = Width;

        // Most recent files management
        MruList->Add(ini->ReadString("RecentFiles","MRU1",""));
        MruList->Add(ini->ReadString("RecentFiles","MRU2",""));
        MruList->Add(ini->ReadString("RecentFiles","MRU3",""));
        MruList->Add(ini->ReadString("RecentFiles","MRU4",""));
        MruList->Add(ini->ReadString("RecentFiles","MRU5",""));
        UpdateMruMenu();

        // Always default to the 100% to begin with, before changing to real dimensions upon the next timer event
        ClientHeight = BaseHeight + StatusBar1->Height;
        ClientWidth = BaseWidth;
}

void TForm1::SaveSettings(TIniFile *ini)
{
        ini->WriteInteger("MAIN","Top",Top);
        ini->WriteInteger("MAIN","Left",Left);
        ini->WriteInteger("MAIN","Height",Height);
        ini->WriteInteger("MAIN","Width",Width);

        ini->WriteInteger("MAIN","RenderMode", RenderMode);

        ini->WriteBool("MAIN","OVS1x",OVS1x->Checked);
        ini->WriteBool("MAIN","OVS2x",OVS2x->Checked);
        ini->WriteBool("MAIN","OVS3x",OVS3x->Checked);

        ini->WriteString("RecentFiles","MRU1", MruList->Strings[0]);
        ini->WriteString("RecentFiles","MRU2", MruList->Strings[1]);
        ini->WriteString("RecentFiles","MRU3", MruList->Strings[2]);
        ini->WriteString("RecentFiles","MRU4", MruList->Strings[3]);
        ini->WriteString("RecentFiles","MRU5", MruList->Strings[4]);

        KeysWrite(ini);

        cartprofile->SaveSettings(ini);
        hardware->SaveSettings(ini);
        patternviewer->SaveSettings(ini);
        spriteviewer->SaveSettings(ini);
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
        static int startup=0;
        AnsiString Filename, Ext;
        int i=0;
        int targetfps;

        // Manage startup for size & load command line rom
        if (startup<=2) startup++;
        switch(startup) {
                case 1:
                        if ( (StartUpWidth==0) || (StartUpHeight==0)) OVS1xClick(NULL);
                        else
                        {
                                Width=StartUpWidth;
                                Height=StartUpHeight;
                        }
                        break;
                case 2:
                        startup++;
                        while(CommandLine[i])
                        {
                                Filename=CommandLine[i];
                                Ext = FileNameGetExt(Filename);

                                if (Ext==".COL" || Ext==".BIN" || Ext==".ROM")
                                {
                                        LoadProgram(Filename);
                                        break;
                                }
                                i++;
                        }
                        break;
                default:
                break;
        }

        // Compute current fps
        targetfps = coleco.NTSC ? 60:50;
        if (((targetfps-1) == fps) || ((targetfps+1)==fps)) fps=targetfps;
        targetfps = (targetfps  * 9) / 10;
        if (fps > (targetfps+2) && coleco.frameskip>0) coleco.frameskip--;
        if (fps < targetfps && coleco.frameskip<10 && coleco.frameskip>=0) coleco.frameskip++;

        // Add Fps and current state
        AnsiString text="";
        if (coleco.stop) text +="Paused";
         else
        {
                if (coleco.singlestep)
                        text +="Debug Mode";
                else
                {
                        text += coleco.NTSC ? "NTSC " : "PAL ";
                        text += fps;
                        text += "fps";
                        if (coleco.frameskip>0)
                        {
                                text += " FS ";
                                text += coleco.frameskip;
                        }
                }
        }
        fps=0;

        // Change Status bar
        StatusBar1->Panels->Items[1]->Text = text;

        // Update viewers if available
        if (patternviewer->Visible) patternviewer->do_refresh();
        if (nametabviewer->Visible) nametabviewer->do_refresh();
        if (spriteviewer->Visible) spriteviewer->do_refresh();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::OVS1xClick(TObject *Sender)
{
        OVS1x->Checked=true;
        OVS2x->Checked=false;
        OVS3x->Checked=false;

        StatusBar1->SizeGrip = false;
        BorderStyle = bsSingle;
        TBorderIcons newBorderIcons = BorderIcons;
        newBorderIcons >> biMinimize;
        newBorderIcons >> biMaximize;
        BorderIcons = newBorderIcons;

        ClientWidth=BaseWidth;
        ClientHeight=BaseHeight;
        ClientHeight += StatusBar1->Height;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OVS2xClick(TObject *Sender)
{
        OVS1x->Checked=false;
        OVS2x->Checked=true;
        OVS3x->Checked=false;

        StatusBar1->SizeGrip = false;
        BorderStyle = bsSingle;
        TBorderIcons newBorderIcons = BorderIcons;
        newBorderIcons >> biMinimize;
        newBorderIcons >> biMaximize;
        BorderIcons = newBorderIcons;

        ClientWidth=BaseWidth*2;
        ClientHeight=BaseHeight*2;
        ClientHeight += StatusBar1->Height;
        StatusBar1->Refresh();
        StatusBar1->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OVS3xClick(TObject *Sender)
{
        OVS1x->Checked=false;
        OVS2x->Checked=false;
        OVS3x->Checked=true;

        StatusBar1->SizeGrip = false;
        BorderStyle = bsSingle;
        TBorderIcons newBorderIcons = BorderIcons;
        newBorderIcons >> biMinimize;
        newBorderIcons >> biMaximize;
        BorderIcons = newBorderIcons;

        ClientWidth=BaseWidth*3;
        ClientHeight=BaseHeight*3;
        ClientHeight += StatusBar1->Height;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Open1Click(TObject *Sender)
{
        AnsiString Extension, Filename;
        int stopped;

        SoundSuspend();
        try
        {
                stopped=coleco.stop;
                coleco.stop=1;

        //PCAllKeysUp();
                if (!OpenDialog1->Execute())
                {
                        coleco.stop=stopped;
                }
                else
                {
                        coleco.stop=stopped;
                        Filename=OpenDialog1->FileName;
                        Extension=FileNameGetExt(Filename);
                        LoadProgram(Filename);
                }
        }
        catch (Exception &exception)
        {
                // The default exception handler not only shows the exception that
                // occured, but also quits the message handler
                Application->ShowException(&exception);
        }

        SoundResume();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Emulation1Click(TObject *Sender)
{
        hardware->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AnimTimer1Timer(TObject *Sender)
{
        static int j, borrow;

        //if (!nosound) sound_frame();

        if (coleco.stop)
        {
                AccurateUpdateDisplay(false);
                return;
        }

        fps++;
        frametstates=0;

        //j=coleco.single_step?1:(machine.tperframe + borrow);

        //while (j>0 && !coleco.stop)
        //{
                j-= machine.do_scanline();
        //}
        AccurateUpdateDisplay(coleco.singlestep ? false : true); //AccurateDraw(BuildLine);

        //if (!coleco.stop) borrow=j;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SoftReset1Click(TObject *Sender)
{
        // do a reset with the current rom
        coleco.stop=1;
        machine.reset();
        coleco.stop=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HardReset1Click(TObject *Sender)
{
        int initialStopState = coleco.stop;

        coleco.stop=1;
        AccurateInit(false);
        machine.initialise();
        coleco.stop=initialStopState;

//        DebugUpdate();
//        LiveMemoryWindow->Reset();
//        BasicLister->Clear();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::FormResize(TObject *Sender)
{
        extern void RecalcWinSize(void);

        RecalcWinSize();

        StatusBar1->Panels->Items[3]->Width =
                StatusBar1->ClientWidth -
                        (StatusBar1->Panels->Items[0]->Width
                         +StatusBar1->Panels->Items[1]->Width
                         +StatusBar1->Panels->Items[2]->Width);

        StatusBar1->Refresh();
        StatusBar1->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::mRU1Click(TObject *Sender)
{
        AnsiString Extension, Filename;

        // Load a file based on the MRU item that was clicked.
        TMenuItem* itemClicked = dynamic_cast < TMenuItem * > (Sender);
        try
        {
                Filename=MruList->Strings[itemClicked->MenuIndex - mRU1->MenuIndex];
                Extension=FileNameGetExt(Filename);
                LoadProgram(Filename);
        }
        catch (Exception &exception)
        {
                // The default exception handler not only shows the exception that
                // occured, but also quits the message handler
                Application->ShowException(&exception);
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        //if (Key==VK_LSHIFT) Key=VK_SHIFT;
        //if (Key==VK_RSHIFT) Key=VK_CONTROL;
        CheckKeyDown(Key);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        //if (Key==VK_LSHIFT) Key=VK_SHIFT;
        //if (Key==VK_RSHIFT) Key=VK_CONTROL;
        CheckKeyUp(Key);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Screenshot1Click(TObject *Sender)
{
    // Check save dialog to save effectively to disk
    try
    {
        if(Form1->SaveScrDialog->Execute())
        {
                if (Form1->SaveScrDialog->FilterIndex==1)
                        RenderSaveScreenBMP(Form1->SaveScrDialog->FileName);
                else
                        RenderSaveScreenPNG(Form1->SaveScrDialog->FileName);
        }
    }
    catch (Exception &exception)
    {
        // The default exception handler not only shows the exception that
        // occured, but also quits the message handler
        Application->ShowException(&exception);
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::TilesViewer1Click(TObject *Sender)
{
    TilesViewer1->Checked = !TilesViewer1->Checked;
    if (TilesViewer1->Checked) patternviewer->Show();
    else patternviewer->Close();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Cartprofile1Click(TObject *Sender)
{
        Cartprofile1->Checked = !Cartprofile1->Checked;
        if (Cartprofile1->Checked) cartprofile->Show();
        else cartprofile->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TilemapViewer1Click(TObject *Sender)
{
    TilemapViewer1->Checked = !TilemapViewer1->Checked;
    if (TilemapViewer1->Checked) nametabviewer->Show();
    else nametabviewer->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpritesViewer1Click(TObject *Sender)
{
    SpritesViewer1->Checked = !SpritesViewer1->Checked;
    if (SpritesViewer1->Checked) spriteviewer->Show();
    else spriteviewer->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Debugger1Click(TObject *Sender)
{
        Debugger1->Checked = !Debugger1->Checked;
        if (Debugger1->Checked)
        {
                debug->Show();
        }
        else
        {
                debug->Close();
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Run1Click(TObject *Sender)
{
        debug->RunStop1Click(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadProgram(AnsiString FileName)
{
        BYTE retload;

        try
        {
                coleco.stop=1;
                retload=coleco_loadcart(FileName.c_str(),0);

                if (retload == ROM_LOAD_PASS)
                {
                        // Reset engine
                        coleco_reset();

                        // add to MUR list
                        MruList->Insert(0, FileName);
                        if (MruList->Count > MRFUCOUNT)
                            MruList->Delete(MRFUCOUNT);
                        UpdateMruMenu();

                        coleco.stop=0;//machine.StartDebug ? 1 : 0;

                        // if stop, well, show debug Window
                        if (coleco.stop)
                        {
                            //Debugger1->Checked=true;
                            //Debug->Show();
                        }
                }
                else if (retload == ROM_VERIFY_FAIL)
                {
                        // Show error
                        Application->MessageBox("Can't verify the rom file","Error",
        	                    MB_OK | MB_ICONERROR);
                }
                // Show error
                else
                {
                        Application->MessageBox("Can't open the rom file","Error",
        	        MB_OK | MB_ICONERROR);
                }
        }
        catch (Exception &exception)
        {
                // The default exception handler not only shows the exception that
                // occured, but also quits the message handler
                Application->ShowException(&exception);
        }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::LoadDiskTape(int TypeMedia, int DiskTapeNum, AnsiString FileName)
{
        BYTE retload;

        try
        {
                coleco.stop=1;
                if (TypeMedia==0) // Disk
                        retload=LoadFDI(&Disks[DiskTapeNum],FileName.c_str(),FMT_ADMDSK);
                else
                        retload=LoadFDI(&Tapes[DiskTapeNum],FileName.c_str(),FMT_DDP);

                if (retload && !DiskTapeNum) // only for 1st disk
                {
                        // Reset engine in Adam mode
                        coleco.machine=MACHINEADAM;
                        coleco_reset();

                        coleco.stop=0;//machine.StartDebug ? 1 : 0;

                        // if stop, well, show debug Window
                        if (coleco.stop)
                        {
                            //Debugger1->Checked=true;
                            //Debug->Show();
                        }
                }
                // Show error
                else
                {
                        Application->MessageBox(TypeMedia ? "Can't open the .DDP tape file" : "Can't open the .DSK disk file","Error",
        	        MB_OK | MB_ICONERROR);
                }
        }
        catch (Exception &exception)
        {
                // The default exception handler not only shows the exception that
                // occured, but also quits the message handler
                Application->ShowException(&exception);
        }
}

//---------------------------------------------------------------------------

void __fastcall TForm1::DDAInsertClick(TObject *Sender)
{
        AnsiString Extension, Filename;
        int stopped;
        TMenuItem *m;

        SoundSuspend();
        try
        {
                stopped=coleco.stop;
                coleco.stop=1;

        //PCAllKeysUp();
                if (!OpenDialogDT->Execute())
                {
                        coleco.stop=stopped;
                }
                else
                {
                        coleco.stop=stopped;
                        Filename=OpenDialogDT->FileName;
                        Extension=FileNameGetExt(Filename);
                        m = (TMenuItem *) Sender;
                        if (m->Tag == 0)
                                LoadDiskTape(0, 0,Filename);
                        else if (m->Tag == 1)
                                LoadDiskTape(0, 1,Filename);
                        else if (m->Tag == 2)
                                LoadDiskTape(1, 0,Filename);
                }
        }
        catch (Exception &exception)
        {
                // The default exception handler not only shows the exception that
                // occured, but also quits the message handler
                Application->ShowException(&exception);
        }

        SoundResume();
}

//---------------------------------------------------------------------------


void __fastcall TForm1::PrinterViewer1Click(TObject *Sender)
{
    PrinterViewer1->Checked = !PrinterViewer1->Checked;
    if (PrinterViewer1->Checked) printviewer->Show();
    else printviewer->Close();
}

void __fastcall TForm1::DDAEjectClick(TObject *Sender)
{
        TMenuItem *m;

        m = (TMenuItem *) Sender;
        if (m->Tag == 0)
                EjectFDI(&Disks[0]);
        else if (m->Tag == 1)
                EjectFDI(&Disks[1]);
        else if (m->Tag == 2)
                EjectFDI(&Tapes[0]);
}
//---------------------------------------------------------------------------

