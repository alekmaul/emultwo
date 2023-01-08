/* EmulTwo  - A Windows ColecoVision emulator.
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
 * coleco.cpp
 * Based on   ColEm for memory implementation
 *
 */
#include <vcl.h>
#include <stdio.h>

#include "coleco.h"
#include "accsound_.h"
#include "utils.h"

#include "z80\z80.h"

#include "ay8910.h"
#include "sn76489.h"
#include "tms9928a.h"
#include "c24xx.h"
#include "adamnet.h"

// include roms files
#include "bios_coleco.h"
#include "bios_adam.h"

#include "printviewer_.h"

extern void DebugUpdate(void);

//---------------------------------------------------------------------------
BYTE cv_display[TVW*TVH];                       // Coleco display buffer
BYTE cv_palette[16*3];                          // Coleco display palette
int cv_pal32[16];                               // Coleco display palette in 32 bits RGB
unsigned int *cv_screen[TVH];                   // Coleco display buffer in 32 bits RGB

BYTE cvmemory[MAXRAMSIZE];                      // CPU address space
BYTE eepromdata[MAXEEPROMSIZE];                 // Max EEProm memory 32Ko
BYTE *ROMPage[8];                               // 8x8kB read-only (ROM) pages
BYTE *RAMPage[8];                               // 8x8kB read-write (RAM) pages

BYTE coleco_megapage;                           // selected page for megacart
BYTE coleco_megasize;                           // mega cart rom size in 16kB pages
BYTE coleco_megacart;                           // <>0 if mega cart rom detected

BYTE coleco_port20;                             // Adam port 0x20-0x3F (AdamNet)
BYTE coleco_port60;                             // Adam port 0x60-0x7F (memory)
BYTE coleco_port53;                             // SGM port 0x53 (memory)

BYTE coleco_joymode;                            // Joystick / Paddle management
unsigned int coleco_joystat;                    // Joystick / Paddle management

unsigned int coleco_spincount;                  // Spinner counters
unsigned int coleco_spinstep;                   // Spinner steps
unsigned int coleco_spinstate;                  // Spinner bit states

int tstates,frametstates;                       // count of instruction  times
int tStatesCount;                               // count of instruction  times

int coleco_nmigenerate=0;                       // 1 if we need to do a NMI
int coleco_updatetms=0;                         // 1 if TMS is updated

FDIDisk Disks[MAX_DISKS] = { 0 };               // Adam disk drives
FDIDisk Tapes[MAX_TAPES] = { 0 };               // Adam tape drives          

static BYTE idleDataBus = 0xFF;

//---------------------------------------------------------------------------
// Get tms vram adress
unsigned short coleco_gettmsaddr(BYTE whichaddr, BYTE mode, BYTE y)
{
    unsigned short result;

    switch (whichaddr)
    {
        case CHRMAP:
            result = tms.ChrTab-tms.ram;
            break;
        case CHRGEN:
            result = tms.ChrGen-tms.ram;
            if ((mode == 2) && (y>= 0x80) )
            {
                switch (tms.VR[4]&3) {
                    case 0: break;
                    case 1: result+=0x1000; break;
                    case 2: break; //PGT-=0x800; break;
                    case 3: result+=0x1000; break; //PGT+=0x800; break;
                }
            }
            else if ((tms.VR[4]&0x02) && (mode ==2) && (y>= 0x40))
            {
                result+=0x800;
            }
            break;
        case CHRCOL:
            result = tms.ColTab-tms.ram;
            if ((mode == 2) && (y>= 0x80) )
            {
                switch (tms.VR[3]&0x60) {
                    case 0: break;
                    case 0x20: result+=0x1000; break;
                    case 0x40: break; //CLT-=0x800; break;
                    case 0x60: result+=0x1000; break; //CLT+=0x800; break;
                }
            }
            else if ((tms.VR[3]&0x40) && (mode ==2) && (y>= 0x40))
            {
                result+=0x800;
            }
            break;
        case SPRATTR:
            result = tms.SprTab-tms.ram;
            break;
        case SPRGEN:
            result = tms.SprGen-tms.ram;
            break;
        case VRAM:
            result = 0;
            break;
    }

    return result;
}

