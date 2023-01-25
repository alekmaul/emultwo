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
 * emul2.cpp
 * Based on   ColEm for some memory management (https://fms.komkon.org/ColEm/)
 * Based on ColecoDS for memory read/write (https://github.com/wavemotion-dave/ColecoDS/)
 *
 */
#include <vcl.h>
#include <stdio.h>

#include "coleco.h"
#include "accdraw_.h"
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
#include "soundviewer_.h"

extern void DebugUpdate(void);

//---------------------------------------------------------------------------
BYTE cv_display[TVW*TVH];                       // Coleco display buffer
BYTE cv_palette[16*3];                          // Coleco display palette
int cv_pal32[16];                               // Coleco display palette in 32 bits RGB
unsigned int *cv_screen[TVH];                   // Coleco display buffer in 32 bits RGB

BYTE ROM_Memory[MAX_CART_SIZE * 1024];          // ROM Carts up to 512K
BYTE RAM_Memory[MAX_RAM_SIZE * 1024];           // RAM up to 128K (for the ADAM... )
BYTE BIOS_Memory[MAX_BIOS_SIZE * 1024];         // 64K To hold our BIOS and related OS memory
BYTE SRAM_Memory[MAX_EEPROM_SIZE*1024];         // SRAM up to 32K for the few carts which use it

BYTE *MemoryMap[8];                             // 8x8kB pages

unsigned int sgm_low_addr;                      // SGM lower address (bios or RAM)
BYTE sgm_neverenable;                           // TO avoid SGM for superDK
BYTE sgm_enable;                                // 1 if port put SGM on
BYTE sgm_firstwrite;                            // 1 if 1st time we enable SGM (to init memory)

BYTE coleco_port20;                             // Adam port 0x20-0x3F (AdamNet)
BYTE coleco_port60;                             // Adam port 0x60-0x7F (memory)
BYTE coleco_port53;                             // SGM port 0x53 (memory)

BYTE adam_ram_lo;                               // Adam memory ram lo & high
BYTE adam_ram_hi;
BYTE adam_ram_lo_exp;
BYTE adam_ram_hi_exp;
BYTE adam_128k_mode;                            // Adam RAM is 128K (64K Intrinsic, 64K Expanded)

BYTE coleco_megabank;                           // selected bank for megacart
BYTE coleco_megasize;                           // mega cart rom size in 16kB pages
BYTE coleco_megacart;                           // <>0 if mega cart rom detected

BYTE coleco_joymode;                            // Joystick / Paddle management
unsigned int coleco_joystat;                    // Joystick / Paddle management

int coleco_spinpos;                             // Spinner position
unsigned int coleco_spincount;                  // Spinner counters
unsigned int coleco_spinstep;                   // Spinner steps
unsigned int coleco_spinstate;                  // Spinner bit states

int tstates,frametstates;                       // count of instruction  times
int tStatesCount;                               // count of instruction  times

int coleco_updatetms=0;                         // 1 if TMS is updated

FDIDisk Disks[MAX_DISKS] = { 0 };               // Adam disk drives
FDIDisk Tapes[MAX_TAPES] = { 0 };               // Adam tape drives

static BYTE idleDataBus = 0xFF;

const unsigned char TMS9918A_palette[6*16*3] = {
        // Coleco palette
        24,24,24, 0,0,0, 33,200,66, 94,220,120, 84,85,237, 125,118,252, 212,82,77, 66,235,245,
        252,85,84, 255,121,120, 212,193,84, 230,206,128, 33,176,59, 201,91,186, 204,204,204, 255,255,255,

        // Adam palette
        0,  0,  0,    0,  0,  0,   71,183, 59,  124,207,111,   93, 78,255,  128,114,255,  182, 98, 71,   93,200,237,
        215,107, 72,  251,143,108,  195,205, 65,  211,218,118, 62,159, 47,  182,100,199,  204,204,204,  255,255,255,

        // TMS9918 Palette
        24,24,24, 0,8,0, 0,241,1, 50,251,65, 67,76,255, 112,110,255, 238,75,28, 9,255,255,
        255,78,31, 255,112,65, 211,213,0, 228,221,52, 0,209,0, 219,79,211, 193,212,190, 244,255,241,

        // black and white
        0,  0,  0,    0,  0,  0,  136,136,136,  172,172,172, 102,102,102,  134,134,134,  120,120,120,  172,172,172,
        136,136,136,  172,172,172,  187,187,187,  205,205,205, 118,118,118,  135,135,135,  204,204,204,  255,255,255,

        // Green scales
        0,  0,  0,    0,  0,  0,    0,118,  0,   43,153, 43, 0, 81,  0,    0,118,  0,   43, 81, 43,   43,153, 43,
        43, 81, 43,   43,118, 43,   43,153, 43,   43,187, 43, 43, 81, 43,   43,118, 43,   43,221, 43,    0,255,  0,

        // Ambre scale
        0,  0,  0,    0,  0,  0,  118, 81, 43,  153,118,  0, 81, 43,  0,  118, 81,  0,   81, 43,  0,  187,118, 43,
        118, 81,  0,  153,118, 43,  187,118, 43,  221,153,  0, 118, 81,  0,  153,118, 43,  221,153,  0,  255,187,  0
};
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
    case SGMRAM:
        result = RAM_Memory[addr];
        break;
    case RAM:
        result = RAM_Memory[addr];
        break;
    case EEPROM:
        result = SRAM_Memory[addr];
        break;
    case SRAM:
        result = RAM_Memory[0xE800+addr];
        break;
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
    case SGMRAM:
        RAM_Memory[addr] = y;
        break;
    case RAM:
        addr&=0x03FF;
        RAM_Memory[addr]       =RAM_Memory[0x0400+addr]=
        RAM_Memory[0x0800+addr]=RAM_Memory[0x0C00+addr]=
        RAM_Memory[0x1000+addr]=RAM_Memory[0x1400+addr]=
        RAM_Memory[0x1800+addr]=RAM_Memory[0x1C00+addr]=y;
        break;
    case ROM:
        RAM_Memory[addr]=y;
        break;
    case EEPROM:
        SRAM_Memory[addr]=y;
        break;
    case SRAM:
        addr&=0x07FF;
        RAM_Memory[0xE800+addr]=y;
        break;
    }
}

