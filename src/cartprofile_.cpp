/* EmulTwo  - A Windows Colecovision emulator.
 * Copyright (C) 2014-2023 Alekmaul
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
 * cartprofile_.cpp

$8000 : CARTRIDGE
        game type = bytes AA 55 , display CV logo title screen
        test type = bytes 55 AA , execute your game no delay
        otherwise it is invalid and display "INSERT CARTRIDGE"

$8002 : LOCAL_SPR_TBL
        it is the memory address where is your sprites table
        for their coordinates, patterns and colors.

$8004 : SPRITE_ORDER
        it is a table of usually maximum 32 bytes
        that simply say in which order each sprite entry
        should display. The corresponding initialisation function
        set the values as 0,1,2,3,4... which mean that the first
        entry (entry 0) is first, then the second entry (entry 1)
        is second to be disaplayed. By reordering these numbers, you
        affect the order of sprites, allowing to do flickering if needed.

$8006 : WORK_BUFFER
        it is usually a big memory RAM space that can be used by
        some BIOS functions to calculate graphics manipulations
        like mobile and semi-mobile objects, tiles rotations, etc.

$8008 : CONTROLLER_MAP
        it contains first 2 bytes telling if port-1 and port-2 are
        enabled to be analyzed with the POLLER function. The following
        values correspond to different fire buttons, stick direction,
        keypad and spinner values.

$800A : START_GAME
        it is the address of the starting point of you code, your main code entry.

$800C - $801B : RST_8H_RAM - RST_30H_RAM
        Are usually jumps to sub-programs in the cartridge or to BIOS functions.

$801E : IRQ_INT_VECT
        It should be a jump to a sub-routine saving all the PAIRs
        (AF, BC, DE, HL, AF', BC' DE' and HL') and then jump to BIOS
        to count spinner value $1F88 -or- something similar made by
        the programmer. To avoid an interuption within an interuption
        it is highly recommanded to use DI and EI opcodes to encapsulate
        this part of the code. "IM 1" should be done as soon as possible
        in game program, like the first thing at the main game entry point.

$8021 : NMI_INT_VECT
        It is the crucial part of all ColecoVision games. If you are not
        using the CV logo screen, you can directly code your sequence
        of actions to do while a non maskable interrupt (NMI) occurs.
        To be safe, you should save all the regsiter PAIRs before
        executing anything in the NMI function.
        And most important, you should read the video status
        Otherwise there will be no other non maskable interupt.

$8024 : GAME_NAME
        It is a string that contains first the name of the game
        with usually the trademark symbol (characters 1E and 1F),
        a slash as a separator, then the usual PRESENTS with  the original licensor,
        a slash separator again, and finally the release year in 4 digits.

*/

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "cartprofile_.h"

#include <stdio.h>

#include "coleco.h"
#include "colecoconfig.h"
#include "main_.h"
#include "utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tcartprofile *cartprofile;
//---------------------------------------------------------------------------
__fastcall Tcartprofile::Tcartprofile(TComponent* Owner)
        : TForm(Owner)
{
        TIniFile *ini;

        ini = new TIniFile(coleco.inipath);
        LoadSettings(ini);
        delete ini;

        // Init empty value
        valEmpty=0xFF;
}
//---------------------------------------------------------------------------

void __fastcall Tcartprofile::FormShow(TObject *Sender)
{
        // Display rom information
        ShowProfile();

        // Display banks footprint
        ShowBanks();
}
//---------------------------------------------------------------------------