//---------------------------------------------------------------------------
// Get tms value of vram adress
BYTE coleco_gettmsval(BYTE whichaddr, unsigned short addr, BYTE mode, BYTE y)
{
    byte result=0;

    switch (whichaddr)
    {
        case CHRMAP:
            result = tms.ChrTab[addr];
            break;
        case CHRGEN:
            switch(mode) {
                case 0:
                case 1:
                    break;
                case 2:
                    if (y>= 0x80)
                    {
                        switch (tms.VR[4]&3) {
                            case 0: break;
                            case 1: addr+=0x1000; break;
                            case 2: break;
                            case 3: addr+=0x1000; break;
                        }
                    }
                    else if ((tms.VR[4]&0x02) && (y>= 0x40))
                    {
                        addr+=0x800;
                    }
                    break;
            }
            result = tms.ChrGen[addr];
            break;
        case CHRCOL:
            switch(mode) {
                case 0:
                case 1:
                    addr>>=6;
                    break;
                case 2:
                    if (y>= 0x80)
                    {
                        switch (tms.VR[3]&0x60) {
                            case 0: break;
                            case 0x20: addr+=0x1000; break;
                            case 0x40: break;
                            case 0x60: addr+=0x1000; break;
                        }
                    }
                    else if ((tms.VR[3]&0x40) && (y>= 0x40))
                    {
                        addr+=0x800;
                    }
                    break;
            }
            result = tms.ColTab[addr];
            break;
        case SPRATTR:
            result = tms.SprTab[addr];
            break;
        case SPRGEN:
            result = tms.SprGen[addr];
            break;
        case VRAM:
            result = tms.ram[addr];
            break;
/*        case SGMRAM:
            result = cvexpram[addr];
            break;
        case RAM:
            result = cartrom[0x6000+addr];
            break;
            */
        case EEPROM:
            result = eepromdata[addr];
            break;
/*        case SRAM:
            result = cartrom[0xE800+addr];
            break;                        */
    }

    return result;
}

//---------------------------------------------------------------------------
// Set a value
void coleco_setval(BYTE whichaddr, unsigned short addr, BYTE y)
{
    switch (whichaddr)
    {
        case VRAM:
            tms.ram[addr] = y;
            break;
/*TODO        case SGMRAM:
            cvexpram[addr] = y;
            break;
        case RAM:
            addr&=0x03FF;
            cartrom[0x6000+addr]=y;
            cartrom[0x6400+addr]=y;
            cartrom[0x6800+addr]=y;
            cartrom[0x6C00+addr]=y;
            cartrom[0x7000+addr]=y;
            cartrom[0x7400+addr]=y;
            cartrom[0x7800+addr]=y;
            cartrom[0x7C00+addr]=y;
            break;
        case ROM:
            cartrom[addr]=y;
            break;
            */
        case EEPROM:
            eepromdata[addr]=y;
            break;
/*        case SRAM:
            addr&=0x07FF;
            cartrom[0xE800+addr]=y;
            break;
*/
    }
}

//---------------------------------------------------------------------------
// Load a rom file
BYTE coleco_loadcart(char *filename, unsigned char cardtype)
{
        int i,j,size;
        BYTE Buf[2],*P;
        BYTE retf = ROM_LOAD_FAIL;
        FILE *fRomfile = NULL;

        // load rom file
        fRomfile = fopen(filename, "rb");
        if (fRomfile == NULL)
                return(retf);

        // Determine size via ftell() or by reading entire [GZIPped] stream
        if(!fseek(fRomfile,0,SEEK_END)) size=ftell(fRomfile);
        else
        {
                // Read file in 8kB increments
                for(size=0;(j=fread(RAM_DUMMY,1,0x2000,fRomfile))==0x2000;size+=j);
                if (j>0) size+=j;
                // Clean up the dummy RAM!
                memset(RAM_DUMMY,idleDataBus,0x2000);
        }

        // Validate size
        if((size<=0)||(size>128*0x4000)) { fclose(fRomfile);return(retf); }

        // Rewind file to the beginning & Read magic number
        rewind(fRomfile);
        if(fread(Buf,1,2,fRomfile)!=2) { fclose(fRomfile);return(retf); }

        // If it is a ColecoVision game cartridge or a Coleco Adam expansion ROM...
        P = (Buf[0]==0x55)&&(Buf[1]==0xAA)? ROM_CARTRIDGE
            : (Buf[0]==0xAA)&&(Buf[1]==0x55)? ROM_CARTRIDGE
            : (Buf[0]==0x66)&&(Buf[1]==0x99)? ROM_EXPANSION
            : 0;

        // Rewind file to the beginning
        rewind(fRomfile);

        // Assume normal cartridge, not a coleco_megacart
        i = 0;

        // coleco_megacarts have magic number in the last 16kB page
        if(!P&&(size>0x8000))
        {
                if(fseek(fRomfile,(size&~0x3FFF)-0x4000,SEEK_SET)>=0)
                {
                        if(fread(Buf,1,2,fRomfile)==2)
                        {
                                if(((Buf[0]==0x55)&&(Buf[1]==0xAA))||((Buf[0]==0xAA)&&(Buf[1]==0x55)))
                                {
                                        i = 1;
                                        P = ROM_CARTRIDGE;
                                }
                        }
                }
                rewind(fRomfile);
        }

        // If ROM not recognized, drop out
        if(!P) { fclose(fRomfile);return(retf); }

        // If loading a cartridge...
        if (P==ROM_CARTRIDGE)
        {
                // Pad to the nearest 16kB and find number of 16kB pages
                size = ((size+0x3FFF)&~0x3FFF)>>14;

                // Round page number up to the nearest power of two
                for(j=2;j<size;j<<=1);

                // Set new MegaROM size
                size = j<<14;
                coleco_megasize = j;
                coleco_megacart = i ? j-1:0;
                coleco.romCartridge = coleco_megacart ? ROMCARTRIDGEMEGA : ROMCARTRIDGESTD;
        }

        // Rewind file to the beginning
        rewind(fRomfile);

        // Clear ROM buffer
        memset(P,idleDataBus,size);

        // Read the ROM
        j=fread(P,1,size,fRomfile);

        // Close rom file
        fclose(fRomfile);

        // Keep initial cartridge CRC (may change after SRAM writes)
        coleco.cardsize=size;
        if(P==ROM_CARTRIDGE) coleco.cardcrc=CRC32Block(ROM_CARTRIDGE,coleco.cardsize);

        return ROM_LOAD_PASS;
}

