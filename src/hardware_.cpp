//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "hardware_.h"

#include "main_.h"

#include "coleco.h"

#include "tms9928a.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Thardware *hardware;
//---------------------------------------------------------------------------
__fastcall Thardware::Thardware(TComponent* Owner)
        : TForm(Owner)
{
        TIniFile *ini;

        ini = new TIniFile(coleco.inipath);
        LoadSettings(ini);
        delete ini;

        ResetRequired=true;
        OKClick(NULL);
}

//---------------------------------------------------------------------------
//#include <set>
//extern std::set<int> dirtyBird;
void coleco_writebyteProxy(int address, int data)
{
        coleco_writebyte(address, data);
//        dirtyBird.insert(address);
}

void __fastcall Thardware::OKClick(TObject *Sender)
{
        AnsiString Name=NewMachineName;

/*
        //First put the new machine name in the status panel

        if (AceBtn->Down)
        {
                int i=3;
                AnsiString Mem;

                if (RamPackBox->ItemIndex!=0)
                        i+=atoi(RamPackBox->Items->Strings[RamPackBox->ItemIndex].c_str());

                Name=i;
                Name += "k Jupiter Ace";
        }
        else if (QLBtn->Down)
        {
                Name = QLMem->Items->Strings[QLMem->ItemIndex] + " " + NewMachineName;
        }
        else if (RamPackBox->Visible && RamPackBox->ItemIndex!=0)
        {
                Name = RamPackBox->Items->Strings[RamPackBox->ItemIndex]
                        + " " + Name;
        }

        zx81.zxpand = 0;

        if (NewMachine != zx81.machine && LiveMemoryWindow && LiveMemoryWindow->Visible)
                LiveMemoryWindow->Close();
*/
        strcpy(coleco.machinename, Name.c_str());
        Form1->StatusBar1->Panels->Items[0]->Text = Name;

//        strcpy(machine.CurRom, RomBox->Text.c_str());

        coleco.machine=NewMachine;
/*
        spectrum.machine=NewSpec;

        Form1->RamPackWobble1->Enabled=true;
        Form1->RamPackWobble1->Visible=true;
        Form1->InsertDockCart1->Visible=false;
        Form1->RemoveDockCart1->Visible=false;
        Form1->DockSpacer->Visible=false;

        if (NewMachine == MACHINEQL)
        {
                if (Dbg && Dbg->Visible) Dbg->Close();
        }
        else
        {
                if (Debug68k && Debug68k->Visible) Debug68k->Close();
        }

        bool prevChromaColourSwitchOn = (zx81.colour == COLOURCHROMA);
        bool prevSpectraColourSwitchOn = (zx81.colour == COLOURSPECTRA);

        switch(NewMachine)
        {
        case MACHINEQL:
                zx81.colour = COLOURDISABLED;
                break;
        case MACHINEACE:
                zx81.colour = (ColourBox->ItemIndex > 0) ? COLOURACE : COLOURDISABLED;
                break;
        case MACHINESPEC48:
                zx81.colour = (ColourBox->ItemIndex > 0) ? COLOURSPECTRA : COLOURSINCLAIR;
                break;
        default:
                zx81.colour = ColourBox->ItemIndex;
                break;
        }

        Artifacts->SelectRGBOutput(zx81.colour == COLOURSPECTRA);
        Artifacts->ForceVibrantColours(zx81.colour == COLOURCHROMA);
        if (zx81.colour == COLOURSPECTRA)
        {
                Form1->DisplayArt->Checked = false;
                Artifacts->Visible = false;
        }
        Form1->DisplayArt->Enabled = (zx81.colour != COLOURSPECTRA);

        zx81.chromaColourSwitchOn = (zx81.colour == COLOURCHROMA);
        if (!prevChromaColourSwitchOn && zx81.chromaColourSwitchOn)
        {
                InitialiseChroma();
        }
        Form1->ChromaColourEnable->Enabled = zx81.chromaColourSwitchOn;
        Form1->ChromaColourEnable->Checked = zx81.chromaColourSwitchOn;
        Form1->ChromaColourEnable->Visible = (NewMachine == MACHINEZX80) || (NewMachine == MACHINEZX81) || (NewMachine == MACHINETS1500);

        zx81.spectraColourSwitchOn = (zx81.colour == COLOURSPECTRA);
        if (!prevSpectraColourSwitchOn && zx81.spectraColourSwitchOn)
        {
                InitialiseSpectra();
        }
        Form1->SpectraColourEnable->Enabled = zx81.spectraColourSwitchOn;
        Form1->SpectraColourEnable->Checked = zx81.spectraColourSwitchOn;
        Form1->SpectraColourEnable->Visible = (NewMachine == MACHINESPEC48) && ((NewSpec != SPECCYTC2048) && (NewSpec != SPECCYTS2068) && (NewSpec != SPECCYSE));

        Form1->BasicListerOption->Enabled = false;
        if (BasicLister->ListerAvailable())
        {
                BasicLister->Clear();
                BasicLister->SetBasicLister(NULL);
        }

        switch(NewMachine)
        {
        case MACHINEZX80:
                strcpy(zx81.ROM80, machine.CurRom);
                zx81.zxpand = (ZXpand->Checked == true);
                CreateBasicLister();
                break;

        case MACHINEZX81:
                if (R470Btn->Down)
                {
                        strcpy(zx81.ROMR470, machine.CurRom);
                }
                else if (TK85Btn->Down)
                {
                        strcpy(zx81.ROMTK85, machine.CurRom);
                        CreateBasicLister();
                }
                else
                {
                        strcpy(zx81.ROM81, machine.CurRom);
                        CreateBasicLister();
                }
                zx81.zxpand = (ZXpand->Checked == true);
                ZXpand->Caption = "ZXpand+";

                break;

        case MACHINEACE:
                strcpy(zx81.ROMACE, machine.CurRom);
                break;

        case MACHINETS1500:
                strcpy(zx81.ROMTS1500, machine.CurRom);
                zx81.zxpand = (ZXpand->Checked == true);
                CreateBasicLister();
                break;

        case MACHINELAMBDA:
                strcpy(zx81.ROMLAMBDA, machine.CurRom);
                break;

        case MACHINEZX97LE:
                strcpy(zx81.ROM97LE, machine.CurRom);
                break;

        case MACHINEQL:
                Form1->RamPackWobble1->Enabled=false;
                Form1->RamPackWobble1->Visible=false;
                strcpy(zx81.ROMQL, machine.CurRom);
                break;

        case MACHINESPEC48:
                Form1->RamPackWobble1->Enabled=false;
                Form1->RamPackWobble1->Visible=false;
                switch(NewSpec)
                {
                case SPECCY16:
                        spectrum.RAMBanks=1;
                        spectrum.ROMBanks=1;
                        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters CF") strcpy(zx81.ROMZXCF, machine.CurRom);
                        else if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters 8Bit") strcpy(zx81.ROMZX8BIT, machine.CurRom);
                        else if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters 16Bit") strcpy(zx81.ROMZX16BIT, machine.CurRom);
                        else strcpy(zx81.ROMSP48, machine.CurRom);
                        spectrum.MFVersion=MF128;
                        CreateBasicLister();
                        break;

                case SPECCY48:
                        spectrum.RAMBanks=3;
                        spectrum.ROMBanks=1;
                        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters CF") strcpy(zx81.ROMZXCF, machine.CurRom);
                        else if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters 8Bit") strcpy(zx81.ROMZX8BIT, machine.CurRom);
                        else if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters 16Bit") strcpy(zx81.ROMZX16BIT, machine.CurRom);
                        else strcpy(zx81.ROMSP48, machine.CurRom);
                        spectrum.MFVersion=MF128;
                        CreateBasicLister();
                        break;

                case SPECCYTC2048:
                        Form1->InsertDockCart1->Visible=true;
                        Form1->RemoveDockCart1->Visible=true;
                        Form1->DockSpacer->Visible=true;
                        spectrum.RAMBanks=3;
                        spectrum.ROMBanks=1;
                        strcpy(zx81.ROMTC2048, machine.CurRom);
                        spectrum.MFVersion=MF128;
                        break;

                case SPECCYTS2068:
                        Form1->InsertDockCart1->Visible=true;
                        Form1->RemoveDockCart1->Visible=true;
                        Form1->DockSpacer->Visible=true;
                        spectrum.RAMBanks=3;
                        spectrum.ROMBanks=1;
                        strcpy(zx81.ROMTS2068, machine.CurRom);
                        spectrum.MFVersion=MF128;
                        break;

                case SPECCY128:
                        spectrum.RAMBanks=8;
                        spectrum.ROMBanks=1;
                        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters CF") strcpy(zx81.ROMZXCF, machine.CurRom);
                        else if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters 8Bit") strcpy(zx81.ROMZX8BIT, machine.CurRom);
                        else if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters 16Bit") strcpy(zx81.ROMZX16BIT, machine.CurRom);
                        else strcpy(zx81.ROMSP128, machine.CurRom);
                        spectrum.MFVersion=MF128;
                        CreateBasicLister();
                        break;

                case SPECCYPLUS2:
                        spectrum.RAMBanks=8;
                        spectrum.ROMBanks=1;
                        strcpy(zx81.ROMSPP2, machine.CurRom);
                        spectrum.MFVersion=MF128;
                        CreateBasicLister();
                        break;

                case SPECCYPLUS2A:
                        spectrum.RAMBanks=8;
                        spectrum.ROMBanks=1;
                        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="ZXCF")
                                strcpy(zx81.ROMSPP3ECF, machine.CurRom);
                        else if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Plus 2/3E")
                                strcpy(zx81.ROMSPP3E, machine.CurRom);
                        else strcpy(zx81.ROMSPP3, machine.CurRom);
                        spectrum.MFVersion=MFPLUS3;
                        CreateBasicLister();
                        break;

                case SPECCYPLUS3:
                        spectrum.RAMBanks=8;
                        spectrum.ROMBanks=1;
                        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="ZXCF")
                                strcpy(zx81.ROMSPP3ECF, machine.CurRom);
                        else if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Plus 2/3E")
                                strcpy(zx81.ROMSPP3E, machine.CurRom);
                        spectrum.MFVersion=MFPLUS3;
                        CreateBasicLister();
                        break;

                case SPECCYSE:
                        spectrum.RAMBanks=8;
                        spectrum.ROMBanks=1;
                        strcpy(zx81.ROMSPSE, machine.CurRom);
                        spectrum.MFVersion=MF128;
                        break;
                }
                break;
        }

        if (!BasicLister->ListerAvailable() && BasicLister->Visible)
        {
                BasicLister->Close();
        }

        zx81.romCartridge = RomCartridgeBox->ItemIndex;

        if ((zx81.romCartridge == ROMCARTRIDGENONE) || (RomCartridgeFileBox->Text.Trim() == ""))
        {
                RomCartridgeCapacity = 0;
                RomCartridgeFileBox->Enabled = false;
                BrowseRomCartridge->Enabled = false;
                RomCartridgeBox->ItemIndex = ROMCARTRIDGENONE;
       }
        else
        {
                LoadRomCartridgeFile(RomCartridgeFileBox->Text.c_str());
                RomCartridgeFileBox->Enabled = true;
                BrowseRomCartridge->Enabled = true;
                ResetRequired=true;
        }

        if (!Multiface->Checked) spectrum.MFVersion=MFNONE;

        spectrum.uspeech=uSpeech->Checked;

        Form1->InWaveLoader->Enabled=true;
        Form1->OutWaveLoader->Enabled=true;

        zx81.shadowROM = !EnableLowRAM->Checked;
        zx81.RAM816k = EnableLowRAM->Checked;
        zx81.protectROM = ProtectROM->Checked;

        if ((zx81.chrgen != CHRGENQS) && (ChrGenBox->ItemIndex == CHRGENQS))
        {
                zx81.enableQSchrgen = true;
        }
        else if (ChrGenBox->ItemIndex != CHRGENQS)
        {
                zx81.enableQSchrgen = false;
        }
        zx81.chrgen = ChrGenBox->ItemIndex;
        Form1->QSChrEnable->Checked = zx81.enableQSchrgen;
        Form1->QSChrEnable->Enabled = (zx81.chrgen == CHRGENQS);
        Form1->QSChrEnable->Visible = (NewMachine != MACHINEACE) && (NewMachine != MACHINEQL) && (NewMachine != MACHINESPEC48);

        zx81.FloatingPointHardwareFix = FloatingPointHardwareFix->Checked;

        zx81.zxprinter = ZXPrinter->Checked;
        zx81.extfont=0;
        if (zx81.chrgen==CHRGENDK || zx81.chrgen==CHRGENCHR16) zx81.maxireg=64;
        else zx81.maxireg=32;
        if (zx81.chrgen == CHRGENLAMBDA) zx81.extfont=1;

        */
        coleco.NTSC = chkNTSC->Checked;
        if (coleco.NTSC) Form1->AnimTimer1->Interval=17;
        else Form1->AnimTimer1->Interval=20;

        coleco.SGM = chkSGM->Checked;
        
        /*
        if ((NewMachine == MACHINESPEC48) || (NewMachine == MACHINEQL))
        {
                zx81.RAMTOP = 65535;
                zx81.ace96k=0;
        }
        else if (RamPackBox->Items->Strings[RamPackBox->ItemIndex]=="96k")
        {
                zx81.RAMTOP = 65535;
                zx81.ace96k=1;
        }
        else
        {
                zx81.RAMTOP = (1 << (RamPackBox->ItemIndex + 10)) + 16383;
                if (RamPackBox->ItemIndex == 6) zx81.RAMTOP = 65535;
                if (zx81.machine==MACHINEACE && RamPackBox->ItemIndex==0) zx81.RAMTOP=16383;
                zx81.ace96k=0;
        }

        Form1->MemotechReset->Enabled = false;
        Form1->MemotechReset->Visible = (NewMachine != MACHINEACE) && (NewMachine != MACHINEQL) && (NewMachine != MACHINESPEC48);

        switch(HiResBox->ItemIndex)
        {
        case 1: zx81.truehires = HIRESWRX; break;
        case 2: zx81.truehires = HIRESG007; break;
        case 3: zx81.truehires = HIRESMEMOTECH;
                Form1->MemotechReset->Enabled = true;
                break;
        default:
        case 0: zx81.truehires = HIRESDISABLED; break;
        }

        switch(SoundCardBox->ItemIndex)
        {
        case 1: zx81.aysound=1; zx81.aytype=AY_TYPE_ACE; break;
        case 2: zx81.aysound=1; zx81.aytype=AY_TYPE_BOLDFIELD; break;
        case 3: zx81.aysound=1; zx81.aytype=AY_TYPE_FULLER; break;
        case 4: zx81.aysound=1; zx81.aytype=AY_TYPE_SINCLAIR; break;
        case 5: zx81.aysound=1; zx81.aytype=AY_TYPE_TIMEX; break;
        case 6: zx81.aysound=1; zx81.aytype=AY_TYPE_QUICKSILVA; break;
        case 7: zx81.aysound=1; zx81.aytype=AY_TYPE_ZONX; break;
        case 0:
        default: zx81.aysound=0; zx81.aytype=0; break;
        }

        spectrum.HDType=HDNONE;
        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="ZXCF") spectrum.HDType=HDZXCF;
        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="divIDE V1") { spectrum.HDType=HDDIVIDE; spectrum.divIDEVersion=1; }
        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="divIDE V2") { spectrum.HDType=HDDIVIDE; spectrum.divIDEVersion=2; }
        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Plus 2/3E") spectrum.HDType=HDPLUS3E;
        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="AceCF") spectrum.HDType=HDACECF;
        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters CF") spectrum.HDType=HDPITERSCF;
        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters 8Bit") spectrum.HDType=HDPITERS8B;
        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="Piters 16Bit") spectrum.HDType=HDPITERS16B;
        if (IDEBox->Items->Strings[IDEBox->ItemIndex]=="MWCFIde") spectrum.HDType=HDPITERSCF;
        spectrum.WriteProtectJumper=WriteProtect->Checked;
        spectrum.UploadJumper=Upload->Checked;

        switch(ZXCFRAM->ItemIndex)
        {
        case 0: spectrum.ZXCFRAMSize=128/16; break;
        case 1: spectrum.ZXCFRAMSize=512/16; break;
        case 2: spectrum.ZXCFRAMSize=1024/16; break;
        }

        //spectrum.interface1=false;
        spectrum.floppytype=FLOPPYNONE;
        if (FDC->Items->Strings[FDC->ItemIndex]=="MGT Disciple") spectrum.floppytype=FLOPPYDISCIPLE;
        if (FDC->Items->Strings[FDC->ItemIndex]=="MGT PlusD") spectrum.floppytype=FLOPPYPLUSD;
        if (FDC->Items->Strings[FDC->ItemIndex]=="Plus 3 FDC") spectrum.floppytype=FLOPPYPLUS3;
        if (FDC->Items->Strings[FDC->ItemIndex]=="Opus Discovery") spectrum.floppytype=FLOPPYOPUSD;
        if (FDC->Items->Strings[FDC->ItemIndex]=="BetaDisc") spectrum.floppytype=FLOPPYBETA;
        if (FDC->Items->Strings[FDC->ItemIndex]=="ZX Interface 1") spectrum.floppytype=FLOPPYIF1;
        if (FDC->Items->Strings[FDC->ItemIndex]=="ZX1541") spectrum.floppytype=FLOPPYZX1541;
        if (FDC->Items->Strings[FDC->ItemIndex]=="Larken") spectrum.floppytype=FLOPPYLARKEN81;

        if (Autoboot->Checked) spectrum.autoboot=true;
        else spectrum.autoboot=false;

        switch(DriveAType->ItemIndex)
        {
        case 0: spectrum.driveatype=DRIVENONE; break;
        case 1: spectrum.driveatype=DRIVE3INCHSS; break;
        case 2: spectrum.driveatype=DRIVE3INCHDS; break;
        case 3: spectrum.driveatype=DRIVE35INCHDS; break;
        case 4: spectrum.driveatype=DRIVE35INCHDS; break;
        }

        switch(DriveBType->ItemIndex)
        {
        case 0: spectrum.drivebtype=DRIVENONE; break;
        case 1: spectrum.drivebtype=DRIVE3INCHSS; break;
        case 2: spectrum.drivebtype=DRIVE3INCHDS; break;
        case 3: spectrum.drivebtype=DRIVE35INCHDS; break;
        case 4: spectrum.drivebtype=DRIVE35INCHDS; break;
        }
        zx81.m1not = M1Not->Checked? 49152 : 32768;

        Form1->Zx97Config->Enabled=false;
        if (zx81.machine==MACHINEZX97LE)
        {
                zx81.RAMTOP=65535;
                zx81.m1not=49152;
                Form1->Zx97Config->Enabled=true;
        }

        if (TS2050->Checked==1) { zx81.ts2050=1; Form1->TS20501->Enabled=true; }
        else { zx81.ts2050=0; Form1->TS20501->Enabled=false; }
*/
        machine.clockspeed=coleco.NTSC ? CLOCK_NTSC : CLOCK_PAL;
        machine.tperscanline=coleco.NTSC ? TMS9918_LINE : TMS9929_LINE;
        machine.scanlines=coleco.NTSC ? TMS9918_LINES : TMS9929_LINES;
        machine.tperframe = machine.tperscanline * machine.scanlines;
/*
        if (zx81.machine == MACHINEZX80)
        {
                machine.tperframe -= 3;
        }

        if (zx81.machine==MACHINELAMBDA)
                machine.tperscanline=208;

        switch(zx81.machine)
        {
        case MACHINEACE:
                machine.initialise = ace_initialise;
                machine.do_scanline = ace_do_scanline;
                machine.writebyte = ace_writebyte;
                machine.setbyte = ace_writebyte;
                machine.readbyte = ace_readbyte;
                machine.readoperandbyte = ace_readoperandbyte;
                machine.getbyte = ace_readbyte;
                machine.opcode_fetch = ace_opcode_fetch;
                machine.readport = ace_readport;
                machine.writeport = ace_writeport;
                machine.contendmem = ace_contend;
                machine.contendio = ace_contend;
                machine.reset = NULL;
                machine.nmi = NULL;
                machine.exit = NULL;
                break;

        case MACHINEQL:
                machine.initialise = ql_initialise;
                machine.do_scanline = ql_do_scanline;
                machine.writebyte = ql_writebyte;
                machine.setbyte = ql_writebyte;
                machine.readbyte = ql_readbyte;
                machine.readoperandbyte = ql_readbyte;
                machine.getbyte = ql_readbyte;
                machine.opcode_fetch = ql_opcode_fetch;
                machine.readport = ql_readport;
                machine.writeport = ql_writeport;
                machine.contendmem = ql_contend;
                machine.contendio = ql_contend;
                machine.reset = NULL;
                machine.nmi = NULL;
                machine.exit = NULL;
                break;

        case MACHINESPEC48:
                machine.initialise = spec48_initialise;
                machine.do_scanline = spec48_do_scanline;
                machine.writebyte = spec48_writebyte;
                machine.setbyte = spec48_setbyte;
                machine.readbyte = spec48_readbyte;
                machine.readoperandbyte = spec48_readoperandbyte;
                machine.getbyte = spec48_getbyte;
                machine.opcode_fetch = spec48_opcode_fetch;
                machine.readport = spec48_readport;
                machine.writeport = spec48_writeport;
                machine.contendmem = spec48_contend;
                machine.contendio = spec48_contendio;
                machine.reset = spec48_reset;
                machine.nmi = spec48_nmi;
                machine.exit = spec48_exit;

                if (spectrum.machine==SPECCY16 || spectrum.machine==SPECCY48
                        || spectrum.machine==SPECCYTC2048
                        || spectrum.machine==SPECCYSE
                        || spectrum.machine==SPECCYTC2048)
                {
                        machine.clockspeed=3500000;
                        machine.tperscanline=224;
                        machine.intposition=14336;
                        machine.scanlines=312;
                        machine.tperframe= machine.tperscanline * machine.scanlines;

                }
                //else if (spectrum.machine==SPECCYTC2048)
                //{
                //        machine.clockspeed=3528000;
                //        machine.tperscanline=226;
                //        machine.intposition=14336;
                //        machine.scanlines=312;
                //        machine.tperframe= machine.tperscanline * machine.scanlines;
                //}
                else if (spectrum.machine==SPECCYTS2068)
                {
                        machine.clockspeed=3528000;
                        machine.tperscanline=226;
                        machine.intposition=10848;
                        machine.scanlines=262;
                        machine.tperframe= machine.tperscanline * machine.scanlines;
                }
                else
                {
                        machine.clockspeed=3546900;
                        machine.tperscanline=228;
                        machine.intposition=14336-228+32;
                        machine.scanlines=311;
                        machine.tperframe= machine.tperscanline * machine.scanlines;
                }
                break;
        default:
        */
                machine.initialise = coleco_initialise;
                machine.do_scanline = coleco_do_scanline;
                machine.writebyte = coleco_writebyteProxy;
                machine.setbyte = coleco_setbyte;
                machine.readbyte = coleco_readbyte;
                machine.readoperandbyte = coleco_readoperandbyte;
                machine.getbyte = coleco_getbyte;
                machine.opcode_fetch = coleco_opcode_fetch;
                machine.readport = coleco_readport;
                machine.writeport = coleco_writeport;
                machine.contendmem = coleco_contend;
                machine.contendio = coleco_contend;
                machine.reset = coleco_reset;
                machine.nmi = NULL;
                machine.exit = NULL;
/*
                break;
        }

        spectrum.kbissue = Issue2->Checked;
        spectrum.kmouse = KMouse->Checked;

        Form1->DiskDrives1->Visible=true;
        P3Drive->FormShow(NULL);

        Dbg->GroupBoxZX81->Visible=false;
        Dbg->GroupBoxAce->Visible=false;
        Dbg->GroupBoxChroma->Visible=false;
        Dbg->GroupBoxSpectra->Visible=false;
        Dbg->GroupBoxZXC->Visible=false;

        switch(zx81.machine)
        {
        case MACHINEACE:
                Dbg->GroupBoxAce->Visible=true;
                break;

        case MACHINESPEC48:
                Dbg->GroupBoxSpectra->Visible=Form1->SpectraColourEnable->Visible;
                Dbg->GroupBoxZXC->Visible=Form1->SpectraColourEnable->Visible;
                break;

        default:
                Dbg->GroupBoxZX81->Visible=Form1->ChromaColourEnable->Visible;
                Dbg->GroupBoxChroma->Visible=Form1->ChromaColourEnable->Visible;
                Dbg->GroupBoxZXC->Visible=Form1->ChromaColourEnable->Visible;
                break;
        }

        if (spectrum.floppytype==FLOPPYOPUSD
                || spectrum.floppytype==FLOPPYPLUSD
                || spectrum.floppytype==FLOPPYDISCIPLE
                || spectrum.machine==SPECCYPLUS2A
                || spectrum.machine==SPECCYPLUS3)
        {
                Form1->PrinterPort1->Enabled=true;
                if (spectrum.floppytype==FLOPPYOPUSD)
                        Form1->PrinterPort1->Caption="Opus Printer Port";
                if (spectrum.floppytype==FLOPPYDISCIPLE)
                        Form1->PrinterPort1->Caption="Disciple Printer Port";
                if (spectrum.floppytype==FLOPPYPLUSD)
                        Form1->PrinterPort1->Caption="PlusD Printer Port";
                if (spectrum.machine==SPECCYPLUS2A)
                        Form1->PrinterPort1->Caption="+2A/+3 Printer Port";
                if (spectrum.machine==SPECCYPLUS3)
                        Form1->PrinterPort1->Caption="+2A/+3 Printer Port";
        }
        else
        {
                Form1->PrinterPort1->Enabled=false;
                Form1->PrinterPort1->Caption="Printer Port";
        }

        symbolstore::reset();

        AnsiString romBase = zx81.cwd;
        romBase += "ROM\\";

        AnsiString rom = machine.CurRom;

        if (!FileExists(rom))
        {
                rom = romBase + machine.CurRom;
        }
        if (!FileExists(rom))
        {
                ShowMessage("ROM file for this system not found. Go to:\n\n"
                "Options > Hardware > Advanced Settings > ROM File.");
        }

        AnsiString sym = romBase;
        sym += ChangeFileExt(machine.CurRom, ".sym");
        symbolstore::loadROMSymbols(sym.c_str());
        SymbolBrowser->RefreshContent();

        AnsiString bmp = romBase;
        bmp += ChangeFileExt(machine.CurRom, ".bmp");
        delete (Graphics::TBitmap*)machine.cset;
        machine.cset = NULL;
        if (FileExists(bmp))
        {
                Graphics::TBitmap* cset = new Graphics::TBitmap;
                cset->LoadFromFile(bmp);
                machine.cset = cset;
        }

        AccurateInit(false);
        Speed->Recalc(NULL);
        PCKbInit();
        Kb->OKClick(NULL);
        Artifacts->TrackBarChange(NULL);
        Artifacts->ConfigureDotCrawlOption();

        bool spec48 = (zx81.machine==MACHINESPEC48 && spectrum.machine<SPECCY128);
        if (!spec48)
        {
                Artifacts->DotCrawl1->Checked = false;
        }
*/
        if (ResetRequired) machine.initialise();
/*
        sound_ay_init();
        Keyboard->KbChange();

        Form1->RZX1->Enabled=false;
        if (zx81.machine==MACHINESPEC48) Form1->RZX1->Enabled=true;

        if ( ((zx81.machine==MACHINESPEC48
                || zx81.machine==MACHINEACE
                || zx81.machine==MACHINELAMBDA)
                        && !Form1->Sound1->Checked)
                || ( (zx81.machine!=MACHINESPEC48
                        && zx81.machine!=MACHINEACE
                        && zx81.machine!=MACHINELAMBDA)
                                && Form1->Sound1->Checked) )
                        Form1->Sound1Click(NULL);

        spectrum.drivebusy = -1;

        InitPatches(NewMachine);
*/
        if (Sender) Close();
/*
        if (Dbg->Visible) Dbg->UpdateVals();
*/
}
//---------------------------------------------------------------------------