void __fastcall Tcartprofile::ShowProfile(void)
{
    char text[256],tVal[16];
    int ix,iy,tlen;
    unsigned char bval;
	char str[128];

    // Init memo and search for address 0x8000 entries
    Memo1->Lines->Clear();
    Memo1->Lines->Add("Cartridge Identifier");

    // Check regarding Coleco or other machines
        // Hearder type & name
        if ((coleco_getbyte(0x8000)==0xAA) && (coleco_getbyte(0x8001)==0x55))
            Memo1->Lines->Add("Header Type: Game (0xAA55)");
        else if ((coleco_getbyte(0x8000)==0x55) && (coleco_getbyte(0x8001)==0xAA) )
            Memo1->Lines->Add("Header Type: Test (0x55AA)");
        else if ((coleco.romCartridge == ROMCARTRIDGEMEGA) )
        {
            sprintf(text,"Header Type: MegaCart (%04X)",coleco_getbyte(0x8000)+coleco_getbyte(0x8001)*256);
            Memo1->Lines->Add(text);
        }
        else if ((coleco.romCartridge == ROMCARTRIDGEZX81) )
        {
            sprintf(text,"Header Type: ZX81Cart (%04X)",coleco_getbyte(0x8000)+coleco_getbyte(0x8001)*256);
            Memo1->Lines->Add(text);
        }
        else
            Memo1->Lines->Add("Header Type: Invalid (InsertCartridge message)");
        strcpy(text,"Game Name  : ....................................");
        for (ix=0;ix<36;ix++)
        {
            bval=coleco_getbyte(0x8024+ix);
            if ( (bval>=32) && (bval<=127) ) // between 32..127
                text[ix+13]=bval;
        }
        Memo1->Lines->Add(text);
        if (coleco.typebackup==EEP24C08) Memo1->Lines->Add("256-byte EEPROM");
        else if (coleco.typebackup==EEP24C256) Memo1->Lines->Add("32kB EEPROM");
        else if (coleco.typebackup==EEPSRAM) Memo1->Lines->Add("2kB SRAM");

        // Vector entries
        Memo1->Lines->Add(" ");
        Memo1->Lines->Add("Pointers");
        sprintf(text,"Sprite Table  : %04Xh",coleco_getbyte(0x8002)+coleco_getbyte(0x8003)*256);
        Memo1->Lines->Add(text);
        sprintf(text,"Sprite Order  : %04Xh",coleco_getbyte(0x8004)+coleco_getbyte(0x8005)*256);
        Memo1->Lines->Add(text);
        sprintf(text,"Work Buffer   : %04Xh",coleco_getbyte(0x8006)+coleco_getbyte(0x8007)*256);
        Memo1->Lines->Add(text);
        sprintf(text,"Controller Map: %04Xh",coleco_getbyte(0x8008)+coleco_getbyte(0x8009)*256);
        Memo1->Lines->Add(text);
        sprintf(text,"Game Start    : %04Xh",coleco_getbyte(0x800A)+coleco_getbyte(0x800B)*256);
        Memo1->Lines->Add(text);

        Memo1->Lines->Add(" ");
        Memo1->Lines->Add("Restart Vectors");
        for (iy=0x800C;iy<0x801D;iy+=3) {
            sprintf(text,"Rst%02XH      : %04Xh",(iy-0x800C)*8+8,coleco_getbyte(iy)+coleco_getbyte(iy+1)*256);
            Memo1->Lines->Add(text);
        }

        Memo1->Lines->Add(" ");
        Memo1->Lines->Add("Interrupt Vectors");
        sprintf(text,"IRQ Vertor  : %04Xh",coleco_getbyte(0x801E)+coleco_getbyte(0x801F)*256);
        Memo1->Lines->Add(text);
        sprintf(text,"NMI Vertor  : %04Xh",coleco_getbyte(0x8021)+coleco_getbyte(0x8022)*256);
        Memo1->Lines->Add(text);

        // Do a dump of header
        Memo1->Lines->Add(" ");
        Memo1->Lines->Add("Header Dump");
        for (iy=0x8000;iy<0x8040;iy+=8)
        {
            sprintf(text,"%04X: ",iy);
            for (ix=0;ix<8;ix++)
            {
                sprintf(tVal,"%02X ",coleco_getbyte(iy+ix));
                strcat(text,tVal);
            }
            for (ix=0;ix<8;ix++)
            {
                bval=coleco_getbyte(iy+ix);
                if ( (bval>=32) && (bval<=127) ) // between 32..127
                {
                    tlen=strlen(text);
                    text[tlen]=bval;
                    text[tlen+1]=0;
                }
                else
                    strcat(text,".");
            }
            Memo1->Lines->Add(text);
        }
}

//---------------------------------------------------------------------------

