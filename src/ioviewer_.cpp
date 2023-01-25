//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ioviewer_.h"

#include "main_.h"
#include "coleco.h"
#include "colecoconfig.h"
#include "tms9928a.h"
#include "ay8910.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tiomapviewer *iomapviewer;
//---------------------------------------------------------------------------

__fastcall Tiomapviewer::Tiomapviewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    ini = new TIniFile(emul2.inipath);
    LoadSettings(ini);
    delete ini;
}
//---------------------------------------------------------------------------

void __fastcall Tiomapviewer::UpdateChanges()
{
    AnsiString texS;


    // Keypads
    eJoyP0->Caption=IntToHex((unsigned short) (coleco_joystat& 0x0000FFFF),4);
    eJoyP1->Caption=IntToHex((unsigned short) ((coleco_joystat & 0xFFFF0000)>>16),4);
    eJoyP0E->Caption="";
    eJoyP1E->Caption="";

    // Spinner
    eSpinP0->Caption=IntToHex(RAM_Memory[0x73EB],4);
    eSpinP1->Caption=IntToHex(RAM_Memory[0x73EC],4);

    // Memory
    eP20->Caption="$"+IntToHex(coleco_port20,2);
    eP53->Caption="$"+IntToHex(coleco_port53,2);
    texS=coleco_port53 & 0x01 ? "SGM enable" : "SGM disable";
    texS+=coleco_port60 & 0x02 ? ", BIOS enable" : ", RAM enable";
    eP53exp->Caption=texS;
    eP60->Caption="$"+IntToHex(coleco_port60,2);
    texS="--";
    if ((coleco_port60 & 0x03)==0x00)
        texS="Low 32K ROM";
    else if ((coleco_port60 & 0x03)==0x01)
        texS="Low 32K RAM";
    else if ((coleco_port60 & 0x03)==0x03)
        texS="Low 32K BIOS + RAM";
    if ((coleco_port60 & 0x0C) == 0x00)
        texS+=", High 32K RAM";
    else if ((coleco_port60 & 0x0C) == 0x08)
        texS+=", Expanded 128K RAM";
    eP60exp->Caption=texS;
    eBank->Caption="$"+IntToHex(coleco_megabank,2);
    eSGMRAM->Caption=emul2.SGM ? (coleco_port60 & 0x02 ? "24K" :"32K") : "--";

    // PSG for SGM
    ePSGRL->Caption="$"+IntToHex(ay.register_latch,2);
    ePSGR0->Caption="$"+IntToHex(ay.Regs[0],2); ePSGR1->Caption="$"+IntToHex(ay.Regs[1],2);
    ePSGR2->Caption="$"+IntToHex(ay.Regs[2],2); ePSGR3->Caption="$"+IntToHex(ay.Regs[3],2);
    ePSGR4->Caption="$"+IntToHex(ay.Regs[4],2); ePSGR5->Caption="$"+IntToHex(ay.Regs[5],2);
    ePSGR6->Caption="$"+IntToHex(ay.Regs[6],2); ePSGR7->Caption="$"+IntToHex(ay.Regs[7],2);
    ePSGR8->Caption="$"+IntToHex(ay.Regs[8],2); ePSGR9->Caption="$"+IntToHex(ay.Regs[9],2);
    ePSGRA->Caption="$"+IntToHex(ay.Regs[10],2); ePSGRB->Caption="$"+IntToHex(ay.Regs[11],2);
    ePSGRC->Caption="$"+IntToHex(ay.Regs[12],2); ePSGRD->Caption="$"+IntToHex(ay.Regs[13],2);
    ePSGRE->Caption="$"+IntToHex(ay.Regs[14],2); ePSGRF->Caption="$"+IntToHex(ay.Regs[15],2);
}
//---------------------------------------------------------------------------

void __fastcall Tiomapviewer::do_refresh()
{
    if (AutoRefresh1->Checked) {
        UpdateChanges();
    }
}

//---------------------------------------------------------------------------
void __fastcall Tiomapviewer::ExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall Tiomapviewer::Refresh1Click(TObject *Sender)
{
    UpdateChanges();
}
//---------------------------------------------------------------------------
void __fastcall Tiomapviewer::AutoRefresh1Click(TObject *Sender)
{
    if (AutoRefresh1->Enabled)
    {
        AutoRefresh1->Checked=!AutoRefresh1->Checked;
        if (AutoRefresh1->Checked)
        {
            UpdateChanges();
        }
    }    
}
//---------------------------------------------------------------------------
void __fastcall Tiomapviewer::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Form1->IOMapViewer1->Checked=false;
}
//---------------------------------------------------------------------------
void __fastcall Tiomapviewer::FormShow(TObject *Sender)
{
    AutoRefresh1->Enabled = true;
    UpdateChanges();
}
//---------------------------------------------------------------------------
void Tiomapviewer::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("IOMAPVIEW","Top",Top);
    Left = ini->ReadInteger("IOMAPVIEW","Left",Left);
    AutoRefresh1->Checked = ini->ReadBool("IOMAPVIEW", "AutoRefresh", true);
}

void Tiomapviewer::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("IOMAPVIEW","Top",Top);
    ini->WriteInteger("IOMAPVIEW","Left",Left);
    ini->WriteBool("IOMAPVIEW", "AutoRefresh", AutoRefresh1->Checked);
}
