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
        maccol->Down=true;

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

        strcpy(coleco.machinename, Name.c_str());
        Form1->StatusBar1->Panels->Items[0]->Text = Name;

        coleco.machine=NewMachine;

        coleco.NTSC = chkNTSC->Checked;
        if (coleco.NTSC) Form1->AnimTimer1->Interval=17;
        else Form1->AnimTimer1->Interval=20;

        coleco.SGM = spSGM->Down ? 1 : 0;
        coleco.F18A = spF18A->Down ? 1 : 0;

        machine.clockspeed=coleco.NTSC ? CLOCK_NTSC : CLOCK_PAL;
        machine.tperscanline=coleco.NTSC ? TMS9918_LINE : TMS9929_LINE;
        machine.scanlines=coleco.NTSC ? TMS9918_LINES : TMS9929_LINES;
        machine.tperframe = machine.tperscanline * machine.scanlines;

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

        if (ResetRequired) machine.initialise();

        if (Sender) Close();
}
//---------------------------------------------------------------------------

void __fastcall Thardware::FormShow(TObject *Sender)
{
    ResetRequired=false;

    // Value that can be changed during loading phase
    //cboSEBACK->ItemIndex=machine.typebackup;
}
//---------------------------------------------------------------------------

void Thardware::SaveSettings(TIniFile *ini)
{
        if (maccol->Down)
                ini->WriteInteger("HWARE","MACHINE",MACHINECOLECO);
        else if (macpho->Down)
                ini->WriteInteger("HWARE","MACHINE",MACHINEPHOENIX);
        else
                ini->WriteInteger("HWARE","MACHINE",MACHINEADAM);

        ini->WriteInteger("HWARE","Top",Top);
        ini->WriteInteger("HWARE","Left",Left);

        ini->WriteBool("HWARE","NTSC",chkNTSC->Checked);

        ini->WriteBool("HWARE","F18A",spF18A->Down);
        ini->WriteInteger("HWARE","SGM",spSGM->Down);
}

void Thardware::LoadSettings(TIniFile *ini)
{
        NewMachine=0;
        NewMachine=ini->ReadInteger("HWARE","MACHINE",NewMachine);
        switch (NewMachine) {
                case MACHINECOLECO: maccol->Down=true; break;
                case MACHINEPHOENIX: macpho->Down=true; break;
                case MACHINEADAM: macadam->Down=true; break;
        }

        Top=ini->ReadInteger("HWARE","Top",Top);
        Left=ini->ReadInteger("HWARE","Left",Left);

        if (ini->ReadBool("HWARE","NTSC",chkNTSC->Checked)) chkNTSCClick(NULL);

        spF18A->Down=ini->ReadBool("HWARE","F18A",spF18A->Down);
        spSGM->Down=ini->ReadBool("HWARE","SGM",spSGM->Down);
}

//---------------------------------------------------------------------------

void __fastcall Thardware::chkNTSCClick(TObject *Sender)
{
        ResetRequired=true;
}
//---------------------------------------------------------------------------

void __fastcall Thardware::maccolClick(TObject *Sender)
{
    //tSCExpMod->Enabled=true;
    //gSERomSet->Enabled=true;
    NewMachine=MACHINECOLECO;
    ResetRequired=true;

}
//---------------------------------------------------------------------------

void __fastcall Thardware::macphoClick(TObject *Sender)
{
    //spSGM->Down = true;
    //spF18A->Down = true;
    //tSCExpMod->Enabled=false;
    //gSERomSet->Enabled=true;
    NewMachine=MACHINEPHOENIX;
    ResetRequired=true;
}
//---------------------------------------------------------------------------

void __fastcall Thardware::macadamClick(TObject *Sender)
{
    //spSGM->Down = false;
    //spF18A->Down = false;
    //tSCExpMod->Enabled=false;
    //gSERomSet->Enabled=false;
    //cboSERo->ItemIndex=0;
    //cboSEBACK->ItemIndex=0;
    NewMachine=MACHINEADAM;
    ResetRequired=true;
}
//---------------------------------------------------------------------------

void __fastcall Thardware::spSGMClick(TObject *Sender)
{
    ResetRequired=true;
}
//---------------------------------------------------------------------------

