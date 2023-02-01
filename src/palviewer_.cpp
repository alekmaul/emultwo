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
 * palviewer_.cpp
 *
*/

#include <vcl.h>
#pragma hdrstop

#include "palviewer_.h"

#include "main_.h"
#include "coleco.h"
#include "colecoconfig.h"
#include "tms9928a.h"
#include "f18a.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tpaletteviewer *paletteviewer;
//---------------------------------------------------------------------------

__fastcall Tpaletteviewer::Tpaletteviewer(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    ini = new TIniFile(emul2.inipath);
    LoadSettings(ini);
    delete ini;
}

//---------------------------------------------------------------------------

void __fastcall Tpaletteviewer::ExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall Tpaletteviewer::FormClose(TObject *Sender, TCloseAction &Action)
{
    Form1->PaletteViewer1->Checked=false;
}


//---------------------------------------------------------------------------
void __fastcall Tpaletteviewer::ChangeCurrentColor(int ColorNum)
{
    SetTColor(paCurCol,cv_palette[ColorNum*3],cv_palette[ColorNum*3+1],cv_palette[ColorNum*3+2]);
    eCurColR->Caption = "$"+IntToHex(cv_palette[ColorNum*3],2);
    eCurColG->Caption = "$"+IntToHex(cv_palette[ColorNum*3+1],2);
    eCurColB->Caption = "$"+IntToHex(cv_palette[ColorNum*3+2],2);
    eCurColIdx->Caption = "$"+IntToHex(ColorNum,2);
}
//---------------------------------------------------------------------------

void __fastcall Tpaletteviewer::pCurCol0MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    int tag = -1;

    if (dynamic_cast < TComponent * > (Sender))
    {
        TComponent *comp = (TComponent *) Sender;
        tag = comp->Tag;
    }
    if(tag!=-1)
        ChangeCurrentColor(tag);
}
//---------------------------------------------------------------------------

void __fastcall Tpaletteviewer::SetTColor(TPaintBox *Sender, int r, int g, int b)
{
    Sender->Canvas->Brush->Color=TColor(RGB(r,g,b));
    Sender->Canvas->Rectangle(0, 0, Sender->Width, Sender->Height);
}
//---------------------------------------------------------------------------