//---------------------------------------------------------------------------
// update the 16 colors Coleco
void coleco_setpalette(int palette) {
        int index, idxpal;

        // init palette
        idxpal=palette*3*16;
        for (index=0;index<16*3;index+=3) {
                cv_palette[index] = TMS9918A_palette[idxpal+index];
                cv_palette[index+1] = TMS9918A_palette[idxpal+index+1];
                cv_palette[index+2] = TMS9918A_palette[idxpal+index+2];
                cv_pal32[index/3]=_mRGB32(cv_palette[index],cv_palette[index+1],cv_palette[index+2]);
        }
}

//---------------------------------------------------------------------------
// Update memory mapping
void coleco_setmemory(BYTE NewPort60,BYTE NewPort20,BYTE NewPort53)
{
        BYTE *P;

        // Store new values
        coleco_port60 = NewPort60;
        coleco_port20 = NewPort20;
        coleco_port53 = NewPort53;

        // Lower 32kB ROM
        if (coleco.machine == MACHINEADAM)
        {
                // Coleco Adam
                if(!(NewPort60&0x03)&&(NewPort20&0x02))
                {
                        // EOS enabled
                        ROMPage[0] = RAM_DUMMY;
                        ROMPage[1] = RAM_DUMMY;
                        ROMPage[2] = ROM_EOS;
                        ROMPage[3] = ROM_EOS;
                }
                else
                {
                        // Normal configuration
                        ROMPage[0] = cvmemory +((int)(NewPort60&0x03)<<15);
                        ROMPage[1] = ((NewPort60&0x03)==3? RAM_MAIN_LO:ROMPage[0])+0x2000;
                        ROMPage[2] = ROMPage[1]+0x2000;
                        ROMPage[3] = ROMPage[1]+0x4000;
                }
        }
        else if(coleco.SGM)
        {
                // Super Game Module (SGM)
                ROMPage[0] = NewPort60&0x02? ROM_BIOS:RAM_BASE;
                if(NewPort53&0x01)
                {
                        // 24kB RAM enabled
                        ROMPage[1] = RAM_BASE+0x2000;
                        ROMPage[2] = RAM_BASE+0x4000;
                        ROMPage[3] = RAM_BASE+0x6000;
                }
                else
                {
                        // Normal configuration
                        ROMPage[1] = RAM_DUMMY;
                        ROMPage[2] = RAM_DUMMY;
                        ROMPage[3] = RAM_BASE;
                }
        }
        else
        {
                // Regular ColecoVision
                ROMPage[0] = ROM_BIOS;
                ROMPage[1] = RAM_DUMMY;
                ROMPage[2] = RAM_DUMMY;
                ROMPage[3] = RAM_BASE;
        }

        // Upper 32kB ROM
        P = RAM_MAIN_HI+((int)(NewPort60&0x0C)<<13);
        ROMPage[4] = P + (coleco_megacart<<14);
        ROMPage[5] = ROMPage[4]+0x2000;
        ROMPage[6] = P + (((NewPort60&0x0C)==0x0C? (coleco_megapage&(coleco_megasize-1)):1)<<14);
        ROMPage[7] = ROMPage[6]+0x2000;

        // Lower 32kB RAM
        if(coleco.machine == MACHINEADAM)
        {
                // Coleco Adam
                if(!(NewPort60&0x03))
                        RAMPage[0]=RAMPage[1]=RAMPage[2]=RAMPage[3]=RAM_DUMMY;
                else
                {
                        RAMPage[0] = (NewPort60&0x03)==3? RAM_DUMMY:ROMPage[0];
                        RAMPage[1] = ROMPage[1];
                        RAMPage[2] = ROMPage[2];
                        RAMPage[3] = ROMPage[3];
                }
        }
        else if(coleco.SGM)
        {
                // Super Game Module (SGM)
                RAMPage[0] = NewPort60&0x02? RAM_DUMMY:ROMPage[0];
                RAMPage[1] = ROMPage[1];
                RAMPage[2] = ROMPage[2];
                RAMPage[3] = ROMPage[3];
        }
        else
        {
                // Regular ColecoVision
                RAMPage[0] = RAM_DUMMY;
                RAMPage[1] = ROMPage[1];
                RAMPage[2] = ROMPage[2];
                RAMPage[3] = ROMPage[3];
        }

        // Upper 32kB RAM
        if(NewPort60&0x04)
                RAMPage[4]=RAMPage[5]=RAMPage[6]=RAMPage[7]=RAM_DUMMY;
        else
        {
                RAMPage[4] = ROMPage[4];
                RAMPage[5] = ROMPage[5];
                RAMPage[6] = ROMPage[6];
                RAMPage[7] = ROMPage[7];
        }

        // Reset AdamNet
        if( (coleco.machine == MACHINEADAM)&&(NewPort20==0x0F)) ResetPCB();
}