void __fastcall Thardware::FormShow(TObject *Sender)
{
/*
        FloatingPointHardwareFix->Enabled = false;

        if (ZX80Btn->Down || ZX81Btn->Down || Spec16Btn->Down || Spec48Btn->Down || Spec128Btn->Down || QLBtn->Down)
        {
                Machine->ActivePage=Sinclair;
                FloatingPointHardwareFix->Enabled = ZX81Btn->Down;
        }
        else if (SpecP2Btn->Down || SpecP2aBtn->Down || SpecP3Btn->Down)
        {
                Machine->ActivePage=Amstrad;
        }
        else if (TS1000Btn->Down || TS1500Btn->Down  || TC2048Btn->Down || TS2068Btn->Down)
        {
                Machine->ActivePage=Timex;
        }
        else if (ZX97LEBtn->Down || SpecSEBtn->Down)
        {
                Machine->ActivePage=HomeBrew;
        }
        else
        {
                Machine->ActivePage=Others;
        }

        ResetRequired=false;
*/                
}
//---------------------------------------------------------------------------

void Thardware::SaveSettings(TIniFile *ini)
{
        ini->WriteInteger("HWARE","Top",Top);
        ini->WriteInteger("HWARE","Left",Left);
        ini->WriteBool("HWARE","COLECO",RadioButton1->Checked);
        ini->WriteBool("HWARE","ADAM",RadioButton2->Checked);

        ini->WriteBool("HWARE","NTSC",chkNTSC->Checked);

        ini->WriteBool("HWARE","SGM",chkSGM->Checked);
}