void __fastcall Tpaletteviewer::UpdateChanges()
{
    // TMS9918 Colors
    SetTColor(paTCol1,cv_palette[0*3],cv_palette[0*3+1],cv_palette[0*3+2]);
    SetTColor(paTCol2,cv_palette[1*3],cv_palette[1*3+1],cv_palette[1*3+2]);
    SetTColor(paTCol3,cv_palette[2*3],cv_palette[2*3+1],cv_palette[2*3+2]);
    SetTColor(paTCol4,cv_palette[3*3],cv_palette[3*3+1],cv_palette[3*3+2]);
    SetTColor(paTCol5,cv_palette[4*3],cv_palette[4*3+1],cv_palette[4*3+2]);
    SetTColor(paTCol6,cv_palette[5*3],cv_palette[5*3+1],cv_palette[5*3+2]);
    SetTColor(paTCol7,cv_palette[6*3],cv_palette[6*3+1],cv_palette[6*3+2]);
    SetTColor(paTCol8,cv_palette[7*3],cv_palette[7*3+1],cv_palette[7*3+2]);
    SetTColor(paTCol9,cv_palette[8*3],cv_palette[8*3+1],cv_palette[8*3+2]);
    SetTColor(paTCol10,cv_palette[9*3],cv_palette[9*3+1],cv_palette[9*3+2]);
    SetTColor(paTCol11,cv_palette[10*3],cv_palette[10*3+1],cv_palette[10*3+2]);
    SetTColor(paTCol12,cv_palette[11*3],cv_palette[11*3+1],cv_palette[11*3+2]);
    SetTColor(paTCol13,cv_palette[12*3],cv_palette[12*3+1],cv_palette[12*3+2]);
    SetTColor(paTCol14,cv_palette[13*3],cv_palette[13*3+1],cv_palette[13*3+2]);
    SetTColor(paTCol15,cv_palette[14*3],cv_palette[14*3+1],cv_palette[14*3+2]);
    SetTColor(paTCol16,cv_palette[15*3],cv_palette[15*3+1],cv_palette[15*3+2]);

    // F18A Colors if enable
    if (emul2.F18A)
    {
        SetTColor(paFCol1,f18a.palette[0][0],f18a.palette[0][1],f18a.palette[0][2]);
        SetTColor(paFCol2,f18a.palette[1][0],f18a.palette[1][1],f18a.palette[1][2]);
        SetTColor(paFCol3,f18a.palette[2][0],f18a.palette[2][1],f18a.palette[2][2]);
        SetTColor(paFCol4,f18a.palette[3][0],f18a.palette[3][1],f18a.palette[3][2]);
        SetTColor(paFCol5,f18a.palette[4][0],f18a.palette[4][1],f18a.palette[4][2]);
        SetTColor(paFCol6,f18a.palette[5][0],f18a.palette[5][1],f18a.palette[5][2]);
        SetTColor(paFCol7,f18a.palette[6][0],f18a.palette[6][1],f18a.palette[6][2]);
        SetTColor(paFCol8,f18a.palette[7][0],f18a.palette[7][1],f18a.palette[7][2]);
        SetTColor(paFCol9,f18a.palette[8][0],f18a.palette[8][1],f18a.palette[8][2]);
        SetTColor(paFCol10,f18a.palette[9][0],f18a.palette[9][1],f18a.palette[9][2]);
        SetTColor(paFCol11,f18a.palette[10][0],f18a.palette[10][1],f18a.palette[10][2]);
        SetTColor(paFCol12,f18a.palette[11][0],f18a.palette[11][1],f18a.palette[11][2]);
        SetTColor(paFCol13,f18a.palette[12][0],f18a.palette[12][1],f18a.palette[12][2]);
        SetTColor(paFCol14,f18a.palette[13][0],f18a.palette[13][1],f18a.palette[13][2]);
        SetTColor(paFCol15,f18a.palette[14][0],f18a.palette[14][1],f18a.palette[14][2]);
        SetTColor(paFCol16,f18a.palette[15][0],f18a.palette[15][1],f18a.palette[15][2]);
        SetTColor(paFCol17,f18a.palette[16][0],f18a.palette[16][1],f18a.palette[16][2]);
        SetTColor(paFCol18,f18a.palette[17][0],f18a.palette[17][1],f18a.palette[17][2]);
        SetTColor(paFCol19,f18a.palette[18][0],f18a.palette[18][1],f18a.palette[18][2]);
        SetTColor(paFCol20,f18a.palette[19][0],f18a.palette[19][1],f18a.palette[19][2]);
        SetTColor(paFCol21,f18a.palette[20][0],f18a.palette[20][1],f18a.palette[20][2]);
        SetTColor(paFCol22,f18a.palette[21][0],f18a.palette[21][1],f18a.palette[21][2]);
        SetTColor(paFCol23,f18a.palette[22][0],f18a.palette[22][1],f18a.palette[22][2]);
        SetTColor(paFCol24,f18a.palette[23][0],f18a.palette[23][1],f18a.palette[23][2]);
        SetTColor(paFCol25,f18a.palette[24][0],f18a.palette[24][1],f18a.palette[24][2]);
        SetTColor(paFCol26,f18a.palette[25][0],f18a.palette[25][1],f18a.palette[25][2]);
        SetTColor(paFCol27,f18a.palette[26][0],f18a.palette[27][1],f18a.palette[27][2]);
        SetTColor(paFCol28,f18a.palette[27][0],f18a.palette[28][1],f18a.palette[28][2]);
        SetTColor(paFCol29,f18a.palette[28][0],f18a.palette[29][1],f18a.palette[29][2]);
        SetTColor(paFCol30,f18a.palette[29][0],f18a.palette[30][1],f18a.palette[30][2]);
        SetTColor(paFCol31,f18a.palette[30][0],f18a.palette[31][1],f18a.palette[31][2]);
        SetTColor(paFCol32,f18a.palette[31][0],f18a.palette[32][1],f18a.palette[32][2]);

        SetTColor(paFCol33,f18a.palette[32][0],f18a.palette[32][1],f18a.palette[32][2]);
        SetTColor(paFCol34,f18a.palette[33][0],f18a.palette[33][1],f18a.palette[331][2]);
        SetTColor(paFCol35,f18a.palette[34][0],f18a.palette[34][1],f18a.palette[34[2]);
        SetTColor(paFCol36,f18a.palette[35][0],f18a.palette[35][1],f18a.palette[35][2]);
        SetTColor(paFCol37,f18a.palette[36][0],f18a.palette[36][1],f18a.palette[36][2]);
        SetTColor(paFCol38,f18a.palette[37][0],f18a.palette[37][1],f18a.palette[37][2]);
        SetTColor(paFCol39,f18a.palette[38][0],f18a.palette[38][1],f18a.palette[38][2]);
        SetTColor(paFCol40,f18a.palette[39][0],f18a.palette[39][1],f18a.palette[39][2]);
        SetTColor(paFCol41,f18a.palette[40][0],f18a.palette[40][1],f18a.palette[40][2]);
        SetTColor(paFCol42,f18a.palette[41][0],f18a.palette[41][1],f18a.palette[41][2]);
        SetTColor(paFCol43,f18a.palette[42][0],f18a.palette[42][1],f18a.palette[42][2]);
        SetTColor(paFCol44,f18a.palette[43][0],f18a.palette[43][1],f18a.palette[43][2]);
        SetTColor(paFCol45,f18a.palette[44][0],f18a.palette[44][1],f18a.palette[44][2]);
        SetTColor(paFCol46,f18a.palette[45][0],f18a.palette[45][1],f18a.palette[45][2]);
        SetTColor(paFCol47,f18a.palette[46][0],f18a.palette[46][1],f18a.palette[46][2]);
        SetTColor(paFCol48,f18a.palette[47][0],f18a.palette[47][1],f18a.palette[47][2]);
        SetTColor(paFCol49,f18a.palette[48][0],f18a.palette[48][1],f18a.palette[48][2]);
        SetTColor(paFCol50,f18a.palette[49][0],f18a.palette[49][1],f18a.palette[49][2]);
        SetTColor(paFCol51,f18a.palette[50][0],f18a.palette[50][1],f18a.palette[50][2]);
        SetTColor(paFCol52,f18a.palette[51][0],f18a.palette[51][1],f18a.palette[51][2]);
        SetTColor(paFCol53,f18a.palette[52][0],f18a.palette[52][1],f18a.palette[52][2]);
        SetTColor(paFCol54,f18a.palette[53][0],f18a.palette[53][1],f18a.palette[53][2]);
        SetTColor(paFCol55,f18a.palette[54][0],f18a.palette[54][1],f18a.palette[54][2]);
        SetTColor(paFCol56,f18a.palette[55][0],f18a.palette[55][1],f18a.palette[55][2]);
        SetTColor(paFCol57,f18a.palette[56][0],f18a.palette[56][1],f18a.palette[56][2]);
        SetTColor(paFCol58,f18a.palette[57][0],f18a.palette[57][1],f18a.palette[57][2]);
        SetTColor(paFCol59,f18a.palette[58][0],f18a.palette[58][1],f18a.palette[58][2]);
        SetTColor(paFCol60,f18a.palette[59][0],f18a.palette[59][1],f18a.palette[59][2]);
        SetTColor(paFCol61,f18a.palette[60][0],f18a.palette[60][1],f18a.palette[60][2]);
        SetTColor(paFCol62,f18a.palette[61][0],f18a.palette[61][1],f18a.palette[61][2]);
        SetTColor(paFCol63,f18a.palette[62][0],f18a.palette[62][1],f18a.palette[62][2]);
        SetTColor(paFCol64,f18a.palette[63][0],f18a.palette[63][1],f18a.palette[63][2]);
    }
}
//---------------------------------------------------------------------------

void __fastcall Tpaletteviewer::do_refresh()
{
    if (AutoRefresh1->Checked) {
        UpdateChanges();
    }
}
//---------------------------------------------------------------------------

void Tpaletteviewer::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("PALVIEW","Top",Top);
    Left = ini->ReadInteger("PALVIEW","Left",Left);
    AutoRefresh1->Checked = ini->ReadBool("PALVIEW", "AutoRefresh", true);
}
//---------------------------------------------------------------------------

void Tpaletteviewer::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("PALVIEW","Top",Top);
    ini->WriteInteger("PALVIEW","Left",Left);
    ini->WriteBool("PALVIEW", "AutoRefresh", AutoRefresh1->Checked);
}
//---------------------------------------------------------------------------

void __fastcall Tpaletteviewer::Refresh1Click(TObject *Sender)
{
    UpdateChanges();
}
//---------------------------------------------------------------------------

void __fastcall Tpaletteviewer::AutoRefresh1Click(TObject *Sender)
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

void __fastcall Tpaletteviewer::FormShow(TObject *Sender)
{
    if (emul2.F18A)
    {
        gF18A->Enabled=true;
    }
    AutoRefresh1->Enabled = true;
    UpdateChanges();
}
//---------------------------------------------------------------------------


