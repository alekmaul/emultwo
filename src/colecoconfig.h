/* EmulTwo  - A Windows ColecoVision emulator.
 * Copyright (C) 2014-2022 Alekmaul
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
 * colecoconfig.h
 *
 */


#ifndef COLECOCONFIG_H
#define COLECOCONFIG_H

#define CFGBYTE char
#define BYTE unsigned char

#define CLOCK_NTSC              3579545
#define CLOCK_PAL               3546895

#define EMUID                   0x85

#define SYNCTYPEH               1
#define SYNCTYPEV               2

#define BORDERNONE      0
#define BORDERSMALL     1
#define BORDERNORMAL    2
#define BORDERLARGE     3
#define BORDERFULL      4

#define ROMCARTRIDGENONE        0X00
#define ROM_LOAD_FAIL           0X01
#define ROM_LOAD_PASS           0X02
#define ROM_VERIFY_FAIL         0X03

#define MACHINECOLECO           0
#define MACHINEADAM             1

#define NOBACKUP                0
#define EEP24C08                1
#define EEP24C256               2
#define EEPSRAM                 3


#define HIRESDISABLED   0
#define HIRESWRX        1
#define HIRESG007       2
#define HIRESMEMOTECH   3

#define CHRGENSINCLAIR  0
#define CHRGENDK        1
#define CHRGENQS        2
#define CHRGENCHR16     3
#define CHRGENLAMBDA    4

#define COLOURDISABLED  0
#define COLOURSINCLAIR  0
#define COLOURLAMBDA    1
#define COLOURCHROMA    2
#define COLOURPRISM     3
#define COLOURDDC       4
#define COLOURHAVEN     5
#define COLOURACE       6
#define COLOURSPECTRA   7


#define FLOPPYNONE      0
#define FLOPPYPLUS3     1
#define FLOPPYPLUSD     2
#define FLOPPYDISCIPLE  3
#define FLOPPYOPUSD     4
#define FLOPPYBETA      5
#define FLOPPYIF1       6
#define FLOPPYZX1541    7
#define FLOPPYLARKEN81  8

#define DRIVENONE       0
#define DRIVE3INCHSS    1
#define DRIVE3INCHDS    2
#define DRIVE35INCHDS   3


#define TVW                     (256+0) // 16)     // Screen buffer width
#define TVH                     (192+0) // 16)     // Screen buffer height

typedef struct
{
        CFGBYTE emuid, major,minor,testver;
        CFGBYTE machine;
        CFGBYTE NTSC;
        CFGBYTE SGM;
        CFGBYTE inverse;
        CFGBYTE aysound;
        CFGBYTE aytype;
        CFGBYTE singlestep;                      // (1) if debug mode on and doing single step
        CFGBYTE stop;                            // to stop (1) or not (0) emulation
        CFGBYTE vsyncsound;
        CFGBYTE bordersize;
        CFGBYTE romCartridge;

        CFGBYTE typebackup;

        int cardcrc;
        int cardsize;
        
        int frameskip;

        char cwd[256];
        char temppath[256];
        char inipath[256];
        char configpath[256];
        char mydocs[256];
        char machinename[256];
} COLECO;

typedef struct
{
        int AdvancedEffects;
        int DotCrawl;
        int Interlaced;
        int DisableAdvanced;
} TV;

typedef struct
{
        void (*initialise)(void);
        int (*do_scanline)(void);
        void (*writebyte)(int Address, int Data);
        void (*setbyte)(int Address, int Data);
        BYTE (*readbyte)(int Address);
        BYTE (*readoperandbyte)(int Address);
        BYTE (*getbyte)(int Address);
        BYTE (*opcode_fetch)(int Address);
        void (*writeport)(int Address, int Data, int *tstates);
        BYTE (*readport)(int Address, int *tstates);
        int (*contendmem)(int Address, int states, int time);
        int (*contendio)(int Address, int states, int time);
        void (*reset)(void);
        void (*nmi)(void);
        void (*exit)(void);

        int clockspeed;
        int tperscanline;
        int tperframe;
        int intposition;
        int scanlines;
        char CurRom[256];

        void* cset;

} MACHINE;

typedef struct
{
        int x,y, lastx, lasty;
        int buttons;
} MOUSE;

extern COLECO coleco;
extern TV tv;
extern MACHINE machine;
extern MOUSE mouse;

extern int lastMemoryReadAddrLo, lastMemoryWriteAddrLo;
extern int lastMemoryReadValueLo, lastMemoryWriteValueLo;
extern int lastMemoryReadAddrHi, lastMemoryWriteAddrHi;
extern int lastMemoryReadValueHi, lastMemoryWriteValueHi;

extern int directMemoryAccess;

extern void load_config();

#define readbyte_internal(Addr) (machine.opcode_fetch(Addr))
#define readbyte(Addr) (machine.readbyte(Addr))
#define readoperandbyte(Addr) (machine.readoperandbyte(Addr))
#define getbyte(Addr) (machine.getbyte(Addr))
#define writebyte(Addr,Data) (machine.writebyte(Addr,Data))
#define setbyte(Addr,Data) (machine.setbyte(Addr,Data))
#define writeport(Addr,Data,tstates) (machine.writeport(Addr,Data,tstates))
#define readport(Addr, tstates) (machine.readport(Addr, tstates))
#define opcode_fetch(Addr) (machine.opcode_fetch(Addr))

#endif