//---------------------------------------------------------------------------
void coleco_reset(void)
{
        // Init memory pages
        coleco_megapage = 1;

        // Clear memory (important for NetPlay, to keep states at both sides consistent)
        // Clearing to zeros (Heist)
        memset(RAM_MAIN_LO,0x00,0x8000);
        memset(RAM_MAIN_HI,0x00,0x8000);
        memset(RAM_EXP_LO,0x00,0x8000);
        memset(RAM_EXP_HI,0x00,0x8000);
        memset(RAM_OS7,0x00,0x2000);

        coleco_setmemory(coleco.machine == MACHINEADAM ? 0x00 : 0x0F,0x00,0x00);

        // check for SRAM && EEPROM and force type
        switch (coleco.cardcrc) {
                case 0x62DACF07: // Boxxle 32kB EEPROM
                        coleco_megacart = 3;
                        coleco_megapage=0;
                        coleco.typebackup=EEP24C256;
                        break;
                case 0xDDDD1396: // Black Onyx 256-byte EEPROM
                        coleco_megacart = 3;
                        coleco_megapage=0;
                        coleco.typebackup=EEP24C08;
                        break;
                case 0xFEE15196: // Lord Of The Dungeon 24kB ROM + 8kB garbage + 2kB SRAM
                case 0x1053F610: // Lord Of The Dungeon 24kB ROM + 2kB SRAM
                case 0x60D6FD7D: // Lord Of The Dungeon 24kB ROM + 2kB SRAM
                case 0x37A9F237: // Lord Of The Dungeon 24kB ROM + 2kB SRAM
                        coleco.typebackup=EEPSRAM;
                        break;
        }

        // Reset hardware and CPU
        tms9918_reset();
        tms.ScanLines=coleco.NTSC ? TMS9918_LINES : TMS9929_LINES;
        sn76489_init(coleco.NTSC ? CLOCK_NTSC : CLOCK_PAL, 44100);
        if (coleco.SGM) {
                ay8910_init(coleco.NTSC ? CLOCK_NTSC : CLOCK_PAL, 44100);
        }
        SoundPrepSmpTab(tms.ScanLines);
        if (coleco.typebackup!=NOBACKUP)
                c24xx_reset(eepromdata,coleco.typebackup==EEP24C08 ? C24XX_24C08 : coleco.typebackup==EEP24C256 ? C24XX_24C256 : 0);
        z80_reset();

        // Init some vars
        tStatesCount = 0;

        // Init joystick/spinner mode and states
        coleco_joymode=0;
        coleco_joystat=0x00000000;
        coleco_spincount=0;
        coleco_spinstep=0;
        coleco_spinstate=0x00000000;
        //coleco_steerwheel=machine.steerwheel ? 1 : 0;
        //coleco_rollercontrol=machine.rollercontrol ? 1 : 0;
        //coleco_superaction=machine.superaction ? 1 : 0;
}

