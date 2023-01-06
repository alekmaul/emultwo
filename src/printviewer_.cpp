//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "printviewer_.h"

#include "colecoconfig.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tprintviewer *printviewer;
//---------------------------------------------------------------------------
__fastcall Tprintviewer::Tprintviewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    ini = new TIniFile(coleco.inipath);
    LoadSettings(ini);
    delete ini;
}

//---------------------------------------------------------------------------

void Tprintviewer::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("PRNVIEW","Top",Top);
    Left = ini->ReadInteger("PRNVIEW","Left",Left);
}

void Tprintviewer::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("PRNVIEW","Top",Top);
    ini->WriteInteger("PRNVIEW","Left",Left);
}

//---------------------------------------------------------------------------
void Tprintviewer::SendPrint(byte Data)
{
           printdump->Text = printdump->Text+Data;
/*
    ActiveControl = printdump;
    Show();
*/
}
//---------------------------------------------------------------------------

void __fastcall Tprintviewer::clearprintClick(TObject *Sender)
{
    printdump->Clear();
}
//---------------------------------------------------------------------------

void __fastcall Tprintviewer::Copytoclipboard1Click(TObject *Sender)
{
    printdump->SelectAll();
    printdump->CopyToClipboard();
}
//---------------------------------------------------------------------------