//---------------------------------------------------------------------------
// Load a rom file
BYTE coleco_loadcart(char *filename, unsigned char cardtype)
{
    int adrlastbank, j, size;
    BYTE  *p;
    BYTE retf = ROM_LOAD_FAIL;
    FILE *fRomfile = NULL;

    // load rom file
    fRomfile = fopen(filename, "rb");
    if (fRomfile == NULL)
        return(retf);


    // Ensure our rom buffer is clear (0xFF to simulate unused memory on ROM/EE though probably 0x00 would be fine too)
    memset(ROM_Memory, 0xFF, (MAX_CART_SIZE * 1024));

    // Determine size via ftell()
    fseek(fRomfile, 0, SEEK_END);
    size = ftell(fRomfile);

    // validate size
    if (size <= (MAX_CART_SIZE * 1024))
    {
        // load rom in memory
        fseek(fRomfile, 0, SEEK_SET);
        fread((void*) ROM_Memory, size, 1, fRomfile);
        fclose(fRomfile);

        // Init megacart info
        coleco_megacart = 0x00;
        coleco_megasize = 2;

        // Keep initial cartridge CRC (may change after SRAM writes) and do CRC for special games
        emul2.cardsize=size;
        emul2.cardcrc=CRC32Block(ROM_Memory,emul2.cardsize);

        // Check default magic heard
        p = (ROM_Memory[0]==0x55)&&(ROM_Memory[1]==0xAA)? RAM_Memory
            : (ROM_Memory[0]==0xAA)&&(ROM_Memory[1]==0x55)? RAM_Memory
            : (ROM_Memory[0]==0x66)&&(ROM_Memory[1]==0x99)? RAM_Memory
            : NULL;

        // Check magic header for Magecarts if not yet found at beginning (for 64K eeprom roms)
        adrlastbank = (size&~0x3FFF)-0x4000;
        if (p==NULL)
        {
            p = (ROM_Memory[adrlastbank]==0x55)&&(ROM_Memory[adrlastbank+1]==0xAA)? RAM_Memory
                : (ROM_Memory[adrlastbank]==0xAA)&&(ROM_Memory[adrlastbank+1]==0x55)? RAM_Memory
                : NULL;
        }
        if (p==NULL) { fclose(fRomfile);return(ROM_VERIFY_FAIL); }

        // Point to ram address
        p = RAM_Memory+0x8000;

        // Do we fit within the standard 32K Colecovision Cart ROM memory space?
        if (size <= 32*1024) // otherwise 32K limit
        {
            memcpy(p, ROM_Memory, size);
        }
        // No - must be Mega Cart (MC) Bankswitched!!
        else
        {
            // Force load of the first bank when asked to bankswitch
            coleco_megabank = 199;

            // Pad to the nearest 16kB and find number of 16kB pages
            size = ((size+0x3FFF)&~0x3FFF)>>14;

            // Round page number up to the nearest power of two
            for(j=2;j<size;j<<=1);

            // Set new MegaROM size
            size = j<<14;
            coleco_megasize = j;

            // Calculate size
            if (size == (64  * 1024)) coleco_megacart = 0x03;
            else if (size == (128 * 1024)) coleco_megacart = 0x07;
            else if (size == (256 * 1024)) coleco_megacart = 0x0F;
            else /*if (size == (512 * 1024)) */ coleco_megacart = 0x1F; // max 512

            // For MegaCart, we map highest 16K bank into fixed ROM
            memcpy(p,ROM_Memory+(coleco_megacart<<14),0x4000);
            memcpy(p+0x4000,ROM_Memory,0x4000);
        }
    }
    else
    {
        fclose(fRomfile);return(retf);
    }

    emul2.romCartridge = coleco_megacart ? ROMCARTRIDGEMEGA : ROMCARTRIDGESTD;

    // Close rom file
    fclose(fRomfile);

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
        }
        RenderCalcPalette(cv_palette);
}
//---------------------------------------------------------------------------