void Thardware::LoadSettings(TIniFile *ini)
{
        Top=ini->ReadInteger("HWARE","Top",Top);
        Left=ini->ReadInteger("HWARE","Left",Left);
        if (ini->ReadBool("HWARE","COLECO",RadioButton1->Checked)) RadioButton1Click(NULL);
        if (ini->ReadBool("HWARE","ADAM",RadioButton2->Checked)) RadioButton2Click(NULL);
        if (ini->ReadBool("HWARE","NTSC",chkNTSC->Checked)) chkNTSCClick(NULL);
        if (ini->ReadBool("HWARE","SGM",chkSGM->Checked)) chkNTSCClick(NULL);
}

//---------------------------------------------------------------------------

void __fastcall Thardware::RadioButton1Click(TObject *Sender)
{
        NewMachineName = RadioButton1->Caption;
        NewMachine = MACHINECOLECO;
        ResetRequired=true;
}
//---------------------------------------------------------------------------

void __fastcall Thardware::RadioButton2Click(TObject *Sender)
{
        NewMachineName = RadioButton2->Caption;
        NewMachine = MACHINEADAM;
        ResetRequired=true;
}
//---------------------------------------------------------------------------

void __fastcall Thardware::chkNTSCClick(TObject *Sender)
{
        ResetRequired=true;
}
//---------------------------------------------------------------------------