//---------------------------------------------------------------------------
void coleco_initialise(void)
{
        int i, romlen;

        z80_init();
        tStatesCount = 0;

        coleco_megasize = 2;
        coleco_megacart = 0;


/*
        directMemoryAccess = false;
        ResetLastIOAccesses();
        InitialiseRomCartridge();
*/

        coleco_nmigenerate=0;

        // Load COLECO.ROM: OS7 (ColecoVision BIOS)
        memcpy(ROM_BIOS,colecobios_rom,0x2000);

        // Load EOS.ROM: EOS (Adam BIOS)
        memcpy(ROM_EOS,adambios_eos,0x2000);

        // Load WRITER.ROM: SmartWriter (Adam bootup)
        memcpy(ROM_WRITER,adambios_writer,0x8000);

        // Unload any ADAM related stuff
        for(i=0;i<MAX_DISKS;i++) EjectFDI(&Disks[i]);
        for(i=0;i<MAX_TAPES;i++) EjectFDI(&Tapes[i]);

        // Reset coleco
        coleco_reset();

        // Prepare Coleco palette
        coleco_setpalette(0);
}

void coleco_WriteByte(int Address, int Data)
{

//        LiveMemoryWindow->Write(Address);

        if(coleco.machine == MACHINEADAM)
        {
                // Write to RAM
                RAMPage[Address>>13][Address&0x1FFF]=Data;

                // Adam may try writing AdamNet
                if(PCBTable[Address]) WritePCB(Address,Data);
        }
        else if((coleco.SGM)&&(coleco_port53&0x01))
        {
                // Write to RAM
                RAMPage[Address>>13][Address&0x1FFF]=Data;
        }
        // Generic write and mirroring write
        else if((Address>=0x6000)&&(Address<0x8000))
        {
                Address&=0x03FF;
                RAM_BASE[Address]       =RAM_BASE[0x0400+Address]=
                RAM_BASE[0x0800+Address]=RAM_BASE[0x0C00+Address]=
                RAM_BASE[0x1000+Address]=RAM_BASE[0x1400+Address]=
                RAM_BASE[0x1800+Address]=RAM_BASE[0x1C00+Address]=Data;
        }
        else if((Address>=0xFF80)&&(coleco_megasize>2)&&(ROMPage[7]!=RAMPage[7]))
        {
                // Cartridges, containing EEPROM, use [1111 1111 11xx 0000] addresses
                if ( (coleco.typebackup==EEP24C08) || (coleco.typebackup==EEP24C256) )
                {
                        switch(Address)
                        {
                                case 0xFFC0: c24xx_write(c24.Pins&~C24XX_SCL);return;
                                case 0xFFD0: c24xx_write(c24.Pins|C24XX_SCL);return;
                                case 0xFFE0: c24xx_write(c24.Pins&~C24XX_SDA);return;
                                case 0xFFF0: c24xx_write(c24.Pins|C24XX_SDA);return;
                        }
                }

                // MegaCarts use [1111 1111 11xx xxxx] addresses
                if(coleco_megacart)
                {
                        if(Address>=0xFFC0)
                        {
                                // Set new MegaCart ROM page at C000h */
                                coleco_megapage   = (Address-0xFFC0)&(coleco_megasize-1);
                                ROMPage[6] = ROM_CARTRIDGE + (coleco_megapage<<14);
                                ROMPage[7] = ROMPage[6]+0x2000;
                        }
                }
                else switch(Address)
                {
                        case 0xFF90:
                        case 0xFFA0:
                        case 0xFFB0:
                                //  Cartridges, containing EEPROM, use [1111 1111 10xx 0000] addresses
                                if(coleco_megasize==4)
                                {
                                        coleco_megapage   = (Address>>4)&3&(coleco_megasize-1);
                                        ROMPage[6] = ROM_CARTRIDGE + (coleco_megapage<<14);
                                        ROMPage[7] = ROMPage[6]+0x2000;
                                }
                                break;
                        case 0xFFFF:
                                // SGM multipage ROMs write page number to FFFFh
                                coleco_megapage   = Data&(coleco_megasize-1);
                                ROMPage[6] = ROM_CARTRIDGE + (coleco_megapage<<14);
                                ROMPage[7] = ROMPage[6]+0x2000;
                                break;
                }
        }
        else if((Address>0xDFFF)&&(Address<0xE800) && (coleco.typebackup==EEPSRAM))
        {
                // SRAM at E800h..EFFFh, writable via E000h..E7FFh
                ROMPage[Address>>13][(Address+0x0800)&0x1FFF] = Data;
        }
/*
    // if here, if alerting of write, do it
    if (machine.HaltWriteRom)
    {
        WarningForm->CentreOn(Form1);
        WarningForm->Warning(Address);
    }
*/    
}