// Setup Adam based on Port60 (Adam Memory) and Port20 (AdamNet)
// Most of this hinges around Port60:
// xxxx xxNN  : Lower address space code.
//       00 = Onboard ROM.  Can be switched between EOS and SmartWriter by output to port 0x20
//       01 = Onboard RAM (lower 32K)
//       10 = Expansion RAM.  Bank switch chosen by port 0x42
//       11 = OS-7 and 24K RAM (ColecoVision mode)
//
// xxxx NNxx  : Upper address space code.
//       00 = Onboard RAM (upper 32K)
//       01 = Expansion ROM (those extra ROM sockets)
//       10 = Expansion RAM.  Bank switch chosen by port 0x42
//       11 = Cartridge ROM (ColecoVision mode).
//
// And Port20: bit 1 (0x02) to determine if EOS.ROM is present on top of WRITER.ROM
void coleco_setadammemory(bool resetAdamNet)
{
    if (emul2.machine != MACHINEADAM) return;

    // Configure lower 32K of memory
    // -> WRITER.ROM (and possibly EOS.ROM)
    if ((coleco_port60 & 0x03) == 0x00)
    {
        adam_ram_lo = 0;
        adam_ram_lo_exp = 0;
        MemoryMap[0] = BIOS_Memory + 0x0000;
        MemoryMap[1] = BIOS_Memory + 0x2000;
        MemoryMap[2] = BIOS_Memory + 0x4000;
        // EOS
        if (coleco_port20 & 0x02)
        {
            MemoryMap[3] = BIOS_Memory + 0x8000;
        }
        // Lst block of Adam WRITER
        else
        {
            MemoryMap[3] = BIOS_Memory + 0x6000;
        }
    }
    // -> Onboard RAM
    else if ((coleco_port60 & 0x03) == 0x01)
    {
        adam_ram_lo = 1;
        adam_ram_lo_exp = 0;
        MemoryMap[0] = RAM_Memory + 0x0000;
        MemoryMap[1] = RAM_Memory + 0x2000;
        MemoryMap[2] = RAM_Memory + 0x4000;
        MemoryMap[3] = RAM_Memory + 0x6000;
    }
    // -> Colecovision BIOS + RAM
    else if ((coleco_port60 & 0x03) == 0x03)
    {
        adam_ram_lo = 1;
        adam_ram_lo_exp = 0;
        MemoryMap[0] = BIOS_Memory + 0xA000;    // Coleco.rom BIOS
        MemoryMap[1] = RAM_Memory + 0x2000;
        MemoryMap[2] = RAM_Memory + 0x4000;
        MemoryMap[3] = RAM_Memory + 0x6000;
    }
    // -> Expanded RAM
    else
    {
        adam_128k_mode = 1;
        adam_ram_lo = 0;
        adam_ram_lo_exp = 1;
        MemoryMap[0] = RAM_Memory + 0x10000;
        MemoryMap[1] = RAM_Memory + 0x12000;
        MemoryMap[2] = RAM_Memory + 0x14000;
        MemoryMap[3] = RAM_Memory + 0x16000;
    }


    // Configure upper 32K of memory
    // -> Onboard RAM
    if ((coleco_port60 & 0x0C) == 0x00)
    {
        adam_ram_hi = 1;
        adam_ram_hi_exp = 0;
        MemoryMap[4] = RAM_Memory + 0x8000;
        MemoryMap[5] = RAM_Memory + 0xA000;
        MemoryMap[6] = RAM_Memory + 0xC000;
        MemoryMap[7] = RAM_Memory + 0xE000;
    }
    // -> Expanded RAM
    else if ((coleco_port60 & 0x0C) == 0x08)
    {
        adam_128k_mode = 1;
        adam_ram_hi = 0;
        adam_ram_hi_exp = 1;
        MemoryMap[4] = RAM_Memory + 0x18000;
        MemoryMap[5] = RAM_Memory + 0x1A000;
        MemoryMap[6] = RAM_Memory + 0x1C000;
        MemoryMap[7] = RAM_Memory + 0x1E000;
    }
    // Nothing else exists so just return 0xFF
    else
    {
        adam_ram_hi = 0;
        adam_ram_hi_exp = 0;
    }

    // Check if we are to Reset the AdamNet
    if (resetAdamNet)  ResetPCB();
}
// --------------------------------------------------------------------------

