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
#include "joyconf_.h"

#include "ioviewer_.h"
#include "iovdpviewer_.h"
#include "soundviewer_.h"
#include "nametabviewer_.h"
#include "patternviewer_.h"
#include "spriteviewer_.h"
#include "palviewer_.h"
#include "printviewer_.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

//---------------------------------------------------------------------------
#define MRFUCOUNT 5

extern char **CommandLine;
extern HINSTANCE g_hwndMain;
static bool iniFileExists = false;

bool nosound;

//---------------------------------------------------------------------------

void __fastcall TForm1::WndProc(TMessage &Message)
{
    switch(Message.Msg)
    {
    case WM_USER:
        if (RunFrameEnable)
            Form1->RunFrame();
            break;

    default:
        break;
    }
	TForm::WndProc(Message);
}

//---------------------------------------------------------------------------

__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
    AnsiString IniPath;
    char path[256];
    int i;

    RunFrameEnable=false;

    strcpy(emul2.cwd, (FileNameGetPath(Application->ExeName)).c_str());
    if (emul2.cwd[strlen(emul2.cwd)-1]!='\\')
    {
        emul2.cwd[strlen(emul2.cwd)-1]='\\';
        emul2.cwd[strlen(emul2.cwd)]='\0';
    }

    IniPath=ChangeFileExt(Application->ExeName, ".ini" );
    strcpy(emul2.inipath, IniPath.c_str());
    IniPath=FileNameGetPath(Application->ExeName);
    strcpy(emul2.configpath, IniPath.c_str());
    strcpy(emul2.mydocs, emul2.cwd);

    for(i=0; CommandLine[i]!=NULL; i++)
    {
        if (FileNameGetExt(CommandLine[i]) == ".INI")
        {
            IniPath=CommandLine[i];
            if (IniPath.Pos("\\")==0)
                IniPath=emul2.configpath + IniPath;
            strcpy(emul2.inipath, IniPath.c_str());
        }
    }

    MruList = new TStringList;

    RenderMode=RENDERGDI;
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
}

