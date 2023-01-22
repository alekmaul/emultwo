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

#define ROMCARTRIDGENONE        0X00
#define ROMCARTRIDGESTD         0X01
#define ROMCARTRIDGEMEGA        0X02
#define ROMCARTRIDGEZX81        0X03
#define ROMCARTRIDGEDISK        0X04
#define ROMCARTRIDGEDTAPE       0X05

#define ROM_LOAD_FAIL           0X01
#define ROM_LOAD_PASS           0X02
#define ROM_VERIFY_FAIL         0X03

#define MACHINECOLECO           0
#define MACHINEADAM             1
#define MACHINEPHOENIX          2

#define NOBACKUP                0
#define EEP24C08                1
#define EEP24C256               2
#define EEPSRAM                 3

#define BDW                     16                  // Screen border width
#define BDH                     16                  // Screen border height

#define TVW                     (256+BDW)           // Screen buffer width
#define TVH                     (192+BDH)           // Screen buffer height

typedef struct
{
        CFGBYTE emuid, major,minor,testver;
        CFGBYTE machine;
        CFGBYTE NTSC;
        CFGBYTE SGM;
        CFGBYTE F18A;
        CFGBYTE singlestep;                         // (1) if debug mode on and doing single step
        CFGBYTE startdebug;                         // (1) if start debug mode on 
        CFGBYTE stop;                               // to stop (1) or not (0) emulation
        CFGBYTE vsyncsound;
        CFGBYTE palette;
        CFGBYTE hackbiospal;
        CFGBYTE biosnodelay;

        CFGBYTE romCartridge;
        CFGBYTE typebackup;

        int cardcrc;
        int cardsize;
        
        int frameskip;

        char cwd[256];
        char inipath[256];
        char configpath[256];
        char mydocs[256];
        char machinename[256];

        char CurRom[256];
} EMUL2;

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
        int fps;

        void* cset;

} MACHINE;

extern EMUL2 emul2;
extern MACHINE machine;

extern int lastMemoryReadAddrLo, lastMemoryWriteAddrLo;
extern int lastMemoryReadValueLo, lastMemoryWriteValueLo;
extern int lastMemoryReadAddrHi, lastMemoryWriteAddrHi;
extern int lastMemoryReadValueHi, lastMemoryWriteValueHi;

extern void load_config();
#if 0
#define readbyte_internal(Addr) (machine.opcode_fetch(Addr))
#define readbyte(Addr) (machine.readbyte(Addr))
#define readoperandbyte(Addr) (machine.readoperandbyte(Addr))
#define getbyte(Addr) (machine.getbyte(Addr))
#define writebyte(Addr,Data) (machine.writebyte(Addr,Data))
#define setbyte(Addr,Data) (machine.setbyte(Addr,Data))
#define writeport(Addr,Data,tstates) (machine.writeport(Addr,Data,tstates))
#define readport(Addr, tstates) (machine.readport(Addr, tstates))
#define opcode_fetch(Addr) (machine.opcode_fetch(Addr))

#else
#define cpu_readmem16(address) (machine.readbyte(address))
#define cpu_writemem16(address,value) (machine.writebyte(address,value))
#define cpu_readport16(Port) (machine.readport(Port,0))
#define cpu_writeport16(Port,Value) (machine.writeport(Port,Value,0))
#define getbyte(Addr) (machine.getbyte(Addr))
#define setbyte(Addr,Data) (machine.setbyte(Addr,Data))
#endif

#endif

