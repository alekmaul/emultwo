//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "soundviewer_.h"

#include "main_.h"
#include "coleco.h"
#include "colecoconfig.h"
#include "sn76489.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tsoundviewer *soundviewer;
//---------------------------------------------------------------------------
__fastcall Tsoundviewer::Tsoundviewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    ini = new TIniFile(emul2.inipath);
    LoadSettings(ini);
    delete ini;

    nbayval=nbsnval=0;
    bytesn="";
    byteay="";
}

//---------------------------------------------------------------------------

void Tsoundviewer::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("SNDVIEW","Top",Top);
    Left = ini->ReadInteger("SNDVIEW","Left",Left);
    AutoRefresh1->Checked = ini->ReadBool("SNDVIEW", "AutoRefresh", true);
}

void Tsoundviewer::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("SNDVIEW","Top",Top);
    ini->WriteInteger("SNDVIEW","Left",Left);
    ini->WriteBool("SNDVIEW", "AutoRefresh", AutoRefresh1->Checked);
}

//---------------------------------------------------------------------------
void __fastcall Tsoundviewer::ExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall Tsoundviewer::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Form1->SoundViewer1->Checked=false;
}

//---------------------------------------------------------------------------
void __fastcall Tsoundviewer::addsnvalue(byte Data)
{
    // analyse data
    //    To control the Volume:
    //      |1 |R2|R1|R0|V3|V2|V1|V0|
    //      R2-R0 tell the register
    //      V3-V0 tell the volume:
    //      0000=Full volume
    //      1111=Silence
    if ( (Data & 0x8F) == 0x8F )
    {
    }
    else // ok log data
    {
        nbsnval++;
        if (nbsnval>7)
        {
            bytesn+="0x"+IntToHex(Data,2)+",";
            mSN76489->Lines->Add(bytesn);
            bytesn="";
            nbsnval=0;
        }
        else
        {
            bytesn+="0x"+IntToHex(Data,2)+",";
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall Tsoundviewer::addsnplay(byte Data)
{
    int coleco_sndtable=(RAM_Memory[0x7000+0x20]+256*RAM_Memory[0x7000+0x21]);

    // Add sound in view
    mSNPlay->Lines->Add("AddSound "+IntToHex(Data,2)+" "+IntToHex(getbyte(coleco_sndtable+Data*4)+256*getbyte(coleco_sndtable+Data*4+1),4));
}

//---------------------------------------------------------------------------
void __fastcall Tsoundviewer::mSN76489KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
     TShiftState TestState;
     TestState << ssCtrl;
     if (Key == 65 && Shift == TestState) {  // CTRL-A
        mSN76489->SelectAll();
     }
}

//---------------------------------------------------------------------------

void __fastcall Tsoundviewer::clearsnClick(TObject *Sender)
{
    mSN76489->Clear();
    mSNPlay->Clear();
    nbsnval=0;
    bytesn="";
}

//---------------------------------------------------------------------------
void __fastcall Tsoundviewer::UpdateChanges()
{
    // CV Address entries
    eMPA->Caption = IntToHex(RAM_Memory[0x7000+0x20]+256*RAM_Memory[0x7000+0x21],4);
    eNGP->Caption = IntToHex(RAM_Memory[0x7000+0x22]+256*RAM_Memory[0x7000+0x23],4);
    eT1GP->Caption = IntToHex(RAM_Memory[0x7000+0x24]+256*RAM_Memory[0x7000+0x25],4);
    eT2GP->Caption = IntToHex(RAM_Memory[0x7000+0x26]+256*RAM_Memory[0x7000+0x27],4);
    eT3GP->Caption = IntToHex(RAM_Memory[0x7000+0x28]+256*RAM_Memory[0x7000+0x29],4);
    eNCRS->Caption = IntToHex(RAM_Memory[0x7000+0x2A],2);

    // SN76489
    tsnRLatch->Caption=IntToHex(sn.LastRegister,2); tsnRRand->Caption=IntToHex(sn.RNG,4);
    tsnR0->Caption=IntToHex(sn.Register[0],3); tsnR1->Caption=IntToHex(sn.Register[1],3);
    tsnR2->Caption=IntToHex(sn.Register[2],3); tsnR3->Caption=IntToHex(sn.Register[3],3);
    tsnR4->Caption=IntToHex(sn.Register[4],3); tsnR5->Caption=IntToHex(sn.Register[5],3);
    tsnR6->Caption=IntToHex(sn.Register[6],3); tsnR7->Caption=IntToHex(sn.Register[7],3);
    pbTone0->Position=sn.Volume[0]*6;pbTone1->Position=sn.Volume[1]*6;
    pbTone2->Position=sn.Volume[2]*6;pbNoise->Position=sn.Volume[3]*6;
}

//---------------------------------------------------------------------------
void __fastcall Tsoundviewer::do_refresh()
{
    if (AutoRefresh1->Checked) {
        UpdateChanges();
    }
}

//---------------------------------------------------------------------------
void __fastcall Tsoundviewer::Refresh1Click(TObject *Sender)
{
    UpdateChanges();
}
//---------------------------------------------------------------------------

void __fastcall Tsoundviewer::AutoRefresh1Click(TObject *Sender)
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

void __fastcall Tsoundviewer::Button1Click(TObject *Sender)
{
    int i;
    int coleco_sndtable=(RAM_Memory[0x7000+0x20]+256*RAM_Memory[0x7000+0x21]);

    mSTContent->Clear();
    if (coleco_sndtable!=0xFFFF)
    {
        i=0;
        int firstsndarea=getbyte(coleco_sndtable+2+i*4)+256*getbyte(coleco_sndtable+3+i*4);
        while(1)
        {
            // Get curren sound area
            int sndarea=getbyte(coleco_sndtable+2+i*4)+256*getbyte(coleco_sndtable+3+i*4);
            int sndval=getbyte(coleco_sndtable+i*4)+256*getbyte(coleco_sndtable+1+i*4);
            // check if valid soundarea
            if ( (sndarea>0x73b8) || (sndarea<0x702b)) // 0x73b8=stack and 0x702b=beginning of ram
                break;
            // add entry to list
            mSTContent->Lines->Add(IntToHex(sndval,4)+" , SOUNDAREA"+((sndarea-firstsndarea)/10+1)+" // #"+(i+1) );

            // avoid infinite loop
            i++;
            if (i>64) break;
        }
    }
    else
        Application->MessageBox("No sound table address","Error",
                MB_OK | MB_ICONERROR);

}
//---------------------------------------------------------------------------

void __fastcall Tsoundviewer::Copytoclipboard1Click(TObject *Sender)
{
    mSN76489->SelectAll();
    mSN76489->CopyToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall Tsoundviewer::FlushSN1Click(TObject *Sender)
{
    mSN76489->Lines->Add(bytesn);
    bytesn="";
    nbsnval=0;
}
//---------------------------------------------------------------------------

