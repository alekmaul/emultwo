//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "hardware_.h"

#include "main_.h"

#include "accdraw_.h"
#include "accsound_.h"
#include "coleco.h"

#include "z80.h"
#include "f18a.h"
#include "tms9928a.h"

#include "symbolstore.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Thardware *hardware;

static const char* os7Symbols =
{
    "00001F61  PLAY_SONGS\n"
    "00001F64  ACTIVATEP\n"
    "00001F67  PUTOBJP\n"
    "00001F6A  REFLECT_VERTICAL\n"
    "00001F6D  REFLECT_HORIZONTAL\n"
    "00001F70  ROTATE_90\n"
    "00001F73  ENLARGE\n"
    "00001F76  CONTROLLER_SCAN\n"
    "00001F79  DECODER\n"
    "00001F7C  GAME_OPT\n"
    "00001F7F  LOAD_ASCII\n"
    "00001F82  FILL_VRAM\n"
    "00001F85  MODE_1\n"
    "00001F88  UPDATE_SPINNER\n"
    "00001F8B  INIT_TABLEP\n"
    "00001F8E  GET_VRAMP\n"
    "00001F91  PUT_VRAMP\n"
    "00001F94  INIT_SPR_ORDERP\n"
    "00001F97  WR_SPR_NM_TBLP\n"
    "00001F9A  INIT_TIMERP\n"
    "00001F9D  FREE_SIGNALP\n"
    "00001FA0  REQUEST_SIGNALP\n"
    "00001FA3  TEST_SIGNALP\n"
    "00001FA6  WRITE_REGISTERP\n"
    "00001FA9  WRITE_VRAMP\n"
    "00001FAC  READ_VRAMP\n"
    "00001FAF  INIT_WRITERP\n"
    "00001FB2  SOUND_INITP\n"
    "00001FB5  PLAY_ITP\n"
    "00001FB8  INIT_TABLE\n"
    "00001FBB  GET_VRAM\n"
    "00001FBE  PUT_VRAM\n"
    "00001FC1  INIT_SPR_ORDER\n"
    "00001FC4  WR_SPR_NM_TBL\n"
    "00001FC7  INIT_TIMER\n"
    "00001FCA  FREE_SIGNAL\n"
    "00001FCD  REQUEST_SIGNAL\n"
    "00001FD0  TEST_SIGNAL\n"
    "00001FD3  TIME_MGR\n"
    "00001FD6  TURN_OFF_SOUND\n"
    "00001FD9  WRITE_REGISTER\n"
    "00001FDC  READ_REGISTER\n"
    "00001FDF  WRITE_VRAM\n"
    "00001FE2  READ_VRAM\n"
    "00001FE5  INIT_WRITER\n"
    "00001FE8  WRITER\n"
    "00001FEB  POLLER\n"
    "00001FEE  SOUND_INIT\n"
    "00001FF1  PLAY_IT\n"
    "00001FF4  SOUND_MAN\n"
    "00001FF7  ACTIVATE\n"
    "00001FFA  PUTOBJ\n"
    "00001FFD  RAND_GEN\n"

    /*
    "00000000  BOOT_UP\n"
    "00000069  AMERICA\n"
    "0000006A  ASCII_TABLE\n"
    "0000006C  NUMBER_TABLE\n"
    "0000006E  POWER_UP\n"
    "000000FC  FREQ_SWEEP\n"
    "0000012F  ATN_SWEEP\n"
    "00000190  DECLSN\n"
    "0000019B  DECMSN\n"
    "000001A6  MSNTOLSN\n"
    "000001B1  ADD816\n"
    "000001D5  LEAVE_EFFECT\n"
    "000002D6  PROCESS_DATA_AREA\n"
    "000002EE  EFXOVER\n"
    "0000080B  PUT_FRAME\n"
    "00000898  GET_BKGRND\n"
    "000008C0  CALC_OFFSET\n"
    "00001D43  CTRL_PORT_PTR\n"
    "00001D47  DATA_PORT_PTR\n"
    "00001D6C  ENLRG\n"
    "000073B9  STACK\n"
    "000073C3  VDP_MODE_WORD\n"
    "000073C5  VDP_STATUS_BYTE\n"
    "000073C6  DEFER_WRITES\n"
    "000073C7  MUX_SPRITES\n"
    "000073C9  RAND_NUM\n"

    "00008000  CARTRIDGE\n"
    "00008002  LOCAL_SPR_TABLE\n"
    "00008004  SPRITE_ORDER\n"
    "00008006  WORK_BUFFER\n"
    "00008008  CONTROLLER_MAP\n"
    "0000800A  START_GAME\n"
    "0000800C  RST_8H_RAM\n"
    "0000800F  RST_10H_RAM\n"
    "00008012  RST_18H_RAM\n"
    "00008015  RST_20H_RAM\n"
    "00008018  RST_28H_RAM\n"
    "0000801B  RST_30H_RAM\n"
    "0000801E  IRQ_INT_VECT\n"
    "00008021  NMI_INT_VECT\n"
    "00008024  GAME_NAME"
*/
};

void caserMunger(AnsiString& sym, AnsiString& val)
{
        sym = UpperCase(sym);
}

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
    machine.interrupt = INPUT_LINE_NMI;
    machine.exit = NULL;
    machine.vdp_reset=(emul2.F18A ? f18a_reset : tms9918_reset);
    machine.vdp_writedata=(emul2.F18A ? f18a_writedata : tms9918_writedata);
    machine.vdp_readdata=(emul2.F18A ? f18a_readdata : tms9918_readdata);
    machine.vdp_writectrl=(emul2.F18A ? f18a_writectrl : tms9918_writectrl);
    machine.vdp_readctrl=(emul2.F18A ? f18a_readctrl : tms9918_readctrl);
    machine.vdp_loop=(emul2.F18A ? f18a_loop : tms9918_loop);
    symbolstore::loadROMSymbols(os7Symbols, caserMunger);

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

