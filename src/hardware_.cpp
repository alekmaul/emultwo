//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "hardware_.h"

#include "main_.h"

#include "accdraw_.h"
#include "accsound_.h"
#include "coleco.h"

#include "f18a.h"
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

    // Init default value before reading ini file
    maccol->Down=true;
    cSEPalNtsc->ItemIndex=0;
    cboPal->ItemIndex=0;
    cboSEDispD->ItemIndex=0;

    cSEStart->Checked=false;
    cSENodelay->Checked=false;
    cSEPFreq->Checked=false;


    ini = new TIniFile(emul2.inipath);
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
//    dirtyBird.insert(address);
}

void __fastcall Thardware::OKClick(TObject *Sender)
{
    AnsiString Name=NewMachineName;

    strcpy(emul2.machinename, Name.c_str());
    Form1->StatusBar1->Panels->Items[0]->Text = Name;

    emul2.machine=NewMachine;

    emul2.NTSC = 1-cSEPalNtsc->ItemIndex;
    machine.fps = emul2.NTSC ? 60 : 50;

    emul2.palette=cboPal->ItemIndex;

    emul2.hackbiospal=cSEPFreq->Checked ? 1 : 0;;
    emul2.biosnodelay=cSENodelay->Checked ? 1 : 0;
    emul2.startdebug=cSEStart->Checked ? 1 : 0;

    emul2.SGM = spSGM->Down ? 1 : 0;
    emul2.F18A = spF18A->Down ? 1 : 0;

    emul2.steerwheel = coSteWhe->Down ? 1 : 0;
    emul2.rollercontrol = coRolCon->Down ? 1 : 0;
    emul2.superaction = coSupAct->Down ? 1 : 0;

    strcpy(emul2.colecobios,stBios->Caption.c_str());
    strcpy(emul2.adameos,stEos->Caption.c_str());
    strcpy(emul2.adamwriter,stWriter->Caption.c_str());

    machine.clockspeed=emul2.NTSC ? CLOCK_NTSC : CLOCK_PAL;
    machine.tperscanline=emul2.NTSC ? TMS9918_LINE : TMS9929_LINE;
    machine.scanlines=emul2.NTSC ? TMS9918_LINES : TMS9929_LINES;
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
    machine.vdp_reset=(emul2.F18A ? f18a_reset : tms9918_reset);
    machine.vdp_writedata=(emul2.F18A ? f18a_writedata : tms9918_writedata);
    machine.vdp_readdata=(emul2.F18A ? f18a_readdata : tms9918_readdata);
    machine.vdp_writectrl=(emul2.F18A ? f18a_writectrl : tms9918_writectrl);
    machine.vdp_readctrl=(emul2.F18A ? f18a_readctrl : tms9918_readctrl);
    machine.vdp_loop=(emul2.F18A ? f18a_loop : tms9918_loop);


    TVW=TVW_TMS;
    TVH=TVH_TMS;

    AccurateInit(true);

    if (ResetRequired) machine.initialise();

    Sound.ReInitialise(NULL, machine.fps,0,0,0);

    if (Sender) Close();
}
//---------------------------------------------------------------------------

void __fastcall Thardware::FormShow(TObject *Sender)
{
    ResetRequired=false;

    // Value that can be changed during loading phase
    if (emul2.machine==MACHINECOLECO)
    {
        maccol->Down=true;
    }
    else if (emul2.machine==MACHINEADAM)
    {
        macadam->Down=true;
    }
    else
        macpho->Down=true;
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

    ini->WriteInteger("HWARE","NTSC",cSEPalNtsc->ItemIndex);

    ini->WriteInteger("HWARE","PALETTE",cboPal->ItemIndex);
    ini->WriteInteger("HWARE","DSPDRV",cboSEDispD->ItemIndex);

    ini->WriteInteger("HWARE","HACKPAL",cSEPFreq->Checked);
    ini->WriteInteger("HWARE","HACKDLAY",cSENodelay->Checked);
    ini->WriteInteger("HWARE","DEBSTART",cSEStart->Checked);


    ini->WriteBool("HWARE","F18A",spF18A->Down);
    ini->WriteBool("HWARE","SGM",spSGM->Down);

    ini->WriteBool("HWARE","STEWHE",coSteWhe->Down);
    ini->WriteBool("HWARE","ROLCON",coRolCon->Down);
    ini->WriteBool("HWARE","SUPACT",coSupAct->Down);

    ini->WriteString("HWARE","BIOSCOLECO",stBios->Caption);
    ini->WriteString("HWARE","BIOSEOS",stEos->Caption);
    ini->WriteString("HWARE","BIOSWRITER",stWriter->Caption);
}