//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
        DragAcceptFiles(Handle, true);

        StatusBar1->Refresh();
        StatusBar1->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Exit1Click(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::About1Click(TObject *Sender)
{
    int stopped=emul2.stop;

    emul2.stop=1;
    Sound.SoundSuspend();
    about->ShowModal();
    Sound.SoundResume();
    emul2.stop=stopped;
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
        int J;

        // stop properly the current machine
        if (machine.exit) machine.exit();
        emul2.stop=1;

        RunFrameEnable=false;

        // Eject all disks and tapes
        for(J=0;J<MAX_DISKS;++J) EjectFDI(&Disks[J]);
        for(J=0;J<MAX_TAPES;++J) EjectFDI(&Tapes[J]);

        // Release sound
        if (!nosound) Sound.End();

        // Save current settings
        ini = new TIniFile(emul2.inipath);
        SaveSettings(ini);
        delete ini;

        // Release video and input
        RenderEnd();
        JoystickEnd();

        delete MruList;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    TIniFile *ini;
    char soundfile[256];

    RunFrameEnable=false;

    nosound = false;

    Application->OnMessage = AppMessage;

    load_config();

    // Load current ini config
    ini = new TIniFile(emul2.inipath);
    iniFileExists = FileExists(emul2.inipath);
    LoadSettings(ini);
    delete ini;

    // Init joystick if possible
    JoystickInit(Form1->Handle, g_hwndMain);

    // Prepare timer for emulation
    Timer2->Interval=1000;

    // Prepare sound
    if ( Sound.Initialise(Form1->Handle, machine.fps,0,0,0 )) MessageBox(NULL, "","Sound Error",0);
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
                else if (Ext==".DDP")
                {
                    LoadDiskTape(1, 0,Filename);
                }
                else if (Ext==".DSK")
                {
                    LoadDiskTape(0, 0,Filename);
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
    RenderMode=ini->ReadInteger("MAIN","RenderMode", RENDERGDI); // DDraw bugs with W11
    if (!RenderInit())
        {
            RenderEnd();
            exit(0);
        }
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

        KeysRead(ini);

        Sound.ReInitialise(0, 0, 8, 44100, 1);

/*
        if (!SoundInit(44100,60))
        {
            Application->MessageBox("Count not initialise Direct\nPlease ensure DirectX 5 or greater is installed.",
				"Error",
	            MB_OK | MB_ICONERROR);
            SoundEnd();
            nosound = true;
        }
*/

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

        // Update the status bar
        UpdateStatusBar();
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
        debug->SaveSettings(ini);
        hardware->SaveSettings(ini);
        iomapviewer->SaveSettings(ini);
        iovdpviewer->SaveSettings(ini);
        joyconf->SaveSettings(ini);
        nametabviewer->SaveSettings(ini);
        paletteviewer->SaveSettings(ini);
        patternviewer->SaveSettings(ini);
        printviewer->SaveSettings(ini);
        spriteviewer->SaveSettings(ini);
        soundviewer->SaveSettings(ini);
}
//---------------------------------------------------------------------------
void TForm1::UpdateStatusBar(void)
{
        AnsiString text="";

        // Update status bar
        text="Coleco";
        if (emul2.machine == MACHINEPHOENIX)
            text = "Phoenix";
        else if (emul2.machine == MACHINEADAM)
            text = "Adam";
        StatusBar1->Panels->Items[0]->Text = text;

        text="No cart";
        if (emul2.romCartridge == ROMCARTRIDGESTD)
            text="Standard Cart";
        if (emul2.romCartridge == ROMCARTRIDGEMEGA)
            text = "MegaCart";
        else if (emul2.romCartridge == ROMCARTRIDGEDTAPE)
            text = "Disk Image";
        else if (emul2.romCartridge == ROMCARTRIDGEDTAPE)
            text = "Digital Data Pack";
        StatusBar1->Panels->Items[2]->Text = text;

        text="";
        if (emul2.SGM == 1)
            text += " SGM";
        if (emul2.F18A == 1)
            text += " F18A";
        StatusBar1->Panels->Items[3]->Text = text;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
    static int startup=0;
    AnsiString Filename, Ext;
    int i=0;
    int targetfps;

    static HWND OldhWnd=NULL;

    if (Form1->Handle != OldhWnd)
    {
        OldhWnd=Form1->Handle;
        Sound.ReInitialise(OldhWnd, 0,0,0,0);

        RenderEnd();
        RenderInit();
        AccurateInit(true);
    }

        // Manage startup for size & load command line rom
        if (startup<=2) startup++;
        switch(startup)
        {
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
        targetfps = emul2.NTSC ? 60:50;
        if (((targetfps-1) == fps) || ((targetfps+1)==fps)) fps=targetfps;
        targetfps = (targetfps  * 9) / 10;
        if (fps > (targetfps+2) && emul2.frameskip>0) emul2.frameskip--;
        if (fps < targetfps && emul2.frameskip<10 && emul2.frameskip>=0) emul2.frameskip++;

        // Add Fps and current state
        AnsiString text="";
        if (emul2.stop) text +="Paused";
     else
        {
            if (emul2.singlestep)
                text +="Debug Mode";
            else
            {
                text += emul2.NTSC ? "NTSC " : "PAL ";
                text += fps;
                text += "fps";
                if (emul2.frameskip>0)
                {
                    text += " FS ";
                    text += emul2.frameskip;
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
        if (paletteviewer->Visible) paletteviewer->do_refresh();
        if (iomapviewer->Visible) iomapviewer->do_refresh();
        if (iovdpviewer->Visible) iovdpviewer->do_refresh();
        if (soundviewer->Visible) soundviewer->do_refresh();
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

void __fastcall TForm1::Emulation1Click(TObject *Sender)
{
    int stopped=emul2.stop;

    emul2.stop=1;
    Sound.SoundSuspend();
    hardware->ShowModal();
    Sound.SoundResume();
    emul2.stop=stopped;

    // Refresh StatusBar if needed
    UpdateStatusBar();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RunFrame()
{
    if (emul2.stop)
    {
        AccurateUpdateDisplay(false);
        return;
    }

    fps++;
    frametstates=0;

    // Update Joystick, spinner once per frame
    KeybJoyUpdate();

    // Update emulation
    machine.do_scanline();
    AccurateUpdateDisplay(emul2.singlestep ? true : false);//false : true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SoftReset1Click(TObject *Sender)
{
        // do a reset with the current rom
        emul2.stop=1;
        if (machine.reset) machine.reset();
        emul2.stop=0;
        //DebugUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HardReset1Click(TObject *Sender)
{
        emul2.stop=1;
        AccurateInit(false);
        machine.initialise();
        emul2.stop=emul2.startdebug ? 1 : 0;

//    DebugUpdate();
//    LiveMemoryWindow->Reset();
//    BasicLister->Clear();
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
        CheckKeyDown(Key);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        CheckKeyUp(Key);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Screenshot1Click(TObject *Sender)
{
        int stopped=emul2.stop;

        Sound.SoundSuspend();
        try
        {
            emul2.stop=1;

            // Check save dialog to save effectively to disk
            SaveDialog->DefaultExt="png";
            SaveDialog->FileName="*.png";
            SaveDialog->Filter="Portable Network Graphic (.png)|*.png|Windows Bitmap (.bmp)|*.bmp";
            if(SaveDialog->Execute())
            {
                if (SaveDialog->FilterIndex==1)
                    RenderSaveScreenPNG(SaveDialog->FileName);
                else
                    RenderSaveScreenBMP(SaveDialog->FileName);
            }
        }
        catch (Exception &exception)
        {
            // The default exception handler not only shows the exception that
            // occured, but also quits the message handler
            Application->ShowException(&exception);
        }
        Sound.SoundResume();
        emul2.stop=stopped;
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

void __fastcall TForm1::TilesViewer1Click(TObject *Sender)
{
    TilesViewer1->Checked = !TilesViewer1->Checked;
    if (TilesViewer1->Checked) patternviewer->Show();
    else patternviewer->Close();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::IOVdpViewer1Click(TObject *Sender)
{
    IOVdpViewer1->Checked = !IOVdpViewer1->Checked;
    if (IOVdpViewer1->Checked) iovdpviewer->Show();
    else iovdpviewer->Close();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::PaletteViewer1Click(TObject *Sender)
{
    PaletteViewer1->Checked = !PaletteViewer1->Checked;
    if (PaletteViewer1->Checked) paletteviewer->Show();
    else paletteviewer->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::IOMapViewer1Click(TObject *Sender)
{
    IOMapViewer1->Checked = !IOMapViewer1->Checked;
    if (IOMapViewer1->Checked) iomapviewer->Show();
    else iomapviewer->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SoundViewer1Click(TObject *Sender)
{
    SoundViewer1->Checked = !SoundViewer1->Checked;
    if (SoundViewer1->Checked) soundviewer->Show();
    else soundviewer->Close();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::PrinterViewer1Click(TObject *Sender)
{
    PrinterViewer1->Checked = !PrinterViewer1->Checked;
    if (PrinterViewer1->Checked) printviewer->Show();
    else printviewer->Close();
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
        emul2.stop=1;
        retload=coleco_loadcart(FileName.c_str());

        if (retload == ROM_LOAD_PASS)
        {
            // Reset engine in coleco mode
            emul2.machine=MACHINECOLECO;
            strcpy(emul2.currentrom,FileName.c_str());
            coleco_reset();

            // add to MUR list
            MruList->Insert(0, FileName);
            if (MruList->Count > MRFUCOUNT)
            MruList->Delete(MRFUCOUNT);
            UpdateMruMenu();

            // Update StatusBar if needed
            UpdateStatusBar();

            emul2.stop=emul2.startdebug ? 1 : 0;

            // if stop, well, show debug Window
            if (emul2.stop)
            {
                Debugger1->Checked=true;
                debug->Show();
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
        emul2.stop=1;
        if (TypeMedia==0) // Disk
            retload=LoadFDI(&Disks[DiskTapeNum],FileName.c_str(),FMT_ADMDSK);
        else
            retload=LoadFDI(&Tapes[DiskTapeNum],FileName.c_str(),FMT_DDP);

        if (retload && !DiskTapeNum) // only for 1st disk
        {
            // Change media type  and eject disk if on a tape
            emul2.romCartridge = TypeMedia ? ROMCARTRIDGEDTAPE : ROMCARTRIDGEDISK;
            if (TypeMedia)
                EjectFDI(&Disks[0]);


            // Reset engine in Adam mode
            emul2.machine=MACHINEADAM;
            coleco_reset();
            strcpy(emul2.currentrom,FileName.c_str());

            // Update StatusBar if needed
            UpdateStatusBar();

            emul2.stop=emul2.startdebug ? 1 : 0;

            // if stop, well, show debug Window
            if (emul2.stop)
            {
                Debugger1->Checked=true;
                debug->Show();
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

void __fastcall TForm1::Open1Click(TObject *Sender)
{
        AnsiString Extension, Filename;
        int stopped=emul2.stop;

        Sound.SoundSuspend();
        try
        {
            emul2.stop=1;

            OpenDialog->DefaultExt="rom";
            OpenDialog->FileName="*";
            OpenDialog->Filter="All Files (*.COL,*.ROM,*.BIN)|*.col;*.rom;*.bin|BIN Files (*.BIN)|*.bin|COL Files (*.COL)|*.col|ROM Files (*.ROM)|*.rom";
            if (OpenDialog->Execute())
            {
                Filename=OpenDialog->FileName;
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
        emul2.stop=stopped;
        Sound.SoundResume();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::DDAInsertClick(TObject *Sender)
{
        AnsiString Extension, Filename;
        int stopped;
        TMenuItem *m;

        Sound.SoundSuspend();
        try
        {
            stopped=emul2.stop;
            emul2.stop=1;

            m = (TMenuItem *) Sender;
            if ((m->Tag == 0) || (m->Tag == 1))
            {
                OpenDialog->FileName="*.dsk";
                OpenDialog->Filter="Disk Files (*.DSK)|*.dsk";
                OpenDialog->DefaultExt="dsk";
            }
            else
            {
                OpenDialog->FileName="*.ddp";
                OpenDialog->Filter="Tape Files (*.DDP)|*.ddp";
                OpenDialog->DefaultExt="ddp";
            }
            if (!OpenDialog->Execute())
            {
                emul2.stop=stopped;
            }
            else
            {
                emul2.stop=stopped;
                Filename=OpenDialog->FileName;
                Extension=FileNameGetExt(Filename);
                if (m->Tag == 0)
                    LoadDiskTape(0, 0,Filename);
                else if (m->Tag == 1)
                    LoadDiskTape(0, 1,Filename);
                else if (m->Tag == 2)
                    LoadDiskTape(1, 0,Filename);
                else if (m->Tag == 3)
                    LoadDiskTape(1, 1,Filename);
            }
        }
        catch (Exception &exception)
        {
            // The default exception handler not only shows the exception that
            // occured, but also quits the message handler
            Application->ShowException(&exception);
        }

        Sound.SoundResume();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::LoadState1Click(TObject *Sender)
{
        int stopped=emul2.stop;

        Sound.SoundSuspend();
        try
        {
            emul2.stop=1;

            // Check save dialog to save effectively current emulator state to disk
            OpenDialog->DefaultExt="sta";
            OpenDialog->FileName="*.sta";
            OpenDialog->Filter="Save State (.sta)|*.sta";
            if(OpenDialog->Execute())
            {
                coleco_loadstate(OpenDialog->FileName.c_str());
            }
        }
        catch (Exception &exception)
        {
            // The default exception handler not only shows the exception that
            // occured, but also quits the message handler
            Application->ShowException(&exception);
        }
        emul2.stop=stopped;
        Sound.SoundResume();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveState1Click(TObject *Sender)
{
        int stopped=emul2.stop;

        Sound.SoundSuspend();
        try
        {
            emul2.stop=1;

            // Check save dialog to save effectively current emulator state to disk
            SaveDialog->DefaultExt="sta";
            SaveDialog->FileName="*.sta";
            SaveDialog->Filter="Save State (.sta)|*.sta";
            if(SaveDialog->Execute())
            {
                coleco_savestate(SaveDialog->FileName.c_str());
            }
        }
        catch (Exception &exception)
        {
            // The default exception handler not only shows the exception that
            // occured, but also quits the message handler
            Application->ShowException(&exception);
        }
        emul2.stop=stopped;
        Sound.SoundResume();
}
//---------------------------------------------------------------------------

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
        else if (m->Tag == 3)
            EjectFDI(&Tapes[1]);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OnJoyMove(TMessage &msg)
{
        // Check joystick
        CheckJoyMove(msg);

        // Continue to impart information
        TForm::Dispatch (&msg);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OnJoyDown(TMessage &msg)
{
        // Check joystick
        CheckJoyDown(msg);

        // Dispatch message with joystick
        TForm::Dispatch (&msg);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OnJoyUp(TMessage &msg)
{
        // Check joystick
        CheckJoyUp(msg);

        // Dispatch message with joystick
        TForm::Dispatch (&msg);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Reportabug1Click(TObject *Sender)
{
        ShellExecute(NULL, "open", "https://github.com/alekmaul/emultwo/issues", "", NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Chatwithcommunity1Click(TObject *Sender)
{
        ShellExecute(NULL, "open", "https://discord.gg/2qxH6FAgzW", "", NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
        CheckMouseMove(X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Content1Click(TObject *Sender)
{
    ShellExecute(NULL, "open", "https://github.com/alekmaul/pvcollib/wiki/Complete-Colecovision-Specs", "", NULL, SW_RESTORE);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::JoystickEditor1Click(TObject *Sender)
{
    int stopped=emul2.stop;

    emul2.stop=1;
    Sound.SoundSuspend();
    joyconf->ShowModal();
    emul2.stop=stopped;
    Sound.SoundResume();
}
//---------------------------------------------------------------------------