void coleco_setbyte(int Address, int Data)
{
//        directMemoryAccess = true;
        coleco_WriteByte(Address, Data);
//        directMemoryAccess = false;
}

void coleco_writebyte(int Address, int Data)
{
/*
        lastMemoryWriteAddrLo = lastMemoryWriteAddrHi;
        lastMemoryWriteAddrHi = Address;

        lastMemoryWriteValueLo = lastMemoryWriteValueHi;
        lastMemoryWriteValueHi = Data;
*/
        coleco_WriteByte(Address, Data);
}

BYTE coleco_ReadByte(int Address)
{
//        LiveMemoryWindow->Read(Address);

        int data;

        // If trying to switch MegaCart...
        if((Address>=0xFFC0)&&coleco_megacart&&(ROMPage[7]!=RAMPage[7]))
        {
                // Set new MegaCart ROM page
                coleco_megapage = (Address-0xFFC0)&(coleco_megasize-1);
                ROMPage[6] = ROM_CARTRIDGE + (coleco_megapage<<14);
                ROMPage[7] = ROMPage[6]+0x2000;
        }
        else if((Address==0xFF80)&&(coleco_megasize>2)&&(ROMPage[7]!=RAMPage[7])
                && ((coleco.typebackup==EEP24C08) || (coleco.typebackup==EEP24C256))  )
        {
                // Return EEPROM output bit
                return(c24xx_read());
        }

        // Adam may try reading AdamNet
        if((coleco.machine == MACHINEADAM)&&PCBTable[Address]) ReadPCB(Address);

        data = (ROMPage[Address>>13][Address&0x1FFF]);
        return(data);
}

// Called by debugger routines
BYTE coleco_getbyte(int Address)
{
        BYTE b = coleco_ReadByte(Address);

        return b;
}

// Called by Z80 instruction operand fetches
BYTE coleco_readoperandbyte(int Address)
{
        return coleco_ReadByte(Address);
}

// Called by emulated program
BYTE coleco_readbyte(int Address)
{
        lastMemoryReadAddrLo = lastMemoryReadAddrHi;
        lastMemoryReadAddrHi = Address;

        BYTE byte = coleco_ReadByte(Address);

        lastMemoryReadValueLo = lastMemoryReadValueHi;
        lastMemoryReadValueHi = byte;

        return byte;
}

// BYTE opcode_fetch(int Address)
//
// Given an address, opcode fetch returns the byte at that memory address
//
// Called by Z80 instruction opcode fetches
BYTE coleco_opcode_fetch(int Address)
{
        BYTE byte = coleco_ReadByte(Address);
        return byte;
}

//---------------------------------------------------------------------------
void coleco_writeport(int Address, int Data, int *tstates)
{
        // Coleco uses 8bit port addressing
        Address&=0x00FF;

        switch(Address&0xE0)
        {
                case 0x80:
                        coleco_joymode=0;
                        break;
                case 0xC0:
                        coleco_joymode=1;
                        break;
                case 0xE0:
                        sn76489_write(Data);
                        break;
                case 0xA0:
                        coleco_updatetms=1; // to update screen if needed
                        if(!(Address&0x01)) tms9918_writedata(Data);
                        else if (tms9918_writectrl(Data)) coleco_nmigenerate=1;;
                        break;
                case 0x40:
                        if((coleco.machine == MACHINEADAM)&&(Address==0x40)) Printer(Data);
                        if(coleco.SGM)
                        {
                                if(Address==0x53) coleco_setmemory(coleco_port60,coleco_port20,Data);
                                else if (Address==0x50) ay8910_write(0,Data); // Control data
                                else if (Address==0x51) ay8910_write(1,Data); // Write data
                        }
                        break;
                case 0x20:
                      if (coleco.machine == MACHINEADAM) coleco_setmemory(coleco_port60,Data,coleco_port53);
                      break;
                case 0x60:
                      if ( (coleco.machine == MACHINEADAM) || (coleco.SGM) ) coleco_setmemory(Data,coleco_port20,coleco_port53);
                      break;
        }
}

//---------------------------------------------------------------------------
BYTE coleco_readport(int Address, int *tstates)
{
        BYTE data = ReadInputPort(Address, tstates);
        //LogInAccess(Address, data);

        return data;
}

