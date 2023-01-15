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
 * coleco.h
 *
 */

#ifndef coleco_h
#define coleco_h

#include "colecoconfig.h"

#include "fdidisk.h"

#define MAXRAMSIZE      512*1024                 // 512K of Memory (rom begin at 0x38000, so for 256K max of ROM)
#define MAXEEPROMSIZE   32*1024                  // 32K of EEProm memory
#define MAXSTATESIZE    (MAXRAMSIZE+MAXEEPROMSIZE+32*1024)

#define ROM_WRITER      (cvmemory)              // 32kB SmartWriter ROM
#define RAM_MAIN_LO     (cvmemory+0x8000)       // 32kB main Adam RAM
#define RAM_EXP_LO      (cvmemory+0x10000)      // 32kB exp Adam RAM
#define ROM_OS7         (cvmemory+0x18000)      // 8kB OS7 ROM (CV BIOS)
#define ROM_BIOS        ROM_OS7
#define RAM_OS7         (cvmemory+0x1A000)      // 8x1kB main CV RAM
#define RAM_BASE        RAM_OS7
#define RAM_DUMMY       (cvmemory+0x1C000)      // 8kB dummy RAM
#define ROM_EOS         (cvmemory+0x1E000)      // 8kB EOS ROM
#define RAM_MAIN_HI     (cvmemory+0x20000)      // 32kB main Adam RAM
#define ROM_EXPANSION   (cvmemory+0x28000)      // 32kB Expansion ROM
#define RAM_EXP_HI      (cvmemory+0x30000)      // 32kB exp Adam RAM
#define ROM_CARTRIDGE   (cvmemory+0x38000)      // 32kB Cartridge ROM

#define MAX_DISKS     4       /* Maximal number of disks     */
#define MAX_TAPES     4       /* Maximal number of tapes     */

enum                   
{
    CHRMAP=0,CHRGEN,SPRATTR,SPRGEN,CHRCOL,VRAM, SGMRAM, RAM, ROM, EEPROM, SRAM,
};

//---------------------------------------------------------------------------
extern BYTE cv_palette[16*3];                           // Coleco display palette
extern BYTE cv_display[TVW*TVH];                        // Coleco display buffer
extern int cv_pal32[16];                                // Coleco display palette in 32 bits RGB
extern unsigned int *cv_screen[TVH];                    // Coleco display buffer in 32 bits RGB

extern BYTE cvmemory[MAXRAMSIZE];                      // CPU address space
extern BYTE eepromdata[MAXEEPROMSIZE];                 // Max EEProm memory 32Ko

extern int tstates,frametstates;

extern BYTE coleco_megacart;                            // <>0 if mega cart rom detected
extern BYTE coleco_megapage;                           // selected page for megacart
extern BYTE coleco_megasize;                           // mega cart rom size in 16kB pages

extern unsigned int coleco_joystat;                     // Joystick / Paddle management

extern unsigned int coleco_spincount;                  // Spinner counters
extern unsigned int coleco_spinstep;                   // Spinner steps
extern unsigned int coleco_spinstate;                  // Spinner bit states

extern int coleco_intpending;                           // 1 if we need a IRQ38 interrupt (spinner, and so on ...)

extern FDIDisk Disks[MAX_DISKS];                        // Adam disk drives
extern FDIDisk Tapes[MAX_TAPES];                        // Adam tape drives

//---------------------------------------------------------------------------
extern unsigned short coleco_gettmsaddr(BYTE whichaddr, BYTE mode, BYTE y);
extern BYTE coleco_gettmsval(BYTE whichaddr, unsigned short addr, BYTE mode, BYTE y);

extern void coleco_initialise(void);
extern void coleco_setbyte(int Address, int Data);
extern void coleco_writebyte(int Address, int Data);
extern BYTE coleco_ReadByte(int Address);
extern BYTE coleco_readoperandbyte(int Address);
extern BYTE coleco_readbyte(int Address);
extern BYTE coleco_getbyte(int Address);
extern BYTE coleco_opcode_fetch(int Address);
extern void coleco_writeport(int Address, int Data, int *tstates);
extern BYTE ReadInputPort(int Address, int *tstates);
extern BYTE coleco_readport(int Address, int *tstates);
extern int coleco_contend(int Address, int states, int time);
extern int coleco_do_scanline(void);

extern void coleco_setval(BYTE whichaddr, unsigned short addr, BYTE y);

extern void coleco_setpalette(int palette);

extern BYTE coleco_loadcart(char *filename, unsigned char cardtype);
extern void coleco_reset(void);
extern void Printer(BYTE V);

extern BYTE coleco_savestate(char *filename);
extern BYTE coleco_loadstate(char *filename);

#endif