// Based on writes to Port53 and Port60 we configure the SGM handling of memory...
//  this includes 24K vs 32K of RAM (the latter is BIOS disabled).
void coleco_setupsgm(void)
{
    // There are a couple of games were we don't want to enable the SGM. Most notably Super DK won't play with SGM emulation.
    if (sgm_neverenable) return;
    if (emul2.machine == MACHINEADAM) return;

    // Port 53 lowest bit dictates SGM memory support enable.
    sgm_enable = (coleco_port53 & 0x01) ? 1:0;

    // The first time we enable the SGM expansion RAM, we clear it out
    if (sgm_enable && sgm_firstwrite)
    {
        memset(RAM_Memory+0x2000, 0x00, 0x6000);
        sgm_firstwrite = 0;
    }

    // Port 60 will tell us if we want to swap out the lower 8K bios for more RAM (total of 32K RAM for SGM)
    // Since this can swap back and forth (not sure if any game really does this), we need to preserve that 8K
    // when we switch back and forth...
    if (coleco_port60 & 0x02)
    {
        if (sgm_low_addr != 0x2000)
        {
            sgm_low_addr = 0x2000;
            MemoryMap[0] = BIOS_Memory + 0x0000;
        }
    }
    else
    {
        // Force this if someone disabled the BIOS.... based on reading some comments in the AA forum...
        sgm_enable = 1;
        if (sgm_low_addr != 0x0000)
        {
            MemoryMap[0] = RAM_Memory + 0x0000;
            sgm_low_addr = 0x0000;
        }
    }
}
//---------------------------------------------------------------------------