//---------------------------------------------------------------------------
BYTE ReadInputPort(int Address, int *tstates)
{
        // Coleco uses 8bit port addressing
        Address&=0x00FF;

        switch(Address&0xE0)
        {
                case 0x40: // Printer Status and SGM Module
                        if((coleco.machine == MACHINEADAM)&&(Address==0x40)) return(0xFF);
                        if((coleco.SGM)&&(Address==0x52))  return(ay8910_read());
                        break;

                case 0x20: // AdamNet Control
                        if (coleco.machine == MACHINEADAM) return(coleco_port20);
                        break;

                case 0x60: // Memory Control
                        if (coleco.machine == MACHINEADAM) return(coleco_port60);
                        break;

                case 0xE0: // Joysticks Data
                        Address = Address&0x02? (coleco_joystat>>16):coleco_joystat;
                        Address = coleco_joymode?   (Address>>8):Address;
                        return(~Address&0x7F);

                case 0xA0: // VDP Status/Data
                        return(Address&0x01? tms9918_readctrl():tms9918_readdata());
        }

        // No such port
        return idleDataBus;
}

//---------------------------------------------------------------------------
int coleco_contend(int Address, int states, int time)
{
        return(time);
}

//---------------------------------------------------------------------------
// do a Z80 instruction or frame
int coleco_do_scanline(void)
{
        int ts;
        int MaxScanLen,CurScanLine_len,CurScanLinesync_valid;
        int tstotal=0;

        CurScanLine_len=0;
        MaxScanLen = (coleco.singlestep ? 1:machine.tperscanline);

        CurScanLinesync_valid=0;

        try
        {
                do
                {
                        ts = z80_do_opcode();

                        frametstates += ts;
                        tStatesCount += ts;
                        CurScanLine_len += ts;

                        if (coleco_nmigenerate)
                        {
                                int nmilen;
                                nmilen = z80_nmi(CurScanLine_len);
                                ts += nmilen;
                                coleco_nmigenerate=0;
                        }

                        if (CurScanLine_len>=MaxScanLen)
                        {
                                // if end of line, update sound
                                //SoundUpdate(tms.CurLine);

                                // go to next line and check nmi
                                coleco_nmigenerate = tms9918_loop();
                                CurScanLine_len-=MaxScanLen;

                                // end of screen, update sound and go outside
                                if (tms.CurLine==(tms.ScanLines-1))
                                {
                                        SoundUpdate(tms.CurLine);
                                        CurScanLinesync_valid=1;
                                }

                        }

                        tstotal += ts;

                        DebugUpdate();
                } while(!CurScanLinesync_valid && !coleco.stop);
        }
        catch (Exception &exception)
        {
                // The default exception handler not only shows the exception that
                // occured, but also quits the message handler
                Application->ShowException(&exception);
        }

        return(tstotal);
}

//---------------------------------------------------------------------------
void Printer(BYTE V)
{
        printviewer->SendPrint(V);
}