void __fastcall Tcartprofile::DrawBANK(int x,int y,unsigned char *data,int fill)
{
	const int markw[9]={8,3,6,3,8,3,6,3,8};
	int i,j,n,bf,sy;
	unsigned char *src,*dst;
	TCanvas *c;

        // Fill content with color or not regarding fill value
	c=ImageBanks->Canvas;
	src=data;
	for(i=0;i<128;++i)
	{
		dst=(unsigned char*)iBank->Picture->Bitmap->ScanLine[i];
		for(j=0;j<64;++j)
                {
                        *dst++=((*src++==valEmpty)?2:0);
                        src++;
                }
	}

        // Draw border
	c->Pen->Color=clBlack;
	c->Rectangle(x,y,x+66,y+130);
	c->Draw(x+1,y+1,iBank->Picture->Bitmap);

        // Draw the lines on the side
	sy=y;
	for(i=0;i<9;++i)
	{
		c->PenPos=TPoint(x+66,sy);
		c->LineTo(x+66+markw[i],sy);
		c->PenPos=TPoint(x+66,sy+1);
		c->LineTo(x+66+markw[i],sy+1);
		sy+=16;
	}

        // Calculate size regarding content (0 or else)
	src=data;
	bf=0;
	for(i=0;i<1024;++i)
	{
		n=16;
		for(j=0;j<16;j++)
                        if(*src++!=valEmpty) n=0;
		bf+=n;
	}

	c->TextOut(x,y+132,"~"+IntToStr(bf/1024)+"K free ("+IntToStr((int)(100.0f/32768.0f*(float)bf))+"%)");
}
//---------------------------------------------------------------------------

void __fastcall Tcartprofile::ShowBanks(void)
{
        AnsiString info;
        int romsize;
        int i,x,y,bknum;
        BYTE *romentry;
        int maxx,maxy,off,hgt;

    if (coleco.romCartridge == ROMCARTRIDGENONE)
	{
                ImageBanks->Canvas->TextOut(10,10,"No rom cartridge");
		return;
	}

        // Init some vars regarding current rom
        romentry=ROM_CARTRIDGE;
        bknum=coleco_megasize;

        // Calculate size of view
	x=8;
	y=8;
	maxx=grpMemFootPrint->ClientWidth;
	maxy=y;
        hgt=128+60;
	for(i=0;i<bknum;i++)
	{
		maxy=y+hgt;
		x+=64+20;
		if(x+64+10>=maxx)
		{
			x=10;
			y+=hgt;
			if(i==bknum-1) hgt=128+20;
		}
	}
	ImageBanks->Height=(maxy<ScrollBanks->ClientHeight)?ScrollBanks->ClientHeight:maxy;
	ImageBanks->Canvas->Brush->Color=clWhite;
	ImageBanks->Canvas->Rectangle(0,0,ImageBanks->Width,ImageBanks->Height);

	off=0;
	x=8;
	y=4;
	hgt=128+40;

	for(i=0;i<bknum;i++)
	{
		ImageBanks->Canvas->TextOut(x,y,"BANK $"+IntToHex(i+1,2));
		DrawBANK(x,y+16,&romentry[off],0x00);

		off+=16384;
		x+=64+20;
		if(x+64+10>=maxx)
		{
			x=10;
			y+=hgt;
			if(i==bknum-1) hgt=128+20;
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall Tcartprofile::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Form1->Cartprofile1->Checked = false;
}
//---------------------------------------------------------------------------

void Tcartprofile::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("TCPROF","Top",Top);
    Left = ini->ReadInteger("TCPROF","Left",Left);
}
//---------------------------------------------------------------------------

void Tcartprofile::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("TCPROF","Top",Top);
    ini->WriteInteger("TCPROF","Left",Left);
}
//---------------------------------------------------------------------------

void __fastcall Tcartprofile::Button1Click(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------

void __fastcall Tcartprofile::FormCreate(TObject *Sender)
{
	Graphics::TBitmap *bmp;
        PALETTEENTRY pal[256];

        // create bitmap for each 16K bank
	bmp=new Graphics::TBitmap();
	bmp->Width=iBank->Width;
        bmp->Height=iBank->Height;
	bmp->PixelFormat=pf8bit;

	pal[0].peBlue =255; pal[0].peGreen=255;	pal[0].peRed=255;
	pal[1].peBlue =240; pal[1].peGreen=240;	pal[1].peRed=240;
	pal[2].peBlue =48; pal[2].peGreen=255; pal[2].peRed=48;
	pal[3].peBlue =0; pal[3].peGreen=0; pal[3].peRed=240;

	SetPaletteEntries(bmp->Palette,0,4,pal);

	iBank->Picture=new TPicture();
	iBank->Picture->Bitmap=bmp;

        delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall Tcartprofile::RefreshClick(TObject *Sender)
{
        // Display rom information
        ShowProfile();

        // Display banks footprint
        ShowBanks();
}
//---------------------------------------------------------------------------

void __fastcall Tcartprofile::EmptyFFClick(TObject *Sender)
{
        valEmpty=0xFF;
        RefreshClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall Tcartprofile::Empty00Click(TObject *Sender)
{
        valEmpty=0x00;
        RefreshClick(NULL);
}
//---------------------------------------------------------------------------