void coleco_reset(void)
{
    int i;

    // Init memory pages. By default, use a simple flat 64K memory model
    MemoryMap[0] = RAM_Memory + 0x0000;
    MemoryMap[1] = RAM_Memory + 0x2000;
    MemoryMap[2] = RAM_Memory + 0x4000;
    MemoryMap[3] = RAM_Memory + 0x6000;
    MemoryMap[4] = RAM_Memory + 0x8000;
    MemoryMap[5] = RAM_Memory + 0xA000;
    MemoryMap[6] = RAM_Memory + 0xC000;
    MemoryMap[7] = RAM_Memory + 0xE000;

    // Be sure to have bios in Coleco mode
    if (emul2.machine != MACHINEADAM)
    {
        // Load COLECO.ROM ColecoVision BIOS
        memcpy(RAM_Memory,colecobios_rom,0x2000);

        // Hack 50/60 Bios & NO delay Bios
        RAM_Memory[0x0069]=emul2.hackbiospal ? 50 : 60;
        if (emul2.biosnodelay)
        {
            RAM_Memory[159*32+17]=0x00;
            RAM_Memory[159*32+18]=0x00;
            RAM_Memory[159*32+19]=0x00;
        }
    }

    // Clear memory (important for NetPlay, to keep states at both sides consistent)
    // Clearing to zeros (Heist) TODO
    //memset(RAM_Memory+0x6000,0xFF,0x2000);
    for (i=0;i<0x2000;i++)
        RAM_Memory[i+0x6000]=rand()%256;

    sgm_enable=0;
    sgm_firstwrite=1;
    sgm_low_addr = 0x2000;       // And the first 8K is BIOS
    sgm_neverenable=0;

    // Init the SGM Port 53,Adam/Memory Port 60 and AdamNet
    coleco_port53 = 0x00;
    coleco_port60 = ((emul2.machine == MACHINEADAM) ? 0x00:0x0F);
    coleco_port20 = 0x00;

    // Init Adam for memory mapping
    if (emul2.machine == MACHINEADAM)
    {
        adam_ram_lo=adam_ram_hi=adam_ram_lo_exp=adam_ram_hi_exp=0;
        adam_128k_mode=0; // Normal 64K ADAM to start
        coleco_setadammemory(false);
    }

    // check for SRAM && EEPROM and force type
    switch (emul2.cardcrc)
    {
    case 0x62DACF07:    // Boxxle 32kB EEPROM
        emul2.typebackup=EEP24C256;
        break;
    case 0xDDDD1396:    // Black Onyx 256-byte EEPROM
        emul2.typebackup=EEP24C08;
        break;
    case 0xFEE15196:    // Lord Of The Dungeon 24kB ROM + 8kB garbage + 2kB SRAM
    case 0x1053F610:    // Lord Of The Dungeon 24kB ROM + 2kB SRAM
    case 0x60D6FD7D:    // Lord Of The Dungeon 24kB ROM + 2kB SRAM
    case 0x37A9F237:    // Lord Of The Dungeon 24kB ROM + 2kB SRAM
        emul2.typebackup=EEPSRAM;
        break;
    case 0xEF25AF90:    // Super DK Prototype - ignore any SGM/Adam Writes
    case 0xC2E7F0E0:    // Super DK JR Prototype - ignore any SGM/Adam Writes
        sgm_neverenable=1;
        break;
    }

    // Reset hardware and CPU
    tms9918_reset();
    tms.ScanLines=emul2.NTSC ? TMS9918_LINES : TMS9929_LINES;

    sn76489_init(emul2.NTSC ? CLOCK_NTSC : CLOCK_PAL, 44100);
    if (emul2.SGM)
    {
        ay8910_init(emul2.NTSC ? CLOCK_NTSC : CLOCK_PAL, 44100);
    }
    SoundPrepSmpTab(tms.ScanLines);
    if (emul2.typebackup!=NOBACKUP)
        c24xx_reset(SRAM_Memory,emul2.typebackup==EEP24C08 ? C24XX_24C08 : emul2.typebackup==EEP24C256 ? C24XX_24C256 : 0);
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

    // Init rom banking
    coleco_megasize = 2;
    coleco_megacart = 0;
    emul2.romCartridge == ROMCARTRIDGENONE;

        //ResetLastIOAccesses();

    // Init all rom memory
    memset(ROM_Memory,0xFF,MAX_CART_SIZE * 1024);
    memset(RAM_Memory,0xFF,MAX_RAM_SIZE * 1024);

    // Put bios in memory
    if (emul2.machine == MACHINEADAM)
    {
        // Load COLECO.ROM: OS7 (ColecoVision BIOS)
        memcpy(BIOS_Memory+0xA000, colecobios_rom, 0x2000);

        // Load EOS.ROM: EOS (Adam BIOS)
        memcpy(BIOS_Memory+0x8000, adambios_eos,0x2000);

        // Load WRITER.ROM: SmartWriter (Adam bootup)
        memcpy(BIOS_Memory+0x0000, adambios_writer, 0x8000);
    }
    else
    {
        // Load COLECO.ROM ColecoVision BIOS
        memcpy(RAM_Memory,colecobios_rom,0x2000);

        // Hack 50/60 Bios & NO delay Bios
        RAM_Memory[0x0069]=emul2.hackbiospal ? 50 : 60;
        if (emul2.biosnodelay)
        {
            RAM_Memory[159*32+17]=0x00;
            RAM_Memory[159*32+18]=0x00;
            RAM_Memory[159*32+19]=0x00;
        }
    }

    // Unload any ADAM related stuff
    for(i=0;i<MAX_DISKS;i++) EjectFDI(&Disks[i]);
    for(i=0;i<MAX_TAPES;i++) EjectFDI(&Tapes[i]);

    // Reset coleco
    coleco_reset();

    // Prepare Coleco palette
    coleco_setpalette(emul2.palette);
}
//---------------------------------------------------------------------------

// Switch banks. Up to 512K of the Colecovision Mega Cart ROM can be stored
void megacart_bankswitch(BYTE bank)
{
    // Only if the bank was changed...
    if (coleco_megabank != bank)
    {
        MemoryMap[6] = ROM_Memory + ((unsigned int) bank * 0x4000);
        MemoryMap[7] = MemoryMap[6] + 0x2000;
        coleco_megabank = bank;
    }
}
//---------------------------------------------------------------------------