//---------------------------------------------------------------------------
// Save a state file
BYTE coleco_savestate(char *filename)
{
        BYTE stateheader[24] = "emultwo state\032\0\0\0\0\0\0\0\0\0\0";
        unsigned int statesave[48];
        unsigned int statesize,i;
        BYTE *statebuf;
        FILE *fstatefile = NULL;


        // Allocate temporary buffer
        statebuf  = (BYTE *) malloc(MAXSTATESIZE);
        if(!statebuf)
                return(0);

        // Fill header with current crc (still have space for something else)
        stateheader[4] = coleco.cardcrc&0xFF;
        stateheader[5] = (coleco.cardcrc>>8)&0xFF;
        stateheader[6] = (coleco.cardcrc>>16)&0xFF;
        stateheader[7] = (coleco.cardcrc>>24)&0xFF;

        // Get memory pointers
        statesize=0;
        memset(statesave,0,sizeof(statesave));

        // Get coleco and machine info
        memcpy(statebuf+statesize,&coleco,sizeof(coleco)); statesize+=sizeof(coleco);
        memcpy(statebuf+statesize,&machine,sizeof(machine)); statesize+=sizeof(machine);

        // Get global variable memory location
        i=0;
        statesave[i++] = coleco_megapage;
        statesave[i++] = coleco_megasize;
        statesave[i++] = coleco_megacart;
        statesave[i++] = coleco_port20;
        statesave[i++] = coleco_port60;
        statesave[i++] = coleco_port53;
        statesave[i++] = coleco_joymode;
        statesave[i++] = coleco_joystat;
        statesave[i++] = coleco_spincount;
        statesave[i++] = coleco_spinstep;
        statesave[i++] = coleco_spinstate;
        statesave[i++] = coleco_nmigenerate;
        statesave[i++] = coleco_updatetms;
        statesave[i++] = tstates;
        statesave[i++] = frametstates;
        statesave[i++] = tStatesCount;
        statesave[i++] = (int) (ROMPage[0]-RAM); statesave[i++] = (int) (ROMPage[1]-RAM); statesave[i++] = (int) (ROMPage[2]-RAM);
        statesave[i++] = (int) (ROMPage[3]-RAM); statesave[i++] = (int) (ROMPage[4]-RAM); statesave[i++] = (int) (ROMPage[5]-RAM);
        statesave[i++] = (int) (ROMPage[6]-RAM); statesave[i++] = (int) (ROMPage[7]-RAM);
        statesave[i++] = (int) (RAMPage[0]-RAM); statesave[i++] = (int) (RAMPage[1]-RAM); statesave[i++] = (int) (RAMPage[2]-RAM);
        statesave[i++] = (int) (RAMPage[3]-RAM); statesave[i++] = (int) (RAMPage[4]-RAM); statesave[i++] = (int) (RAMPage[5]-RAM);
        statesave[i++] = (int) (RAMPage[6]-RAM); statesave[i++] = (int) (RAMPage[7]-RAM);
        memcpy(statebuf+statesize,&statesave,sizeof(statesave));statesize+=sizeof(statesave);

        // Save Z80 CPU
        memcpy(statebuf+statesize,&z80,sizeof(z80)); statesize+=sizeof(z80);
        memcpy(statebuf+statesize,&sz53_table,0x100); statesize+=0x100;
        memcpy(statebuf+statesize,&parity_table,0x100); statesize+=0x100;
        memcpy(statebuf+statesize,&sz53p_table,0x100); statesize+=0x100;

        // Save VDP
        memcpy(statebuf+statesize,&tms,sizeof(tms)); statesize+=sizeof(tms);

        // Save sound
        memcpy(statebuf+statesize,&sn,sizeof(sn)); statesize+=sizeof(sn);
        memcpy(statebuf+statesize,&tms,sizeof(ay)); statesize+=sizeof(ay);

        // Save memory
        memcpy(statebuf+statesize,&statesave,sizeof(statesave)); statesize+=sizeof(statesave);
        memcpy(statebuf+statesize,cvmemory,MAXRAMSIZE);statesize+=MAXRAMSIZE;
        memcpy(statebuf+statesize,eepromdata,MAXEEPROMSIZE);statesize+=MAXEEPROMSIZE;

        // Open new state file
        fstatefile = fopen(filename,"wb");
        if(!fstatefile)
        {
                free(statebuf);return(0);
        }

        // Write out the header and data
        if (fwrite(stateheader,1,24,fstatefile)!=24)
        {
                fclose(fstatefile);fstatefile=NULL;
        }
        if ((fstatefile!=NULL) && (fwrite(statebuf,1,statesize,fstatefile)!=statesize))
        {
                fclose(fstatefile);fstatefile=NULL;
        }

        // If failed writing state, delete open file
        if(fstatefile) fclose(fstatefile); else unlink(filename);

        // Done
        free(statebuf);
        return(1);
}

//---------------------------------------------------------------------------
// Load a state file
BYTE coleco_loadstate(char *filename)
{
#if 0
  byte Header[16],*Buf;
  int Size,OldMode,J;
  FILE *F;

  /* Fail if no state file */
  if(!Name) return(0);

  /* Open saved state file */
  if(!(F=fopen(Name,"rb"))) return(0);

  /* Read and check the header */
  if(fread(Header,1,16,F)!=16)       { fclose(F);return(0); }
  if(memcmp(Header,"STF\032\002",5)) { fclose(F);return(0); }
  J = LastCRC;
  if(
    (Header[5]!=(Mode&CV_ADAM)) ||
    (Header[6]!=(J&0xFF))       ||
    (Header[7]!=((J>>8)&0xFF))  ||
    (Header[8]!=((J>>16)&0xFF)) ||
    (Header[9]!=((J>>24)&0xFF))
  ) { fclose(F);return(0); }

  /* Allocate temporary buffer */
  Buf = malloc(MAX_STASIZE);
  if(!Buf) { fclose(F);return(0); }

  /* Read state into temporary buffer, then load it */
  OldMode = Mode;
  Size    = fread(Buf,1,MAX_STASIZE,F);
  Size    = Size>0? LoadState(Buf,Size):0;

  /* If failed loading state, reset hardware */
  if(!Size) ResetColeco(OldMode);

  /* Done */
  free(Buf);
  fclose(F);
#endif
        return(1);
}

