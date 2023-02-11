/* EmulTwo  - A Windows Colecovision emulator.
 * Copyright (C) 2018-2023 Alekmaul
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
 *
 * iovdpviewer_.cpp
 *
 */
#include <vcl.h>
#pragma hdrstop

#include "iovdpviewer_.h"

#include "colecoconfig.h"
#include "main_.h"
#include "tms9928a.h"
#include "f18a.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tiovdpviewer *iovdpviewer;
//---------------------------------------------------------------------------
__fastcall Tiovdpviewer::Tiovdpviewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    ini = new TIniFile(emul2.inipath);
    LoadSettings(ini);
    delete ini;
}
//---------------------------------------------------------------------------

void Tiovdpviewer::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("IOVDPVIEW","Top",Top);
    Left = ini->ReadInteger("IOVDPVIEW","Left",Left);

    AutoRefresh1->Checked = ini->ReadBool("IOVDPVIEW", "AutoRefresh", true);
}
//---------------------------------------------------------------------------

void Tiovdpviewer::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("IOVDPVIEW","Top",Top);
    ini->WriteInteger("IOVDPVIEW","Left",Left);
    ini->WriteBool("IOVDPVIEW", "AutoRefresh", AutoRefresh1->Checked);
}
//---------------------------------------------------------------------------

void __fastcall Tiovdpviewer::ExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall Tiovdpviewer::UpdateChanges()
{
    AnsiString texS;
    int index;

    // Put VDP information
    // Mode and others features
    switch(tms.Mode)
    {
        case 0:eVDPMode->Caption="Gfx1 Mode 0 40x24"; break;
        case 1:eVDPMode->Caption="Txt1 Mode 1 32x24"; break;
        case 2:eVDPMode->Caption="Gfx2 Mode 2 32x24"; break;
        case 3:eVDPMode->Caption="Mult Mode 3 64x48x16"; break;
    }
    switch (tms.VR[1] & 0x3)
    {
        case 0:eVDPOption->Caption="8x8 not magnified"; break;
        case 1:eVDPOption->Caption="8x8 magnified"; break;
        case 2:eVDPOption->Caption="16x16 not magnified"; break;
        case 3:eVDPOption->Caption="16x16 magnified"; break;
    }

    // Address
    eVDPBGmap->Caption="$"+IntToHex(coleco_gettmsaddr(CHRMAP,0,0),4)+"-$"+IntToHex(coleco_gettmsaddr(CHRMAP,0,0)+32*24-1,4);
    eVDPBGtile->Caption="$"+IntToHex(coleco_gettmsaddr(CHRGEN,0,0),4);
    eVDPBGcolor->Caption="$"+IntToHex(coleco_gettmsaddr(CHRCOL,0,0),4)+"-$"+IntToHex(coleco_gettmsaddr(CHRCOL,0,0)+32*24-1,4);
    eVDPSPRdata->Caption="$"+IntToHex(coleco_gettmsaddr(SPRATTR,0,0),4);
    eVDPSPRtile->Caption="$"+IntToHex(coleco_gettmsaddr(SPRGEN,0,0),4);

    // Register
    for (index = 0; index < gVDP->ControlCount; index ++)
    {
        if(gVDP->Controls[index]->InheritsFrom(__classid(TStaticText)))
        {
            TStaticText *ptxt = (TStaticText*) gVDP->Controls[index];
            ptxt->Caption="$"+IntToHex(tms.VR[ptxt->Tag],2);
        }
    }
/*
    eVDPR0->Caption="$"+IntToHex(tms.VR[0],2); eVDPR1->Caption="$"+IntToHex(tms.VR[1],2);
    eVDPR2->Caption="$"+IntToHex(tms.VR[2],2); eVDPR3->Caption="$"+IntToHex(tms.VR[3],2);
    eVDPR4->Caption="$"+IntToHex(tms.VR[4],2); eVDPR5->Caption="$"+IntToHex(tms.VR[5],2);
    eVDPR6->Caption="$"+IntToHex(tms.VR[6],2); eVDPR7->Caption="$"+IntToHex(tms.VR[7],2);
    */
    if (emul2.F18A)
    {
        gF18A->Enabled=true;
        for (index = 0; index < gF18A->ControlCount; index ++)
        {
            if(gF18A->Controls[index]->InheritsFrom(__classid(TStaticText)))
            {
                TStaticText *ptxt = (TStaticText*) gF18A->Controls[index];
                ptxt->Caption="$"+IntToHex(f18a.VDPR[ptxt->Tag],2);
            }
        }
    }
    eVDPStat->Caption="$"+IntToHex(tms.SR,2); eVDPlatch->Caption="$"+IntToHex(tms.DLatch,2);

    // Information
    texS="";
    if (tms.SR & 0x80) texS=texS+"F,";                                        
    if (tms.SR & 0x40) texS=texS+"5th S,";
    if (tms.SR & 0x20) texS=texS+"C,";
    texS=texS+"5thS#"+IntToHex(tms.SR&0x1F,2);
    eVDPStatus->Caption=texS;
    eVDPColF->Color=(TColor) cv_pal32[tms.VR[7]>>4];   eVDPColB->Color=(TColor) cv_pal32[tms.VR[7] & 0xF];

    texS=IntToStr((tms.VR[1]>>4) & 1)+" " +IntToStr((tms.VR[1]>>3) & 1)+" " +IntToStr((tms.VR[0]>>1) & 1);
    eVDPValMod->Caption=texS;
}
//---------------------------------------------------------------------------
void __fastcall Tiovdpviewer::do_refresh()
{
    if (AutoRefresh1->Checked) {
      UpdateChanges();
    }
}

//---------------------------------------------------------------------------

void __fastcall Tiovdpviewer::AutoRefresh1AdvancedDrawItem(TObject *Sender,
      TCanvas *ACanvas, TRect &ARect, TOwnerDrawState State)
{
    if (AutoRefresh1->Enabled)
    {
        AutoRefresh1->Checked=!AutoRefresh1->Checked;
        if (AutoRefresh1->Checked)
        {
            do_refresh();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall Tiovdpviewer::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Form1->IOVdpViewer1->Checked=false;
}
//---------------------------------------------------------------------------