void coleco_WriteByte(int Address, int Data)
{
    // If the ADAM is enabled, we may be trying to write to AdamNet
    if(emul2.machine == MACHINEADAM)
    {
        if ((Address < 0x8000) && adam_ram_lo)        {RAM_Memory[Address] = Data; if  (PCBTable[Address]) WritePCB(Address, Data);}
        else if ((Address >= 0x8000) && adam_ram_hi)  {RAM_Memory[Address] = Data; if  (PCBTable[Address]) WritePCB(Address, Data);}

        if ((Address < 0x8000) && adam_ram_lo_exp)        {RAM_Memory[0x10000 + Address] = Data;}
        else if ((Address >= 0x8000) && adam_ram_hi_exp)  {RAM_Memory[0x10000 + Address] = Data;}
        return;
    }
    else
    {
        // if SGM enable, we have a much wider range of RAM that can be written (and no mirroring)
        if (sgm_enable)
        {
            if ((Address >= sgm_low_addr) && (Address < 0x8000)) RAM_Memory[Address]=Data;
            return;
        }
        // Normal memory RAM write... with mirrors...
        else if((Address>0x5FFF)&&(Address<0x8000))
        {
            Address&=0x03FF;
            RAM_Memory[0x6000+Address]=RAM_Memory[0x6400+Address]=
            RAM_Memory[0x6800+Address]=RAM_Memory[0x6C00+Address]=
            RAM_Memory[0x7000+Address]=RAM_Memory[0x7400+Address]=
            RAM_Memory[0x7800+Address]=RAM_Memory[0x7C00+Address]=Data;
            return;
        }
        // Allow SRAM if cart doesn't extend this high...
        else if ((Address >= 0xE000) && (Address < 0xE800))
        {
            // SRAM at E800h..EFFFh, writable via E000h..E7FFh
            if (emul2.typebackup==EEPSRAM)
            {
                RAM_Memory[Address+0x800]=Data;
                return;
            }
        }
        // Cartridges, containing EEPROM, use [1111 1111 11xx 0000] addresses
        else if ( (emul2.typebackup==EEP24C08) || (emul2.typebackup==EEP24C256) )
        {
            if ((Address == 0xFF90) || (Address == 0xFFA0) || (Address == 0xFFB0))
            {
                megacart_bankswitch((Address>>4) & coleco_megacart);
            }

            switch(Address)
            {
                case 0xFFC0: c24xx_write(c24.Pins&~C24XX_SCL);return;
                case 0xFFD0: c24xx_write(c24.Pins|C24XX_SCL);return;
                case 0xFFE0: c24xx_write(c24.Pins&~C24XX_SDA);return;
                case 0xFFF0: c24xx_write(c24.Pins|C24XX_SDA);return;
            }
            return;
        }
        else if (Address >= 0xFFC0)
        {
            megacart_bankswitch(Address & coleco_megacart/*romBankMask*/);  // Handle Megacart Hot Spot writes (don't think anyone actually uses this but it's possible)
            return;
        }
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
        coleco_WriteByte(Address, Data);
}

void coleco_writebyte(int Address, int Data)
{
        lastMemoryWriteAddrLo = lastMemoryWriteAddrHi;
        lastMemoryWriteAddrHi = Address;

        lastMemoryWriteValueLo = lastMemoryWriteValueHi;
        lastMemoryWriteValueHi = Data;

        coleco_WriteByte(Address, Data);
}

BYTE coleco_ReadByte(int Address)
{
    // Handle Megacart Hot Spots
    if (coleco_megacart)
    {
        if (Address >= 0xFFC0)
        {
            // Only if the bank was changed...
            megacart_bankswitch(Address & coleco_megacart);
            return (coleco_megabank);
        }
    }
    // Activison PCB   type EEPROM
    else if ((emul2.typebackup==EEP24C08) || (emul2.typebackup==EEP24C256))
    {
        if (Address==0xFF80)
        {
            // Return EEPROM output bit
            return(c24xx_read());
        }
    }

    // Adam may try reading AdamNet
    if ((emul2.machine == MACHINEADAM)&&PCBTable[Address]) ReadPCB(Address);

    // return data
    return (*(MemoryMap[Address>>13] + (Address&0x1FFF)));
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

// Given an address, opcode fetch returns the byte at that memory address
// Called by Z80 instruction opcode fetches
BYTE coleco_opcode_fetch(int Address)
{
        BYTE byte = coleco_ReadByte(Address);
        return byte;
}

//---------------------------------------------------------------------------
void coleco_writeport(int Address, int Data, int *tstates)
{
    bool resetadam;

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
        else tms9918_writectrl(Data);
/*        {
            if (tms9918_writectrl(Data))
            {
                z80_set_irq_line(INPUT_LINE_NMI, ASSERT_LINE);
            }
            else
                z80_set_irq_line(INPUT_LINE_NMI, CLEAR_LINE);
        }*/
        break;
    case 0x40:
        if((emul2.machine == MACHINEADAM)&&(Address==0x40)) Printer(Data);
        else if(emul2.SGM)
        {
            if(Address==0x53) { coleco_port53 = Data; coleco_setupsgm(); } 
            else if (Address==0x50) ay8910_write(0,Data); // Control data
            else if (Address==0x51) ay8910_write(1,Data); // Write data
        }
        break;
    case 0x20:
        resetadam=(coleco_port20 & 1) && ((Data & 1) == 0);
        coleco_port20=Data;
        if (emul2.machine == MACHINEADAM) coleco_setadammemory(resetadam);
        else if(emul2.SGM) coleco_setupsgm();
        break;
    case 0x60:
        coleco_port60=Data;
        if (emul2.machine == MACHINEADAM) coleco_setadammemory(resetadam);
        else if (emul2.SGM) coleco_setupsgm();
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
        if((emul2.machine == MACHINEADAM)&&(Address==0x40)) return(0xFF);
        if((emul2.SGM)&&(Address==0x52))  return(ay8910_read());
        break;
    case 0x20: // AdamNet Control
        if (emul2.machine == MACHINEADAM) return(coleco_port20);
        break;
    case 0x60: // Memory Control
        if (emul2.machine == MACHINEADAM) return(coleco_port60);
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
    int MaxScanLen,CurScanLine_len;
    int tstotal=0;

    MaxScanLen = (emul2.singlestep ? 1:machine.tperscanline);
    CurScanLine_len=MaxScanLen;

    try
    {
        while (!emul2.stop)
        {

            // Check nmi before each line
            z80_checknmi();

            // For each line
            do
            {
                // fetch specific coleco bios address:
                // -> add sound to play
                if (Z80.pc.w.l==0x1ff1) { if (soundviewer->Visible) soundviewer->addsnplay(Z80.bc.w.l); }

                // do current opcode
                ts = z80_do_opcode();
                CurScanLine_len -= ts;


                frametstates += ts;
                tStatesCount += ts;

                tstotal += ts;

                DebugUpdate();
            } while(CurScanLine_len>0 && !emul2.stop && !emul2.singlestep);

            // if end of line, update sound
            if (CurScanLine_len<=0)
            {
                CurScanLine_len+=MaxScanLen;
                SoundUpdate(tms.CurLine);

                // go to next line and check nmi
                if (tms9918_loop())
                    z80_set_irq_line(INPUT_LINE_NMI, ASSERT_LINE);

                // end of screen, update sound and go outside
                if (tms.CurLine==TMS9918_END_LINE /*tms.ScanLines-1*/)
                {
                    //SoundUpdate(tms.ScanLines-1);
                    break;
                }
            }
            if (emul2.singlestep)
                break;
        }
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
    BYTE stateheader[24] = "emultwo state\032\1\0\0\0\0\0\0\0\0\0";
    unsigned int statesave[32];
    unsigned int statesize,i;
    BYTE *statebuf;
    FILE *fstatefile = NULL;


    // Allocate temporary buffer
    statebuf  = (BYTE *) malloc(MAXSTATESIZE*1024);
    if(!statebuf)
        return(0);

    // Fill header with current crc (still have space for something else)
    stateheader[18] = emul2.cardcrc&0xFF;
    stateheader[19] = (emul2.cardcrc>>8)&0xFF;
    stateheader[20] = (emul2.cardcrc>>16)&0xFF;
    stateheader[21] = (emul2.cardcrc>>24)&0xFF;

    // Get memory pointers
    statesize=0;
    memset(statesave,0,sizeof(statesave));

    // Get coleco and machine info
    memcpy(statebuf+statesize,&emul2,sizeof(emul2)); statesize+=sizeof(emul2);
    memcpy(statebuf+statesize,&machine,sizeof(machine)); statesize+=sizeof(machine);

    // Get global variable memory location
    i=0;
    statesave[i++] = coleco_megasize;
    statesave[i++] = coleco_megacart;
    statesave[i++] = coleco_megabank;
    statesave[i++] = coleco_port20;
    statesave[i++] = coleco_port60;
    statesave[i++] = coleco_port53;
    statesave[i++] = coleco_joymode;
    statesave[i++] = coleco_joystat;
    statesave[i++] = coleco_spincount;
    statesave[i++] = coleco_spinstep;
    statesave[i++] = coleco_spinstate;
    statesave[i++] = coleco_updatetms;
    statesave[i++] = tstates;
    statesave[i++] = frametstates;
    statesave[i++] = tStatesCount;
       statesave[i++] = (int) (MemoryMap[0]); statesave[i++] = (int) (MemoryMap[1]); statesave[i++] = (int) (MemoryMap[2]);
    statesave[i++] = (int) (MemoryMap[3]); statesave[i++] = (int) (MemoryMap[4]); statesave[i++] = (int) (MemoryMap[5]);
    statesave[i++] = (int) (MemoryMap[6]); statesave[i++] = (int) (MemoryMap[7]);
    memcpy(statebuf+statesize,&statesave,sizeof(statesave));statesize+=sizeof(statesave);

    // Save Z80 CPU
    memcpy(statebuf+statesize,&Z80,sizeof(Z80)); statesize+=sizeof(Z80);

    // Save VDP
    memcpy(statebuf+statesize,&tms,sizeof(tms)); statesize+=sizeof(tms);

    // Save sound
    memcpy(statebuf+statesize,&sn,sizeof(sn)); statesize+=sizeof(sn);
    memcpy(statebuf+statesize,&ay,sizeof(ay)); statesize+=sizeof(ay);

    // Save memory
    memcpy(statebuf+statesize,ROM_Memory,MAX_CART_SIZE*1024);statesize+=MAX_CART_SIZE*1024;
    memcpy(statebuf+statesize,RAM_Memory,MAX_RAM_SIZE*1024);statesize+=MAX_RAM_SIZE*1024;
    memcpy(statebuf+statesize,SRAM_Memory,MAX_EEPROM_SIZE*1024);statesize+=MAX_EEPROM_SIZE*1024;

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
    BYTE stateheader[24];
    unsigned int statesave[32];
    unsigned int statesize,i;
    BYTE *statebuf;
    FILE *fstatefile = NULL;

    // Allocate temporary buffer
    statebuf  = (BYTE *) malloc(MAXSTATESIZE);
    if(!statebuf)
        return(0);

    // Open saved state file
    fstatefile=fopen(filename,"rb");
    if (fstatefile==NULL) return(0);

    // Read and check the header
    if (fread(stateheader,1,24,fstatefile)!=24)
    {
        fclose(fstatefile);return(0);
    }
    if(memcmp(stateheader,"emultwo state\032\1\0\0",17))
    {
        fclose(fstatefile);return(0);
    }

    // Read state into temporary buffer, then load it
    statesize=fread(statebuf,1,MAXSTATESIZE,fstatefile);

    // If failed loading state, reset hardware
    if(!statesize)
        machine.initialise();
    else
    {
        statesize=0;

        // Get coleco and machine info
        memcpy(&emul2,statebuf+statesize,sizeof(emul2)); statesize+=sizeof(emul2);
        memcpy(&machine,statebuf+statesize,sizeof(machine)); statesize+=sizeof(machine);

        // Get global variable memory location
        memcpy(&statesave,statebuf+statesize,sizeof(statesave));statesize+=sizeof(statesave);
        i=0;
        coleco_megasize=statesave[i++];
        coleco_megacart=statesave[i++];
        coleco_megabank=statesave[i++];
        coleco_port20=statesave[i++];
        coleco_port60=statesave[i++];
        coleco_port53=statesave[i++];
        coleco_joymode=statesave[i++];
        coleco_joystat=statesave[i++];
        coleco_spincount=statesave[i++];
        coleco_spinstep=statesave[i++];
        coleco_spinstate=statesave[i++];
        coleco_updatetms=statesave[i++];
        tstates=statesave[i++];
        frametstates=statesave[i++];
        tStatesCount=statesave[i++];
        MemoryMap[0]=(unsigned char *) (statesave[i++]); MemoryMap[1]=(unsigned char *) (statesave[i++]); MemoryMap[2]=(unsigned char *) (statesave[i++]);
        MemoryMap[3]=(unsigned char *) (statesave[i++]); MemoryMap[4]=(unsigned char *) (statesave[i++]); MemoryMap[5]=(unsigned char *) (statesave[i++]);
        MemoryMap[6]=(unsigned char *) (statesave[i++]); MemoryMap[7]=(unsigned char *) (statesave[i++]);

        // Get Z80 CPU   (init it again for tables)
        z80_init();
        memcpy(&Z80,statebuf+statesize,sizeof(Z80)); statesize+=sizeof(Z80);

        // Get VDP
        memcpy(&tms,statebuf+statesize,sizeof(tms)); statesize+=sizeof(tms);

        // Get sound
        memcpy(&sn,statebuf+statesize,sizeof(sn)); statesize+=sizeof(sn);
        memcpy(&ay,statebuf+statesize,sizeof(ay)); statesize+=sizeof(ay);

        // Get memory
        memcpy(ROM_Memory,statebuf+statesize,MAX_CART_SIZE*1024);statesize+=MAX_CART_SIZE*1024;
        memcpy(RAM_Memory,statebuf+statesize,MAX_RAM_SIZE*1024);statesize+=MAX_RAM_SIZE*1024;
        memcpy(SRAM_Memory,statebuf+statesize,MAX_EEPROM_SIZE*1024);statesize+=MAX_EEPROM_SIZE*1024;
    }

    // Done, close file and free memory
    fclose(fstatefile);
    free(statebuf);

    return(1);
}



