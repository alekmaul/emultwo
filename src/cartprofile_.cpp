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


Hardware Information
Video Type    : NTSC 60Hz
Chipset       : -
Mapping       : LoROM
ROM Size      : 1024K
ROM Speed     : Slow (2.68MHz)
SRAM Size     : 0K
Battery       : No
Exp.RAM Size  : 0K
Exp.FLASH Size: 0K
DataPackSlot  : No

ROM-Image (File)
File Name     : F:\DropboxSvnClient\snes\illmoorebay\out\illmoorebay.sfc
Short Name    : illmoorebay.s
File Format   : RAW
CRC-32        : ADDE20DBh

Header Corruption
Invalid ROM Size Entry

Special Controllers
(no special controllers detected)

Header Dump
FFB0 28 6B 53 4E 45 53 6D 61 70 5F 6D 61 69 6E 0A 00 (kSNESmap_main
FFC0 49 4C 4C 4D 4F 4F 52 45 42 41 59 20 20 20 20 20 ILLMOOREBAY     
FFD0 20 20 20 20 20 20 00 0B 00 01 00 00 15 B9 EA 46       F
FFE0 CB 6B 6B 6B 00 80 00 80 00 80 91 80 00 00 00 80 kkk
FFF0 6B 00 00 00 00 80 00 00 00 80 00 80 F8 8A 00 80 k

Registers
(no special registers)


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
}
//---------------------------------------------------------------------------

void __fastcall Tcartprofile::FormShow(TObject *Sender)
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
        else if ((coleco_megacart != 0) )
        {
            sprintf(text,"Header Type: MegaCart (%04X)",coleco_getbyte(0x8000)+coleco_getbyte(0x8001)*256);
            Memo1->Lines->Add(text);
        }
/*        else if ((coleco_zxcart != 0) )
        {
            sprintf(text,"Header Type: ZX81Cart (%04X)",coleco_getbyte(0x8000)+coleco_getbyte(0x8001)*256);
            Memo1->Lines->Add(text);
        }*/
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

