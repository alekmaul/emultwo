//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "dumpmem_.h"

#include "coleco.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDumpMem *DumpMem;
//---------------------------------------------------------------------------
__fastcall TDumpMem::TDumpMem(TComponent* Owner)
    : TForm(Owner)
{
}

//---------------------------------------------------------------------------
int TDumpMem::get_memvalue(int adrmemory)
{
    int by=0;
    switch (typememory) {
        case 0 : by=getbyte(adrmemory); break; // ROM
        case 1 : by=coleco_gettmsval(VRAM,adrmemory,0,0); break; // VRAM
        case 2 : by=coleco_gettmsval(RAM,adrmemory,0,0); break; // RAM
        case 3 : by=coleco_gettmsval(SGMRAM,adrmemory,0,0); break; // SGM RAM
    }

    return by;
}

//---------------------------------------------------------------------------
// returns true or false depending on whether the gathering of a value was
// succesful. The data is both passed in and returned in value. This is
// unchanged in the case when gathering fails.
//
void TDumpMem::Dump2(int address, int maxval, int typmem)
{
        AnsiString NewVal;

        maxValue = maxval;
        typememory=typmem;
        curAddress = address;
        EditVal->Text = 32;

        ActiveControl = memdump;
        ShowModal();
}

bool TDumpMem::Translate(AnsiString text, int& value)
{
    AnsiString result = text.Trim();

    if (result.Length() == 0)
    {
        return false;
    }

    char* endPtr;
    int tval;
    tval = int(strtol(result.c_str(), &endPtr, 10));

    if (*endPtr != 0)
    {
        // and we know when the user entered junk...
        return false;
    }

    value = tval;

    return (value <= maxValue);
}

//---------------------------------------------------------------------------
void __fastcall TDumpMem::EditValKeyPress(TObject *Sender, char &Key)
{
    if (Key == VK_ESCAPE)
    {
        Close();
    }
}
//---------------------------------------------------------------------------

void __fastcall TDumpMem::EditValChange(TObject *Sender)
{
    AnsiString bytesn;
    int value,i,j;
    byte Data;
    bool valid = Translate(EditVal->Text, value);

    EditVal->Font->Color = valid ? clWindowText : clRed;
    if (valid) {
        memdump->Clear();
        for (i=curAddress;(i-curAddress)<value;i+=16)
        {
            bytesn="";
            for (j=0;j<16 && ((j+i-curAddress)<value);j++)
            {
                Data=get_memvalue(i+j);
                bytesn+="0x"+IntToHex(Data,2)+",";
            }
            memdump->Lines->Add(bytesn);
        }
    }
}
//---------------------------------------------------------------------------
void TDumpMem::CentreOn(TForm* parent)
{
    Top = parent->Top + (parent->Height - Height) /2;
    Left = parent->Left + (parent->Width - Width) /2;
}
void __fastcall TDumpMem::OKClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TDumpMem::SaveClick(TObject *Sender)
{
    memdump->SelectAll();
    memdump->CopyToClipboard();
    Application->MessageBox("Memory copied to clipboard","Information", MB_OK);
}
//---------------------------------------------------------------------------
