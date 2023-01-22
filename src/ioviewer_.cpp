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


    // Keypads
    eJoyP0->Caption=IntToHex((unsigned short) (coleco_joystat& 0x0000FFFF),4);
    eJoyP1->Caption=IntToHex((unsigned short) ((coleco_joystat & 0xFFFF0000)>>16),4);
    eJoyP0E->Caption="";
    eJoyP1E->Caption="";

    // Spinner
    eSpinP0->Caption=IntToHex(RAM_Memory[0x73EB],4);
    eSpinP1->Caption=IntToHex(RAM_Memory[0x73EC],4);

    // PSG for SGM
    ePSGR0->Text=IntToHex(ay.register_latch,2);
    ePSGR1->Text=IntToHex(ay.Regs[0],2); ePSGR2->Text=IntToHex(ay.Regs[1],2);
    ePSGR3->Text=IntToHex(ay.Regs[2],2); ePSGR4->Text=IntToHex(ay.Regs[3],2);
    ePSGR5->Text=IntToHex(ay.Regs[4],2); ePSGR6->Text=IntToHex(ay.Regs[5],2);
    ePSGR7->Text=IntToHex(ay.Regs[6],2); ePSGR8->Text=IntToHex(ay.Regs[7],2);
    ePSGR9->Text=IntToHex(ay.Regs[8],2); ePSGRA->Text=IntToHex(ay.Regs[9],2);
    ePSGRB->Text=IntToHex(ay.Regs[10],2); ePSGRC->Text=IntToHex(ay.Regs[11],2);
    ePSGRD->Text=IntToHex(ay.Regs[12],2); ePSGRE->Text=IntToHex(ay.Regs[13],2);
    ePSGRF->Text=IntToHex(ay.Regs[14],2); ePSGRG->Text=IntToHex(ay.Regs[15],2);
}

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