void Thardware::LoadSettings(TIniFile *ini)
{
    NewMachine=0;
    NewMachine=ini->ReadInteger("HWARE","MACHINE",NewMachine);
    switch (NewMachine)
    {
        case MACHINECOLECO: maccol->Down=true; break;
        case MACHINEPHOENIX: macpho->Down=true; break;
        case MACHINEADAM: macadam->Down=true; break;
    }

    Top=ini->ReadInteger("HWARE","Top",Top);
    Left=ini->ReadInteger("HWARE","Left",Left);

    cSEPalNtsc->ItemIndex=ini->ReadInteger("HWARE","NTSC",cSEPalNtsc->ItemIndex);
    cboPal->ItemIndex=ini->ReadInteger("HWARE","PALETTE",cboPal->ItemIndex);
    cboSEDispD->ItemIndex=ini->ReadInteger("HWARE","DSPDRV",cboSEDispD->ItemIndex);

    cSEPFreq->Checked=ini->ReadInteger("HWARE","HACKPAL",cSEPFreq->Checked);
    cSENodelay->Checked=ini->ReadInteger("HWARE","HACKPAL",cSENodelay->Checked);
    cSEStart->Checked=ini->ReadInteger("HWARE","DEBSTART",cSEStart->Checked);

    spF18A->Down=ini->ReadBool("HWARE","F18A",spF18A->Down);
    spSGM->Down=ini->ReadBool("HWARE","SGM",spSGM->Down);

    coSteWhe->Down=ini->ReadBool("HWARE","STEWHE",coSteWhe->Down);
    coRolCon->Down=ini->ReadBool("HWARE","ROLCON",coRolCon->Down);
    coSupAct->Down=ini->ReadBool("HWARE","SUPACT",coSupAct->Down);

    stBios->Caption=ini->ReadString("HWARE","BIOSCOLECO",stBios->Caption);
    stEos->Caption=ini->ReadString("HWARE","BIOSEOS",stEos->Caption);
    stWriter->Caption=ini->ReadString("HWARE","BIOSWRITER",stWriter->Caption);
}
//---------------------------------------------------------------------------

void __fastcall Thardware::chkNTSCClick(TObject *Sender)
{
    ResetRequired=true;
}
//---------------------------------------------------------------------------

void __fastcall Thardware::maccolClick(TObject *Sender)
{
    tSCExpMod->Enabled=true;
    //gSERomSet->Enabled=true;
    NewMachine=MACHINECOLECO;
    ResetRequired=true;

}
//---------------------------------------------------------------------------

void __fastcall Thardware::macphoClick(TObject *Sender)
{
    spSGM->Down = true;
    spF18A->Down = true;
    tSCExpMod->Enabled=false;
    //gSERomSet->Enabled=true;
    NewMachine=MACHINEPHOENIX;
    ResetRequired=true;
}
//---------------------------------------------------------------------------

void __fastcall Thardware::macadamClick(TObject *Sender)
{
    spSGM->Down = false;
    spF18A->Down = false;
    tSCExpMod->Enabled=true;
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

void __fastcall Thardware::cboPalChange(TObject *Sender)
{
    pbox->Refresh();
    coleco_setpalette(cboPal->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall Thardware::pboxPaint(TObject *Sender)
{
    int r,g,b;
    int xs = pbox->Width/16;
    int idxpal=cboPal->ItemIndex*3*16;

    for (int i=0;i<16;i++)
    {
        r=TMS9918A_palette[idxpal+i*3];g=TMS9918A_palette[idxpal+i*3+1];b=TMS9918A_palette[idxpal+i*3+2];
        pbox->Canvas->Brush->Style = bsSolid;
        pbox->Canvas->Brush->Color = static_cast<TColor>(RGB(r,g,b)); 
        pbox->Canvas->Rectangle(i*xs,0,xs*i+xs,pbox->Height);
    }
}
//---------------------------------------------------------------------------

void __fastcall Thardware::bCancelClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

void __fastcall Thardware::cboSEDispDChange(TObject *Sender)
{
    int NewRenderMode;

    if (cboSEDispD->ItemIndex==0) NewRenderMode=RENDERGDI;
    else NewRenderMode=RENDERDDRAW;

    if (Form1->RenderMode!=NewRenderMode)
    {
        RenderEnd();
        Form1->RenderMode=NewRenderMode;
        RenderInit();
        AccurateInit(true);
    }
}
//---------------------------------------------------------------------------

void __fastcall Thardware::bBiosClick(TObject *Sender)
{
    TButton *b;

    try
    {
        odBIOS->DefaultExt="rom";
        odBIOS->FileName="*";
        odBIOS->Filter="Bios Files (*.ROM,*.BIN)|*.rom;*.bin";
        if (odBIOS->Execute())
        {
            b = (TButton *) Sender;
            if (b->Tag==0)
            {
                stBios->Caption=odBIOS->FileName;
                strcpy(emul2.colecobios,stBios->Caption.c_str());
            }
            else if (b->Tag==1)
            {
                stEos->Caption=odBIOS->FileName;
                strcpy(emul2.adameos,stEos->Caption.c_str());

            }
            else
            {
                stWriter->Caption=odBIOS->FileName;
                strcpy(emul2.adamwriter,stWriter->Caption.c_str());
            }
            ResetRequired=true;
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

void __fastcall Thardware::Button1Click(TObject *Sender)
{
    strcpy(emul2.colecobios,"Internal");
    strcpy(emul2.adameos,"Internal");
    strcpy(emul2.adamwriter,"Internal");
    stBios->Caption=emul2.colecobios;
    stEos->Caption=emul2.adameos;
    stWriter->Caption=emul2.adamwriter;
}
//---------------------------------------------------------------------------

