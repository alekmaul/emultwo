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
 * f18agpu.cpp
 *
 * Based on js99'er - TI-99/4A emulator written in JavaScript
 *  written by Rasmus Moustgaard <rasmus.moustgaard@gmail.com>
 *
 */
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "f18agpu.h"

#include "tms9928a.h"
#include "f18a.h"

//---------------------------------------------------------------------------
#define F18AGPU_PRELOADSIZE     (128/2)*7+(48/2)
const char F18AGPU_PRELOAD[] = {
    "020F47FE100D4036405A409440B440FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0CA0411C034004C1D0603F000971C0214006069010F7C0203F02C0603F04C0A0"
    "3F06D0E03F011305D010DC40060216FD1003DC70060216FD045B0D0B06A040B40F0BC1C7131604C0D02060040A30C0C004C102020400CC01060216FD04C0D020"
    "415106C00A30A0030CA041AED8204151B000045BD820411A3F00020041D6C8003F0202004006C8003F0402004010C8003F06045B04C7D0203F011313C0204118"
    "06000CA041520204000502053F02020641428DB51603060416FC1009060016F11009C0203F020CA04152804014030CA0419A0547D807B000045B0D0B06A040B4"
    "0F0BC1C71304C0203F0C0CA041AE045B050000000000000000000000020041100201411502020B0003A032023230323032303600020200063631060216FD03C0"
    "0C0020202020202020202020000000000000880041181A03C06041180C000D000A4002010B00A020411617010581A0604114020341420202001003A0320106C1"
    "3201320006C0320036003633060216FD03C00F00C06041180C0002003F000201414202020008CC31060216FD0C000201414CD0A0415006C2D0A0414F02030B00"
    "03A0320332313231323136013630060216FD03C00C000340"
};

#pragma package(smart_init)

tF18AGPU f18agpu;

BYTE wStatusLookup[0x10000];
BYTE bStatusLookup[0x100];

typedef void (*F18AGPUFctn)(void);			// function pointers to opcode

F18AGPUFctn opcodef18agpu[0x10000];            // 64K opcode possibles ...

// ----------------------------------------------------------------------------------------

void f18agpu_initopcode0(unsigned short in)
{
    unsigned short x;

    x=(in&0x0f00)>>8;

    switch(x)
    {
    case 2: f18agpu_initopcode02(in);       break;
    case 3: f18agpu_initopcode03(in);       break;
    case 4: f18agpu_initopcode04(in);       break;
    case 5: f18agpu_initopcode05(in);       break;
    case 6: f18agpu_initopcode06(in);       break;
    case 7: f18agpu_initopcode07(in);       break;
    case 8: opcodef18agpu[in]=&f18agpu_sra;    break;
    case 9: opcodef18agpu[in]=&f18agpu_srl;    break;
    case 10:opcodef18agpu[in]=&f18agpu_sla;    break;
    case 11:opcodef18agpu[in]=&f18agpu_src;    break;
    default: opcodef18agpu[in]=&f18agpu_bad; break;
    }
}

void f18agpu_initopcode02(unsigned short in)
{
    unsigned short x;

    x=(in&0x00e0)>>4;

    switch(x)
    {
    case 0: opcodef18agpu[in]=&f18agpu_li; break;
    case 2: opcodef18agpu[in]=&f18agpu_ai; break;
    case 4: opcodef18agpu[in]=&f18agpu_andi; break;
    case 6: opcodef18agpu[in]=&f18agpu_ori;    break;
    case 8: opcodef18agpu[in]=&f18agpu_ci; break;
    case 10:opcodef18agpu[in]=&f18agpu_stwp; break;
    case 12:opcodef18agpu[in]=&f18agpu_stst; break;
    case 14:opcodef18agpu[in]=&f18agpu_lwpi; break;
    default: opcodef18agpu[in]=&f18agpu_bad; break;
    }
}

void f18agpu_initopcode03(unsigned short in)
{
    Word x;

    x=(in&0x00e0)>>4;

    switch(x)
    {
    case 0: opcodef18agpu[in]=&f18agpu_limi; break;
    case 4: opcodef18agpu[in]=&f18agpu_idle; break;
    case 6: opcodef18agpu[in]=&f18agpu_rset; break;
    case 8: opcodef18agpu[in]=&f18agpu_rtwp; break;
    case 10:opcodef18agpu[in]=&f18agpu_ckon; break;
    case 12:opcodef18agpu[in]=&f18agpu_ckof; break;
    case 14:opcodef18agpu[in]=&f18agpu_lrex; break;
    default: opcodef18agpu[in]=&f18agpu_bad; break;
    }
}

void f18agpu_initopcode04(unsigned short in)
{
    unsigned short x;

    x=(in&0x00c0)>>4;

    switch(x)
    {
    case 0: opcodef18agpu[in]=&f18agpu_blwp; break;
    case 4: opcodef18agpu[in]=&f18agpu_b;  break;
    case 8: opcodef18agpu[in]=&f18agpu_x;  break;
    case 12:opcodef18agpu[in]=&f18agpu_clr;    break;
    default: opcodef18agpu[in]=&f18agpu_bad; break;
    }
}

void f18agpu_initopcode05(unsigned short in)
{
    unsigned short x;

    x=(in&0x00c0)>>4;

    switch(x)
    {
    case 0: opcodef18agpu[in]=&f18agpu_neg;    break;
    case 4: opcodef18agpu[in]=&f18agpu_inv;    break;
    case 8: opcodef18agpu[in]=&f18agpu_inc;    break;
    case 12:opcodef18agpu[in]=&f18agpu_inct; break;
    default: opcodef18agpu[in]=&f18agpu_bad; break;
    }
}

void f18agpu_initopcode06(unsigned short in)
{
    unsigned short x;

    x=(in&0x00c0)>>4;

    switch(x)
    {
    case 0: opcodef18agpu[in]=&f18agpu_dec;    break;
    case 4: opcodef18agpu[in]=&f18agpu_dect; break;
    case 8: opcodef18agpu[in]=&f18agpu_bl; break;
    case 12:opcodef18agpu[in]=&f18agpu_swpb; break;
    default: opcodef18agpu[in]=&f18agpu_bad; break;
    }
}

void f18agpu_initopcode07(unsigned short in)
{
    unsigned short x;

    x=(in&0x00c0)>>4;

    switch(x)
    {
    case 0: opcodef18agpu[in]=&f18agpu_seto; break;
    case 4: opcodef18agpu[in]=&f18agpu_abs;    break;
    default: opcodef18agpu[in]=&f18agpu_bad; break;
    }
}

void f18agpu_initopcode1(unsigned short in)
{
    unsigned short x;

    x=(in&0x0f00)>>8;

    switch(x)
    {
    case 0: opcodef18agpu[in]=&f18agpu_jmp;    break;
    case 1: opcodef18agpu[in]=&f18agpu_jlt;    break;
    case 2: opcodef18agpu[in]=&f18agpu_jle;    break;
    case 3: opcodef18agpu[in]=&f18agpu_jeq;    break;
    case 4: opcodef18agpu[in]=&f18agpu_jhe;    break;
    case 5: opcodef18agpu[in]=&f18agpu_jgt;    break;
    case 6: opcodef18agpu[in]=&f18agpu_jne;    break;
    case 7: opcodef18agpu[in]=&f18agpu_jnc;    break;
    case 8: opcodef18agpu[in]=&f18agpu_joc;    break;
    case 9: opcodef18agpu[in]=&f18agpu_jno;    break;
    case 10:opcodef18agpu[in]=&f18agpu_jl; break;
    case 11:opcodef18agpu[in]=&f18agpu_jh; break;
    case 12:opcodef18agpu[in]=&f18agpu_jop;    break;
    case 13:opcodef18agpu[in]=&f18agpu_sbo;    break;
    case 14:opcodef18agpu[in]=&f18agpu_sbz;    break;
    case 15:opcodef18agpu[in]=&f18agpu_tb; break;
    default:opcodef18agpu[in]=&f18agpu_bad;  break;
    }
}

void f18agpu_initopcode2(unsigned short in)
{
    unsigned short x;

    x=(in&0x0c00)>>8;

    switch(x)
    {
    case 0: opcodef18agpu[in]=&f18agpu_coc; break;
    case 4: opcodef18agpu[in]=&f18agpu_czc; break;
    case 8: opcodef18agpu[in]=&f18agpu_xor; break;
    case 12:opcodef18agpu[in]=&f18agpu_xop; break;
    default: opcodef18agpu[in]=&f18agpu_bad; break;
    }
}

void f18agpu_initopcode3(unsigned short in)
{
    unsigned short x;

    x=(in&0x0c00)>>8;

    switch(x)
    {
    case 0: opcodef18agpu[in]=&f18agpu_ldcr; break;
    case 4: opcodef18agpu[in]=&f18agpu_stcr; break;
    case 8: opcodef18agpu[in]=&f18agpu_mpy;    break;
    case 12:opcodef18agpu[in]=&f18agpu_div;    break;
    default:opcodef18agpu[in]=&f18agpu_bad;  break;
    }
}
// ----------------------------------------------------------------------------------------

// Processor initialization
void f18agpu_init(void)
{
    int i;
    unsigned short in;
    BYTE x,z;

    f18agpu.WP = 0xF000; // Place workspace in an unused part of the memory space
    for (i = 0; i < 32; i++) {
        tms.ram[f18agpu.WP + i] = 0;
    }

    // Lookup tables
    // ---------------
    // Opcode table
    /*
         Formats:

         0   1  2  3  4  5  6  7      8  9  10 11 12 13 14 15
             +------------------------------------------------+
         1   | Opcode | B | Td |  RegNr     | Ts |    RegNr   |
             +--------+---+----+------------+----+------------+
         2   |  Opcode               |      Displacement      |
             +-----------------------+------------------------+
         3   |  Opcode         |  RegNr     | Ts |    RegNr   |
             +-----------------+------------+----+------------+
         4   |  Opcode         |  Count     | Ts |    RegNr   |
             +-----------------+------------+----+------------+
         5   |  Opcode               |  Count    |    RegNr   |
             +-----------------------+-----------+------------+
         6   |  Opcode                      | Ts |    RegNr   |
             +------------------------------+----+------------+
         7   |  Opcode                         |0| 0| 0| 0| 0 |
             +---------------------------------+-+--+--+--+---+
         8   |  Opcode                         |0|    RegNr   |
             +---------------------------------+-+------------+
         9   |  Opcode         |   Reg/Nr   | Ts |    RegNr   |
             +-----------------+------------+----+------------+
    */
    for (i=0; i<65536; i++)
    {
        in=(Word)i;

        x=(in&0xf000)>>12;
        switch(x)
        {
        case 0: f18agpu_initopcode0(in);        break;  // 0000 xxxxxxxxxxxx
        case 1: f18agpu_initopcode1(in);        break;  // 0001 xxxxxxxxxxxx
        case 2: f18agpu_initopcode2(in);        break;  // 0010 xxxxxxxxxxxx
        case 3: f18agpu_initopcode3(in);        break;  // 0011 xxxxxxxxxxxx
        case 4: opcodef18agpu[in]=&f18agpu_szc; break;
        case 5: opcodef18agpu[in]=&f18agpu_szcb; break;
        case 6: opcodef18agpu[in]=&f18agpu_s;  break;
        case 7: opcodef18agpu[in]=&f18agpu_sb; break;
        case 8: opcodef18agpu[in]=&f18agpu_c;  break;
        case 9: opcodef18agpu[in]=&f18agpu_cb; break;
        case 10:opcodef18agpu[in]=&f18agpu_a;  break;
        case 11:opcodef18agpu[in]=&f18agpu_ab; break;
        case 12:opcodef18agpu[in]=&f18agpu_mov;    break;
        case 13:opcodef18agpu[in]=&f18agpu_movb; break;
        case 14:opcodef18agpu[in]=&f18agpu_soc;    break;
        case 15:opcodef18agpu[in]=&f18agpu_socb; break;
        default:opcodef18agpu[in]=&f18agpu_bad;  break;
        }
    }
    // new opcodes
    // CALL 0C80 - 0000 1100 10Ts SSSS
    for (int idx=0x0C80; idx<=0x0CBF; idx++)
    {
        opcodef18agpu[idx] = &f18agpu_call;
    }

    // RET  0C00 - 0000 1100 0000 0000
    opcodef18agpu[0x0c00] = &f18agpu_ret;

    // PUSH 0D00 - 0000 1101 00Ts SSSS
    for (int idx=0x0D00; idx<=0x0D3F; idx++)
    {
        opcodef18agpu[idx]=&f18agpu_push;
    }

    // POP  0F00 - 0000 1111 00Td DDDD
    for (int idx=0x0F00; idx<=0x0f3F; idx++)
    {
        opcodef18agpu[idx]=&f18agpu_pop;
    }

    // SLC  0E00 - 0000 1110 00Ts SSSS
    for (int idx=0x0E00; idx<=0x0E3F; idx++)
    {
        opcodef18agpu[idx]=&f18agpu_slc;
    }

    // Word status lookup table
    for (i = 0; i < 0x10000; i++)
    {
        wStatusLookup[i] = 0;
        if (i > 0) wStatusLookup[i] |= F18AGPU_BIT_LGT; // LGT
        if ((i > 0) && (i < 0x8000)) wStatusLookup[i] |= F18AGPU_BIT_AGT; // AGT
        if (i == 0) wStatusLookup[i] |= F18AGPU_BIT_EQ; // EQ
        if (i == 0) wStatusLookup[i] |= F18AGPU_BIT_C; // C
        if (i == 0x8000) wStatusLookup[i] |= F18AGPU_BIT_OV; // OV
    }

    // Byte status lookup table
    for (i = 0; i < 0x100; i++)
    {
        x = (i & 0xFF);
        bStatusLookup[i] = 0;
        if (i > 0) bStatusLookup[i] |= F18AGPU_BIT_LGT; // LGT
        if ((i > 0) && (i < 0x80)) bStatusLookup[i] |= F18AGPU_BIT_AGT; // AGT
        if (i == 0) bStatusLookup[i] |= F18AGPU_BIT_EQ; // EQ
        if (i == 0) bStatusLookup[i] |= F18AGPU_BIT_C; // C
        if (i == 0x80) bStatusLookup[i] |= F18AGPU_BIT_OV; // OV

        // OP
        for (z = 0; x != 0; x = (x & (x - 1)) & 0xFF)
        {
            z++;						// black magic?
        }
        if ((z & 1) != 0) bStatusLookup[i] |= F18AGPU_BIT_OP;		    // set bit if an odd number
    }

/*
    // Flash RAM
    var that = this;
    this.flash = new F18AFlash(function (restored) {
        that.flashLoaded = restored;
        that.reset();
    });
*/
};
// ----------------------------------------------------------------------------------------

// Interrupt reset the F18A GPU
void f18agpu_intreset(void)
{
    f18agpu.cpuIdle = 1;

    // Internal registers
    f18agpu.PC = 0x4000;
    f18agpu.ST = 0x01C0;
    f18agpu.flagX = 0;

    // Operands
    f18agpu.Ts = 0;
    f18agpu.Td = 0;
    f18agpu.D = 0;
    f18agpu.S = 0;
    f18agpu.B = 0;
    f18agpu.nPostInc[0]=f18agpu.nPostInc[1]=0;

    // Counters
    f18agpu.cycles = 0;
}
// ----------------------------------------------------------------------------------------

// Reset the F18A GPU
void f18agpu_reset(void)
{
    int i;
    unsigned short hexVal;
    char shexval[2];

    f18agpu_intreset();

//this.flash.reset();
    // Load and execute GPU code
    //if (this.flashLoaded)
    //{
        for (i = 0; i < F18AGPU_PRELOADSIZE; i++)
        {
            shexval[0]=F18AGPU_PRELOAD[i*2];
            shexval[1]=F18AGPU_PRELOAD[i*2+1];
            hexVal=(int)strtoul(shexval, NULL, 16);
            tms.ram[0x4000 + i] = hexVal;
        }
    //}
    f18agpu_setpc(0x4000);
};
// ----------------------------------------------------------------------------------------

void f18agpu_setpc(unsigned short value)
{
    f18agpu.PC =value;
    f18agpu.cpuIdle = 0;
};

// ----------------------------------------------------------------------------------------

void f18agpu_writeword(unsigned short addr, unsigned short value)
{
    f18agpu_writebyte(addr, (BYTE) ((value & 0xFF00) >> 8) );
    f18agpu_writebyte(addr + 1, (BYTE) (value & 0x00FF) );
};
// ----------------------------------------------------------------------------------------

void f18agpu_writebyte(unsigned short addr, BYTE value)
{
    BYTE colno;
    // GPU register
    if (addr >= f18agpu.WP) // GPU register
    {
        tms.ram[addr] = value;
    }
    else if (addr < 0x4000) // VRAM
    {
        tms.ram[addr] = value;
//    this.f18a.redrawRequired = true;
    }
    else if (addr < 0x5000) // GRAM
    {
        tms.ram[addr & 0x47FF] = value;
    }
    else if (addr < 0x6000) // PRAM
    {
        colno = ((addr & 0x7F) >> 1)*3;
        if ((addr & 1) == 0)
        {
            cv_palette[colno+0] = (value & 0x0F) * 17; // MSB
        }
        else
        {
            cv_palette[colno+1] = ((value & 0xF0) >> 4) * 17; // LSB
            cv_palette[colno+2] = (value & 0x0F) * 17;
        }
        f18a.PalRecalc = 1;
    }
    else if (addr < 0x7000) // VREG >6000->603F
    {
        WriteF18A(addr & 0x3F, value);
    }
    else if (addr < 0x8000) // Scanline and blanking
    {
            //  Read only
    }
    else if (addr < 0x9000) // DMA
    {
    /*
            if ((addr & 0xF) === 8) {
                // Trigger DMA
                var src = (this.vdpRAM[0x8000] << 8) | this.vdpRAM[0x8001];
                var dst = (this.vdpRAM[0x8002] << 8) | this.vdpRAM[0x8003];
                var width = this.vdpRAM[0x8004];
                // if (width === 0) {
                //     width = 0x100;
                // }
                var height = this.vdpRAM[0x8005];
                // if (height === 0) {
                //     height = 0x100;
                // }
                var stride = this.vdpRAM[0x8006];
                // if (stride === 0) {
                //     stride = 0x100;
                // }
                var dir = (this.vdpRAM[0x8007] & 0x02) === 0 ? 1 : -1;
                var diff = dir * (stride - width);
                var copy = (this.vdpRAM[0x8007] & 0x01) === 0;
                var srcByte = this.vdpRAM[src];
                this.log.debug("DMA triggered src=" + src.toHexWord() + " dst=" + dst.toHexWord() + " width=" + width.toHexByte() + " height=" + height.toHexByte() + " stride=" + stride + " copy=" + copy + " dir=" + dir + " srcByte=" + srcByte);
                var x,y;
                if (copy) {
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            this.vdpRAM[dst] = this.vdpRAM[src];
                            src += dir;
                            dst += dir;
                        }
                        src += diff;
                        dst += diff;
                    }
                }
                else {
                    for (y = 0; y < height; y++) {
                        for (x = 0; x < width; x++) {
                            this.vdpRAM[dst] = srcByte;
                            dst += dir;
                        }
                        dst += diff;
                    }
                }
                this.addCycles(width * height); // ?
                this.f18a.redrawRequired = true;
            }
            else {
                // Setup
                this.vdpRAM[addr & 0x800F] = b;
            }
*/
    }
    else if (addr < 0xA000)        // Unused
    {
    }
    else if (addr < 0xB000)         // Version
    {
        //  Read only
    }
    else if (addr < 0xC000) // Status data
    {
        tms.ram[0xB000] = value & 0x7F; // 7 least significant bits, goes to an enhanced status register for the host CPU to read
    }
};
// ----------------------------------------------------------------------------------------

unsigned short f18agpu_readword(unsigned short addr)
{
    return ( (f18agpu_readbyte(addr) << 8) | f18agpu_readbyte(addr+1) );
};
// ----------------------------------------------------------------------------------------

BYTE f18agpu_readbyte(unsigned short addr)
{
    BYTE color;

    if (addr >= f18agpu.WP) // GPU register
    {
        return tms.ram[addr];
    }
    if (addr < 0x4000) // VRAM
    {
        return tms.ram[addr];
    }
    if (addr < 0x5000) // GRAM
    {
        return tms.ram[addr & 0x47FF];
    }
    if (addr < 0x6000) // PRAM
    {
        color = ((addr & 0x7F) >> 1)*3;
        if ((addr & 1) == 0)
        {
            return (cv_palette[color] / 17); // MSB
        }
        else
        {
            return ( ((cv_palette[color+1] / 17) << 4) | (cv_palette[color+2] / 17)); // LSB
        }
    }
    if (addr < 0x7000) // VREG >6000->603F
    {
        return f18a.VDPR[addr & 0x3F];
    }
    if (addr < 0x8000) // Scanline and blanking
    {
        if ((addr & 1) == 0)
        {
            // Current scanline
            return 0x00; // TODO return this.f18a.getCurrentScanline();
        }
        else
        {
            // Blanking
            return 0x00; // TODO return this.f18a.getBlanking();
        }
    }
    if (addr < 0x9000) // DMA
    {
        // TODO: can you read the DMA?
        return 0;
    }
    else if (addr < 0xA000) // Unused
    {
        return 0;
    }
    else if (addr < 0xB000) // Version
    {
        return F18A_VERSION;
    }
    else if (addr < 0xC000) { // Status data
        return 0; // Write only
    }

    return 0;
};
// ----------------------------------------------------------------------------------------

void f18agpu_execute(int cycles)
{
    int startCycles=f18agpu.cycles;

    while (!f18agpu.cpuIdle && (f18agpu.cycles - startCycles < cycles) )
    {
        // Execute instruction
        f18agpu.in = f18agpu_readword(f18agpu.PC);
        f18agpu.PC = (f18agpu.PC + 2) & 0xFFFF;
        (*opcodef18agpu[f18agpu.in])();
    }
};
// ----------------------------------------------------------------------------------------
void f18agpu_postIncrement(BYTE nWhich)
{
    BYTE i;
    unsigned short t2, nTmpVal;
    int tmpCycles;

    if (f18agpu.nPostInc[nWhich])
    {
        i = f18agpu.nPostInc[nWhich] & 0xf;
        t2 = f18agpu.WP + (i << 1);

        tmpCycles = f18agpu.cycles;
        nTmpVal = f18agpu_readword(t2);	// We need to reread this value, but the memory access can't count for cycles
        f18agpu.cycles = tmpCycles;

        f18agpu_writeword(t2, (nTmpVal + ((f18agpu.nPostInc[nWhich] & F18AGPU_POSTINC2) != 0 ? 2 : 1)) & 0xFFFF);
        f18agpu.nPostInc[nWhich] = 0;
    }
};

// ----------------------------------------------------------------------------------------

// Get addresses for the destination and source arguments
// Note: the format code letters are the official notation from Texas instruments.
// See their TMS9900 documentation for details.
// (Td, Ts, D, S, B, etc)
// Note that some format codes set the destination type (Td) to
// '4' in order to skip unneeded processing of the Destination address
void f18agpu_fixs(void)
{
    unsigned short cycles = 0, temp, t2;

    // source type
    switch (f18agpu.Ts)
    {
    case 0: // register	(R1) Address is the address of the register
        f18agpu.S = f18agpu.WP + (f18agpu.S << 1);
        break;
    case 1: // register indirect (*R1) Address is the contents of the register
        f18agpu.S = f18agpu_readword(f18agpu.WP + (f18agpu.S << 1));
        cycles += 4;
        break;
    case 2:
        if (f18agpu.S != 0)
        {
            // indexed (@>1000(R1))	Address is the contents of the argument plus the contents of the register
            f18agpu.S = f18agpu_readword(f18agpu.PC) + f18agpu_readword(f18agpu.WP + (f18agpu.S << 1));
        }
        else
        {
            // symbolic	 (@>1000) Address is the contents of the argument
            f18agpu.S = f18agpu_readword(f18agpu.PC);
        }
        ADDPC(2);
        cycles += 8;
        break;
    case 3: // do the increment after the opcode is done with the source
        f18agpu.nPostInc[F18AGPU_SRC] = f18agpu.S | (f18agpu.B == 1 ? F18AGPU_POSTINC1 : F18AGPU_POSTINC2);
        t2 = f18agpu.WP + (f18agpu.S << 1);
        temp = f18agpu_readword(t2);
        f18agpu.S = temp;
        // (add 1 if byte, 2 if word) (*R1+) Address is the contents of the register, which
        // register indirect autoincrement is incremented by 1 for byte or 2 for word ops
        cycles += f18agpu.B == 1 ? 6 : 8;
        break;
    }
    F18AGPUADDCYCLE(cycles);
};
// ----------------------------------------------------------------------------------------
void f18agpu_fixd(void)
{
    unsigned short cycles = 0, temp, t2;

    // destination type
    // same as the source types
    switch (f18agpu.Td)
    {
    case 0: // register
        f18agpu.D = f18agpu.WP + (f18agpu.D << 1);
        break;
    case 1: // register indirect
        f18agpu.D = f18agpu_readword(f18agpu.WP + (f18agpu.D << 1));
        cycles += 4;
        break;
    case 2:
        if (f18agpu.D != 0)
        {
            // indexed
            f18agpu.D = f18agpu_readword(f18agpu.PC) + f18agpu_readword(f18agpu.WP + (f18agpu.D << 1));
        }
        else
        {
            // symbolic
            f18agpu.D = f18agpu_readword(f18agpu.PC);
        }
        ADDPC(2);
        cycles += 8;
        break;
    case 3:
        // do the increment after the opcode is done with the dest
        f18agpu.nPostInc[F18AGPU_DST] = f18agpu.D | (f18agpu.B == 1 ? F18AGPU_POSTINC1 : F18AGPU_POSTINC2);
        // (add 1 if byte, 2 if word)
        t2 = f18agpu.WP + (f18agpu.D << 1);
        temp = f18agpu_readword(t2);
        f18agpu.D = temp;
        // register indirect autoincrement
        cycles += f18agpu.B == 1 ? 6 : 8;
        break;
    }
    F18AGPUADDCYCLE(cycles);
};

// ----------------------------------------------------------------------------------------

// all 9900 / F18A opcodes
    // Load Immediate: LI src, imm
void f18agpu_li(void)
{
    F18AGPUFORMATVIII_1;       // read immediate

    f18agpu_writeword(f18agpu.D, f18agpu.S);
    F18AGPURESETLGT_AGT_EQ;
    f18agpu.ST |= wStatusLookup[f18agpu.S] & F18AGPU_MSKLGT_AGT_EQ;

    F18AGPUADDCYCLE(12);
};

    // Add Immediate: AI src, imm
void f18agpu_ai(void)
{
/*
        var x1 = this.readMemoryWord(this.D);

        var x3 = (x1 + this.S) & 0xFFFF;
        this.writeMemoryWord(this.D, x3);

        this.resetEQ_LGT_AGT_C_OV();
        this.ST |= this.wStatusLookup[x3] & this.maskLGT_AGT_EQ;

        if (x3 < x1) this.setC();
        if (((x1 & 0x8000) === (this.S & 0x8000)) && ((x3 & 0x8000) !== (this.S & 0x8000))) this.setOV();

        return 14;
*/
    F18AGPUADDCYCLE(14);
};

    // AND Immediate: ANDI src, imm
void f18agpu_andi(void)
{
/*
        var x1 = this.readMemoryWord(this.D);
        var x2 = x1 & this.S;
        this.writeMemoryWord(this.D, x2);

        this.resetLGT_AGT_EQ();
        this.ST |= this.wStatusLookup[x2] & this.maskLGT_AGT_EQ;

        return 14;
*/
};

    // OR Immediate: ORI src, imm
void f18agpu_ori(void)
{
/*
        var x1 = this.readMemoryWord(this.D);
        var x2 = x1 | this.S;
        this.writeMemoryWord(this.D, x2);

        this.resetLGT_AGT_EQ();
        this.ST |= this.wStatusLookup[x2] & this.maskLGT_AGT_EQ;

        return 14;
*/
};

    // Compare Immediate: CI src, imm
void f18agpu_ci(void)
{
/*
        var x3 = this.readMemoryWord(this.D);

        this.resetLGT_AGT_EQ();
        if (x3 > this.S) this.setLGT();
        if (x3 === this.S) this.setEQ();
        if ((x3 & 0x8000) === (this.S & 0x8000)) {
            if (x3 > this.S) this.setAGT();
        } else {
            if ((this.S & 0x8000) !== 0) this.setAGT();
        }

        return 14;
*/
};

    // STore Workspace Pointer: STWP src
    // Copy the workspace pointer to memory
void f18agpu_stwp(void)
{
/*
        // Not implemented
        return null;
*/
};

    // STore STatus: STST src
    // Copy the status register to memory
void f18agpu_stst(void)
{
/*
        this.writeMemoryWord(this.D, this.ST);
        return 8;
*/
};

    // Load Workspace Pointer Immediate: LWPI imm
    // changes the Workspace Pointer
void f18agpu_lwpi(void)
{
/*
        // Not implemented
        return null;
*/
};

    // Load Interrupt Mask Immediate: LIMI imm
    // Sets the CPU interrupt mask
void f18agpu_limi(void)
{
/*
        return null;
        // Not implemented
*/
};

    // This sets A0-A2 to 010, and pulses CRUCLK until an interrupt is received.
void f18agpu_idle(void)
{
/*
        this.setIdle(true);
        return 10;
*/
    f18agpu.cpuIdle=1;
    F18AGPUADDCYCLE(10);
};

    // This will set A0-A2 to 011 and pulse CRUCLK (so not emulated)
    // However, it does have an effect, it zeros the interrupt mask
void f18agpu_rset(void)
{
/*
        // Not implemented
        return null;
*/
};

    // ReTurn with Workspace Pointer: RTWP
    // The matching return for BLWP, see BLWP for description
    // F18A Modified, does not use R13, only performs R14->PC, R15->status flags
void f18agpu_rtwp(void)
{
/*
        this.ST = this.readMemoryWord(this.WP + 30); // R15
        this.PC = this.readMemoryWord(this.WP + 28); // R14
        return 14;
*/
};

    // This is the SPI_EN instruction of the F18A GPU
void f18agpu_ckon(void)
{
/*
        this.flash.enable();
        return 10;
*/
};

    // This is the SPI_DS instruction of the F18A GPU
void f18agpu_ckof(void)
{
/*
        this.flash.disable();
        return 10;
*/
};

    // This will set A0-A2 to 111 and pulse CRUCLK
void f18agpu_lrex(void)
{
/*
        // Not implemented
        return null;
*/
};

    // Branch and Load Workspace Pointer: BLWP src
    // A context switch. The src address points to a 2 word table.
    // the first word is the new workspace address, the second is
    // the address to branch to. The current Workspace Pointer,
    // Program Counter (return address), and Status register are
    // stored in the new R13, R14 and R15, respectively
    // Return is performed with RTWP
void f18agpu_blwp(void)
{
/*
        // Not implemented
        return null;
*/
};

    // Branch: B src
    // Unconditional absolute branch
void f18agpu_b(void)
{
/*
        this.PC = this.S;
        this.postIncrement(this.SRC);
        return 8;
*/
};

    // eXecute: X src
    // The argument is interpreted as an instruction and executed
void f18agpu_x(void)
{
/*
        if (this.flagX !== 0) {
            this.log.info("Recursive X instruction!");
        }

        var xInstr = this.readMemoryWord(this.S);
        this.postIncrement(this.SRC);	// does this go before or after the eXecuted instruction??
        // skip_interrupt=1;	    // (ends up having no effect because we call the function inline, but technically still correct)

        var cycles = 8 - 4;	        // For X, add this time to the execution time of the instruction found at the source address, minus 4 clock cycles and 1 memory access.
        this.flagX = this.PC;	    // set flag and save true post-X address for the JMPs (AFTER X's operands but BEFORE the instruction's operands, if any)
        cycles += this.execute(xInstr);
        this.flagX = 0;			    // clear flag

        return cycles;
*/
};

    // CLeaR: CLR src
    // sets word to 0
void f18agpu_clr(void)
{
/*
        this.writeMemoryWord(this.S, 0);
        this.postIncrement(this.SRC);
        return 10;
*/
};

    // NEGate: NEG src
void f18agpu_neg(void)
{
/*
        var x1 = this.readMemoryWord(this.S);

        x1 = ((~x1) + 1) & 0xFFFF;
        this.writeMemoryWord(this.S, x1);
        this.postIncrement(this.SRC);

        this.resetEQ_LGT_AGT_C_OV();
        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ_OV_C;

        return 12;
*/
};

    // INVert: INV src
void f18agpu_inv(void)
{
/*
        var x1 = this.readMemoryWord(this.S);
        x1 = (~x1) & 0xFFFF;
        this.writeMemoryWord(this.S, x1);
        this.postIncrement(this.SRC);

        this.resetLGT_AGT_EQ();
        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ;

        return 10;
*/
};

    // INCrement: INC src
void f18agpu_inc(void)
{
/*
        var x1 = this.readMemoryWord(this.S);

        x1 = (x1 + 1) & 0xFFFF;
        this.writeMemoryWord(this.S, x1);
        this.postIncrement(this.SRC);

        this.resetEQ_LGT_AGT_C_OV();
        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ_OV_C;

        return 10;
*/
};

    // INCrement by Two: INCT src
void f18agpu_inct(void)
{
/*
        var x1 = this.readMemoryWord(this.S);

        x1 = (x1 + 2) & 0xFFFF;
        this.writeMemoryWord(this.S, x1);
        this.postIncrement(this.SRC);

        this.resetEQ_LGT_AGT_C_OV();
        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ;

        if (x1 < 2) this.setC();
        if ((x1 === 0x8000) || (x1 === 0x8001)) this.setOV();

        return 10;
*/
};

            // DECrement: DEC src
void f18agpu_dec(void)
{
/*
        var x1 = this.readMemoryWord(this.S);

        x1 = (x1 - 1) & 0xFFFF;
        this.writeMemoryWord(this.S, x1);
        this.postIncrement(this.SRC);

        this.resetEQ_LGT_AGT_C_OV();
        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ;

        if (x1 !== 0xffff) this.setC();
        if (x1 === 0x7fff) this.setOV();

        return 10;
*/
};

    // DECrement by Two: DECT src
void f18agpu_dect(void)
{
/*
        var x1 = this.readMemoryWord(this.S);

        x1 = (x1 - 2) & 0xFFFF;
        this.writeMemoryWord(this.S, x1);
        this.postIncrement(this.SRC);

        this.resetEQ_LGT_AGT_C_OV();
        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ;

        // if (x1 < 0xfffe) this.set_C();
        if (x1 < 0xfffe) this.setC();
        if ((x1 === 0x7fff) || (x1 === 0x7ffe)) this.setOV();

        return 10;
*/
};


void f18agpu_bl(void)
{
/*
        // Branch and Link: BL src
        // Essentially a subroutine jump - return address is stored in R11
        // Note there is no stack, and no official return function.
        // A return is simply B *R11. Some assemblers define RT as this.

        this.writeMemoryWord(this.WP + 22, this.PC);
        this.PC = this.S;
        this.postIncrement(this.SRC);

        return 12;
*/
};

    // SWaP Bytes: SWPB src
    // swap the high and low bytes of a word
void f18agpu_swpb(void)
{
/*
        var x1 = this.readMemoryWord(this.S);

        var x2 = ((x1 & 0xff) << 8) | (x1 >> 8);
        this.writeMemoryWord(this.S, x2);
        this.postIncrement(this.SRC);

        return 10;
*/
};

    // SET to One: SETO src
    // sets word to 0xffff
void f18agpu_seto(void)
{
/*
        this.writeMemoryWord(this.S, 0xffff);
        this.postIncrement(this.SRC);

        return 10;
*/
};

    // ABSolute value: ABS src
void f18agpu_abs(void)
{
/*
        var cycles = 0;
        var x1 = this.readMemoryWord(this.S);

        if ((x1 & 0x8000) !== 0) {
            var x2 = ((~x1) + 1) & 0xFFFF;	// if negative, make positive
            this.writeMemoryWord(this.S, x2);
            cycles += 2;
        }
        this.postIncrement(this.SRC);

        this.resetEQ_LGT_AGT_C_OV();
        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ_OV;

        return cycles + 12;
*/
};

    // Shift Right Arithmetic: SRA src, dst
    // For the shift instructions, a count of '0' means use the
    // value in register 0. If THAT is zero, the count is 16.
    // The arithmetic operations preserve the sign bit
void f18agpu_sra(void)
{
/*
        var cycles = 0;
        if (this.D === 0) {
            this.D = this.readMemoryWord(this.WP) & 0xf;
            if (this.D === 0) this.D = 16;
            cycles += 8;
        }
        var x1 = this.readMemoryWord(this.S);
        var x4 = x1 & 0x8000;
        var x3 = 0;

        for (var x2 = 0; x2 < this.D; x2++) {
            x3 = x1 & 1;   // save carry
            x1 = x1 >> 1;  // shift once
            x1 = x1 | x4;  // extend sign bit
        }
        this.writeMemoryWord(this.S, x1);

        this.resetEQ_LGT_AGT_C();
        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ;

        if (x3 !== 0) this.setC();

        return cycles + 12 + 2 * this.D;
*/
};

    // Shift Right Logical: SRL src, dst
    // The logical shifts do not preserve the sign
void f18agpu_srl(void)
{
/*
        var cycles = 0;
        if (this.D === 0) {
            this.D = this.readMemoryWord(this.WP) & 0xf;
            if (this.D === 0) this.D = 16;
            cycles += 8;
        }
        var x1 = this.readMemoryWord(this.S);
        var x3 = 0;

        for (var x2 = 0; x2 < this.D; x2++) {
            x3 = x1 & 1;
            x1= x1 >> 1;
        }
        this.writeMemoryWord(this.S, x1);

        this.resetEQ_LGT_AGT_C();
        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ;

        if (x3 !== 0) this.setC();

        return cycles + 12 + 2 * this.D;
*/
};

// Shift Left Arithmetic: SLA src, dst
void f18agpu_sla(void)
{
/*
        var cycles = 0;
        if (this.D === 0) {
            this.D = this.readMemoryWord(this.WP) & 0xf;
            if (this.D === 0) this.D = 16;
            cycles += 8;
        }
        var x1 = this.readMemoryWord(this.S);
        var x4 = x1 & 0x8000;
        this.resetEQ_LGT_AGT_C_OV();

        var x3=0;
        for (var x2 = 0; x2 < this.D; x2++) {
            x3 = x1 & 0x8000;
            x1 = x1 << 1;
            if ((x1 & 0x8000) !== x4) this.setOV();
        }
        x1 = x1 & 0xFFFF;
        this.writeMemoryWord(this.S , x1);

        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ;

        if (x3 !== 0) this.setC();

        return cycles + 12 + 2 * this.D;
*/
};

    // Shift Right Circular: SRC src, dst
    // Circular shifts pop bits off one end and onto the other
    // The carry bit is not a part of these shifts, but it set
    // as appropriate
void f18agpu_src(void)
{
/*
        var cycles = 0;
        if (this.D === 0)
        {
            this.D = this.readMemoryWord(this.WP) & 0xf;
            if (this.D === 0) this.D=16;
            cycles += 8;
        }
        var x1 = this.readMemoryWord(this.S);
        for (var x2 = 0; x2 < this.D; x2++) {
            var x4 = x1 & 0x1;
            x1 = x1 >> 1;
            if (x4 !== 0) {
                x1 = x1 | 0x8000;
            }
        }
        this.writeMemoryWord(this.S, x1);

        this.resetEQ_LGT_AGT_C();
        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ;

        if (x4 !== 0) this.setC();

        return cycles + 12 + 2 * this.D;
*/
};

    // JuMP: JMP dsp
    // (unconditional)
void f18agpu_jmp(void)
{
/*
        if (this.flagX !== 0) {
            this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
        }
        if ((this.D & 0x80) !== 0) {
            this.D = 128 - (this.D & 0x7f);
            this.addPC(-(this.D + this.D));
        }
        else {
            this.addPC(this.D + this.D);
        }
        return 10;
*/
};

    // Jump if Less Than: JLT dsp
void f18agpu_jlt(void)
{
/*
        if (this.getAGT() === 0 && this.getEQ() === 0) {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }

            if ((this.D & 0x80) !== 0) {
                this.D = 128 - (this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            }
            else {
                this.addPC(this.D + this.D);
            }
            return 10;
        } else {
            return 8;
        }
*/
};

// Jump if Low or Equal: JLE dsp
void f18agpu_jle(void)
{
/*
        if ((this.getLGT() === 0) || (this.getEQ() !== 0)) {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }

            if ((this.D & 0x80) !== 0) {
                this.D = 128 - (this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            }
            else {
                this.addPC(this.D + this.D);
            }
            return 10;
        } else {
            return 8;
        }
*/
};

// Jump if equal: JEQ dsp
// Conditional relative branch. The displacement is a signed byte representing
// the number of words to branch
void f18agpu_jeq(void)
{
/*
        if (this.getEQ() !== 0) {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }

            if ((this.D & 0x80) !== 0) {
                this.D = 128 - (this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            } else {
                this.addPC(this.D + this.D);
            }
            return 10;
        } else {
            return 8;
        }
*/
};

    // Jump if High or Equal: JHE dsp
void f18agpu_jhe(void)
{
/*
        if ((this.getLGT() !== 0) || (this.getEQ() !== 0)) {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }

            if ((this.D & 0x80) !== 0) {
                this.D = 128 - (this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            } else {
                this.addPC(this.D + this.D);
            }
            return 10;
        } else {
            return 8;
        }
*/
};

    // Jump if Greater Than: JGT dsp
void f18agpu_jgt(void)
{
/*
        if (this.getAGT() !== 0) {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }

            if ((this.D & 0x80) !== 0) {
                this.D = 128-(this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            } else {
                this.addPC(this.D + this.D);
            }
            return 10;
        } else {
            return 8;
        }
*/
};

    // Jump if Not Equal: JNE dsp
void f18agpu_jne(void)
{
/*
        if (this.getEQ() === 0) {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }
            if ((this.D & 0x80) !== 0) {
                this.D = 128 - (this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            } else {
                this.addPC(this.D + this.D);
            }
            return 10;
        }
        else {
            return 8;
        }
*/
};

    // Jump if No Carry: JNC dsp
void f18agpu_jnc(void)
{
/*
        if (this.getC() === 0) {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }

            if ((this.D & 0x80) !== 0) {
                this.D = 128 - (this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            } else {
                this.addPC(this.D + this.D);
            }
            return 10;
        }
        else {
            return 8;
        }
*/
};

    // Jump On Carry: JOC dsp
void f18agpu_joc(void)
{
/*
        if (this.getC() !== 0) {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }

            if ((this.D & 0x80) !== 0) {
                this.D = 128 - (this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            } else {
                this.addPC(this.D + this.D);
            }
            return 10;
        }
        else {
            return 8;
        }
*/
};

    // Jump if No Overflow: JNO dsp
void f18agpu_jno(void)
{
/*
        if (this.getOV() === 0) {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }

            if ((this.D & 0x80) !== 0) {
                this.D = 128 - (this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            } else {
                this.addPC(this.D + this.D);
            }
            return 10;
        }
        else {
            return 8;
        }
*/
};

void f18agpu_jl(void)
{
/*
        if ((this.getLGT() === 0) && (this.getEQ() === 0)) {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }

            if ((this.D & 0x80) !== 0) {
                this.D = 128 - (this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            } else {
                this.addPC(this.D + this.D);
            }
            return 10;
        }
        else {
            return 8;
        }
*/
};

// Jump if High: JH dsp
void f18agpu_jh(void)
{
/*
        if ((this.getLGT() !== 0) && (this.getEQ() === 0))
        {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }

            if ((this.D & 0x80) !== 0) {
                this.D = 128 - (this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            } else {
                this.addPC(this.D + this.D);
            }
            return 10;
        } else {
            return 8;
        }
*/
};

// Jump on Odd Parity: JOP dsp
void f18agpu_jop(void)
{
/*
        if (this.getOP() !== 0) {
            if (this.flagX !== 0) {
                this.PC = this.flagX;	// Update offset - it's relative to the X, not the opcode
            }

            if ((this.D & 0x80) !== 0) {
                this.D = 128 - (this.D & 0x7f);
                this.addPC(-(this.D + this.D));
            } else {
                this.addPC(this.D + this.D);
            }
            return 10;
        }
        else {
            return 8;
        }
*/
};

// Set Bit On: SBO src
// Sets a bit in the CRU
void f18agpu_sbo(void)
{
/*
        return null;
*/
};

// Set Bit Zero: SBZ src
// Zeros a bit in the CRU
void f18agpu_sbz(void)
{
/*
        return null;
*/
};

// Test Bit: TB src
// Tests a CRU bit
void f18agpu_tb(void)
{
/*
        return null;
*/
};

// Compare Ones Corresponding: COC src, dst
// Basically comparing against a mask, if all set bits in the src match
// set bits in the dest (mask), the equal bit is set
void f18agpu_coc(void)
{
/*
        var x1 = this.readMemoryWord(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x2 = this.readMemoryWord(this.D);

        var x3 = x1 & x2;

        if (x3 === x1) this.setEQ(); else this.resetEQ();

        return 14;
*/
};

// Compare Zeros Corresponding: CZC src, dst
// The opposite of COC. Each set bit in the dst (mask) must
// match up with a zero bit in the src to set the equals flag
void f18agpu_czc(void)
{
/*
        var x1 = this.readMemoryWord(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x2 = this.readMemoryWord(this.D);

        var x3 = x1 & x2;

        if (x3 === 0) this.setEQ(); else this.resetEQ();

        return 14;
*/
};

// eXclusive OR: XOR src, dst
void f18agpu_xor(void)
{
/*
        var x1 = this.readMemoryWord(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x2 = this.readMemoryWord(this.D);

        var x3 = (x1 ^ x2) & 0xFFFF;
        this.writeMemoryWord(this.D, x3);

        this.resetLGT_AGT_EQ();
        this.ST |= this.wStatusLookup[x3] & this.maskLGT_AGT_EQ;

        return 14;
*/
};

// eXtended OPeration: XOP src ???
// The CPU maintains a jump table starting at 0x0040, containing BLWP style
// jumps for each operation. In addition, the new R11 gets a copy of the address of
// the source operand.
// Apparently not all consoles supported both XOP 1 and 2 (depends on the ROM?)
// so it is probably rarely, if ever, used on the TI99.
//
// In the F18A GPU this is the PIX instruction
// Format: MAxxRWCE xxOOxxPP
// M - 1 = calculate the effective address for GM2 instead of the new bitmap layer
//     0 = use the remainder of the bits for the new bitmap layer pixels
// A - 1 = retrieve the pixel's effective address instead of setting a pixel
//     0 = read or set a pixel according to the other bits
// R - 1 = read current pixel into PP, only after possibly writing PP
//     0 = do not read current pixel into PP
// W - 1 = do not write PP 0 = write PP to current pixel
// C - 1 = compare OO with PP according to E, and write PP only if true
//     0 = always write
// E - 1 = only write PP if current pixel is equal to OO
//     0 = only write PP if current pixel is not equal to OO
// OO - pixel to compare to existing pixel
// PP - new pixel to write, and previous pixel when reading
void f18agpu_xop(void)
{
/*
        this.D = this.WP + (this.D << 1);
        var x1 = this.readMemoryWord(this.S);
        var x2 = this.readMemoryWord(this.D);
        var pixOffset;
        var addr = 0;
        if ((x2 & 0x8000) !== 0) {
            // calculate BM2 address:
            // 00PYYYYY00000YYY +
            //     0000XXXXX000
            // ------------------
            // 00PY YYYY XXXX XYYY
            //
            // Note: Bitmap GM2 address /includes/ the offset from VR4 (pattern table), so to use
            // it for both pattern and color tables, put the pattern table at >0000
            addr =
                (((this.f18a.registers[4] & 0x04) !== 0) ? 0x2000 : 0) |	// P
                ((x1 & 0x00F8) << 5) |						            // YYYYY
                ((x1 & 0xF800) >> 8) |						            // XXXXX
                (x1 & 0x0007);  							            // YYY
        } else {
            // Calculate bitmap layer address
            // this.log.info("Plot(" + ((x1 & 0xFF00) >> 8) + ", " + (x1 & 0x00FF) + ")");
            pixOffset = ((x1 & 0xFF00) >> 8) + (x1 & 0x00FF) * this.f18a.bitmapWidth;
            addr = this.f18a.bitmapBaseAddr + (pixOffset >> 2);
        }

        // Only parse the other bits if M and A are zero
        if ((x2 & 0xc000) === 0) {
            var pixByte = this.readMemoryByte(addr);	    // Get the byte
            var bitShift = (pixOffset & 0x0003) << 1;
            var mask = 0xC0 >> bitShift;
            var pix = (pixByte & mask) >> (6 - bitShift);
            var write = (x2 & 0x0400) === 0;		            // Whether to write
            // TODO: are C and E dependent on W being set? I am assuming yes.
            if (write && (x2 & 0x0200) !== 0) {		        // C - compare active (only important if we are writing anyway?)
                var comp = (pix === ((x2 & 0x0030) >> 4));	    // Compare the pixels
                if ((x2 & 0x0100) !== 0) {
                    // E is set, comparison must be true
                    if (!comp) {
                        write = false;
                    }
                } else {
                    // E is clear, comparison must be false
                    if (comp) {
                        write = false;
                    }
                }
            }
            if (write) {
                var newPix = (x2 & 0x0003) << (6 - bitShift);	// New pixel
                var invMask = (~mask) & 0xFF;
                this.writeMemoryByte(addr, (pixByte & invMask) | newPix);
            }
            if ((x2 & 0x0800) !== 0) {
                // Read is set, so save the original read pixel color in PP
                x2 = (x2 & 0xFFFC) | pix;
                this.writeMemoryWord(this.D, x2);		    // Write it back
            }
        } else {
            // User only wants the address
            this.writeMemoryWord(this.D, addr);
        }

        // Only the source address can be post-inc
        this.postIncrement(this.SRC);

        return 10;
*/
};

// This is the SPI_OUT instruction of the F18A GPU
void f18agpu_ldcr(void)
{
/*
        this.flash.writeByte(this.readMemoryByte(this.S));
        this.postIncrement(this.SRC);
        return 10;
*/
};

// This is the SPI_IN instruction of the F18A GPU
void f18agpu_stcr(void)
{
/*
        this.writeMemoryByte(this.S, this.flash.readByte());
        this.postIncrement(this.SRC);
        return 10;
*/
};

// MultiPlY: MPY src, dst
// Multiply src by dest and store 32-bit result
// Note: src and dest are unsigned.
void f18agpu_mpy(void)
{
/*
        var x1 = this.readMemoryWord(this.S);
        this.postIncrement(this.SRC);

        this.D = this.WP + (this.D << 1);
        var x3 = this.readMemoryWord(this.D);
        x3 = x3 * x1;
        this.writeMemoryWord(this.D,(x3 >> 16) & 0xFFFF);
        this.writeMemoryWord(this.D + 2,(x3 & 0xFFFF));

        return 52;
*/
};

// DIVide: DIV src, dst
// Dest, a 2 word number, is divided by src. The result is stored as two words at the dst:
// the first is the whole number result, the second is the remainder
void f18agpu_div(void)
{
/*
        var x2 = this.readMemoryWord(this.S);
        this.postIncrement(this.SRC);

        this.D = this.WP + (this.D << 1);
        var x3 = this.readMemoryWord(this.D);

        if (x2 > x3) {		// x2 can not be zero because they're unsigned
            x3 = (x3 << 16) | this.readMemoryWord(this.D + 2);
            var x1 = x3 / x2;
            this.writeMemoryWord(this.D, x1 & 0xFFFF);
            x1 = x3 % x2;
            this.writeMemoryWord(this.D + 2, x1 & 0xFFFF);
            this.resetOV();
            return 92;		// This is not accurate. (Up to 124 "depends on the partial quotient after each clock cycle during execution")
        }
        else {
            this.setOV();	// division wasn't possible - change nothing
            return 16;
        }
*/
};

// Set Zeros Corresponding: SZC src, dst
// Zero all bits in dest that are zeroed in src
void f18agpu_szc(void)
{
/*
        var x1 = this.readMemoryWord(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x2 = this.readMemoryWord(this.D);
        var x3 = (~x1) & x2;
        this.writeMemoryWord(this.D, x3);
        this.postIncrement(this.DST);

        this.resetLGT_AGT_EQ();
        this.ST |= this.wStatusLookup[x3] & this.maskLGT_AGT_EQ;

        return 14;
*/
};

// Set Zeros Corresponding, Byte: SZCB src, dst
void f18agpu_szcb(void)
{
/*
        var x1 = this.readMemoryByte(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x2 = this.readMemoryByte(this.D);
        var x3 = (~x1) & x2;
        this.writeMemoryByte(this.D, x3);
        this.postIncrement(this.DST);

        this.resetLGT_AGT_EQ_OP();
        this.ST |= this.bStatusLookup[x3] & this.maskLGT_AGT_EQ_OP;

        return 14;
*/
};

// Subtract: S src, dst
void f18agpu_s(void)
{
/*
        var x1 = this.readMemoryWord(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x2 = this.readMemoryWord(this.D);
        var x3 = (x2 - x1) & 0xFFFF;
        this.writeMemoryWord(this.D, x3);
        this.postIncrement(this.DST);

        this.resetEQ_LGT_AGT_C_OV();
        this.ST |= this.wStatusLookup[x3] & this.maskLGT_AGT_EQ;

        // any number minus 0 sets carry.. Tursi's theory is that converting 0 to the two's complement
        // is causing the carry flag to be set.
        if ((x3 < x2) || (x1 === 0)) this.setC();
        if (((x1 & 0x8000) !== (x2 & 0x8000)) && ((x3 & 0x8000) !== (x2 & 0x8000))) this.setOV();

        return 14;
*/
};

// Subtract Byte: SB src, dst
void f18agpu_sb(void)
{
/*
        var x1 = this.readMemoryByte(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x2 = this.readMemoryByte(this.D);
        var x3 = (x2 - x1) & 0xFF;
        this.writeMemoryByte(this.D, x3);
        this.postIncrement(this.DST);

        this.resetEQ_LGT_AGT_C_OV_OP();
        this.ST |= this.bStatusLookup[x3] & this.maskLGT_AGT_EQ_OP;

        // any number minus 0 sets carry.. Tursi's theory is that converting 0 to the two's complement
        // is causing the carry flag to be set.
        if ((x3 < x2) || (x1 === 0)) this.setC();
        if (((x1 & 0x80) !== (x2 & 0x80)) && ((x3 & 0x80) !== (x2 & 0x80))) this.setOV();

        return 14;
*/
};

// Compare words: C src, dst
void f18agpu_c(void)
{
/*
        var x3 = this.readMemoryWord(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x4 = this.readMemoryWord(this.D);
        this.postIncrement(this.DST);

        this.resetLGT_AGT_EQ();
        if (x3 > x4) this.setLGT();
        if (x3 === x4) this.setEQ();
        if ((x3 & 0x8000) === (x4 & 0x8000)) {
            if (x3 > x4) this.setAGT();
        }
        else {
            if ((x4 & 0x8000) !== 0) this.setAGT();
        }
        return 14;
*/
};

// CompareBytes: CB src, dst
void f18agpu_cb(void)
{
/*
        var x3 = this.readMemoryByte(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x4 = this.readMemoryByte(this.D);
        this.postIncrement(this.DST);

        this.resetLGT_AGT_EQ_OP();
        if (x3 > x4) this.setLGT();
        if (x3 === x4) this.setEQ();
        if ((x3 & 0x80) === (x4 & 0x80)) {
            if (x3 > x4) this.setAGT();
        } else {
            if ((x4 & 0x80) !== 0) this.setAGT();
        }
        this.ST |= this.bStatusLookup[x3] & this.BIT_OP;

        return 14;
*/
};

// Add words: A src, dst
void f18agpu_a(void)
{
/*
        var x1 = this.readMemoryWord(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x2 = this.readMemoryWord(this.D);
        var x3 = (x2 + x1) & 0xFFFF;
        this.writeMemoryWord(this.D, x3);
        this.postIncrement(this.DST);

        this.resetEQ_LGT_AGT_C_OV();	// We come out with either EQ or LGT, never both
        this.ST |= this.wStatusLookup[x3] & this.maskLGT_AGT_EQ;

        if (x3 < x2) this.setC();	// if it wrapped around, set carry
        if (((x1 & 0x8000) === (x2 & 0x8000)) && ((x3 & 0x8000) !== (x2 & 0x8000))) this.setOV(); // if it overflowed or underflowed (signed math), set overflow

        return 14;
*/
};

// Add bytes: A src, dst
void f18agpu_ab(void)
{
/*
        var x1 = this.readMemoryByte(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x2 = this.readMemoryByte(this.D);
        var x3 = (x2 + x1) & 0xFF;
        this.writeMemoryByte(this.D, x3);
        this.postIncrement(this.DST);

        this.resetEQ_LGT_AGT_C_OV();	// We come out with either EQ or LGT, never both
        this.ST |= this.bStatusLookup[x3] & this.maskLGT_AGT_EQ_OP;

        if (x3 < x2) this.setC();	// if it wrapped around, set carry
        if (((x1 & 0x80) === (x2 & 0x80)) && ((x3 & 0x80) !== (x2 & 0x80))) this.setOV();  // if it overflowed or underflowed (signed math), set overflow

        return 14;
*/
};

// MOVe words: MOV src, dst
void f18agpu_mov(void)
{
    unsigned short x1;

    F18AGPUFORMATI;

    x1 = f18agpu_readword(f18agpu.S);
    f18agpu_postIncrement(F18AGPU_SRC);
    f18agpu_fixd();

    f18agpu_writeword(f18agpu.D, x1);
    f18agpu_postIncrement(F18AGPU_DST);

    F18AGPURESETLGT_AGT_EQ;
    f18agpu.ST |= wStatusLookup[x1] & F18AGPU_MSKLGT_AGT_EQ;
    F18AGPUADDCYCLE(14);
};

// MOVe Bytes: MOVB src, dst
void f18agpu_movb(void)
{
/*
        var x1 = this.readMemoryByte(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        this.writeMemoryByte(this.D, x1);
        this.postIncrement(this.DST);

        this.resetLGT_AGT_EQ_OP();
        this.ST |= this.bStatusLookup[x1] & this.maskLGT_AGT_EQ_OP;

        return 14;
*/
};

// Set Ones Corresponding: SOC src, dst
// Essentially performs an OR - setting all the bits in dst that
// are set in src
void f18agpu_soc(void)
{
/*
        var x1 = this.readMemoryWord(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x2 = this.readMemoryWord(this.D);
        var x3 = x1 | x2;
        this.writeMemoryWord(this.D, x3);
        this.postIncrement(this.DST);

        this.resetLGT_AGT_EQ();
        this.ST |= this.wStatusLookup[x3] & this.maskLGT_AGT_EQ;

        return 14;
*/
};

void f18agpu_socb(void)
{
/*
        var x1 = this.readMemoryByte(this.S);
        this.postIncrement(this.SRC);

        this.fixD();
        var x2 = this.readMemoryByte(this.D);
        var x3 = x1 | x2;
        this.writeMemoryByte(this.D, x3);
        this.postIncrement(this.DST);

        this.resetLGT_AGT_EQ_OP();
        this.ST |= this.bStatusLookup[x3] & this.maskLGT_AGT_EQ_OP;

        return 14;
*/
    F18AGPUADDCYCLE(14);
};

// F18A specific opcodes
void f18agpu_call(void)
{
/*
        var x2 = this.readMemoryWord(this.WP + 30);	// get R15
        this.writeMemoryWord(x2, this.PC);
        this.PC = this.S;
        x2 -= 2;
        this.writeMemoryWord(this.WP + 30, x2);     // update R15
        this.postIncrement(this.SRC);
        return 8;
*/
};

void f18agpu_ret(void)
{
/*
        var x1 = this.readMemoryWord(this.WP + 30); // get R15
        x1 += 2;
        this.PC = this.readMemoryWord(x1);          // get PC
        this.writeMemoryWord(this.WP + 30, x1);     // update R15
        return 8;
*/
    F18AGPUADDCYCLE(8);
};

void f18agpu_push(void)
{
/*
        var x1 = this.readMemoryWord(this.S);
        var x2 = this.readMemoryWord(this.WP + 30); // get R15
        this.writeMemoryWord(x2, x1);               // Push the word on the stack
        x2 -= 2;                                    // the stack pointer post-decrements (per Matthew)
        this.writeMemoryWord(this.WP + 30, x2);		// update R15
        this.postIncrement(this.SRC);
        return 8;
*/
    F18AGPUADDCYCLE(8);
};

void f18agpu_slc(void)
{
/*
        var cycles = 0;
        if (this.D === 0)
        {
            this.D = this.readMemoryWord(this.WP) & 0xf;
            if (this.D === 0) this.D=16;
            cycles += 8;
        }
        var x1 = this.readMemoryWord(this.S);
        for (var x2 = 0; x2 < this.D; x2++) {
            var x4 = x1 & 0x8000;
            x1 = x1 << 1;
            if (x4 !== 0) {
                x1 = x1 | 1;
            }
        }
        this.writeMemoryWord(this.S, x1);

        this.resetEQ_LGT_AGT_C();
        this.ST |= this.wStatusLookup[x1] & this.maskLGT_AGT_EQ;

        if (x4 !== 0) this.setC();

        return cycles + 12 + 2 * this.D;
*/
};

void f18agpu_pop(void)
{
/*
        var x2 = this.readMemoryWord(this.WP + 30);	// get R15
        // POP the word from the stack
        // the stack pointer post-decrements (per Matthew)
        x2 += 2;                                    // so here we pre-increment!
        var x1 = this.readMemoryWord(x2);
        this.writeMemoryWord(this.S, x1);
        this.writeMemoryWord(this.WP + 30, x2);		// update R15
        this.postIncrement(this.SRC);
        return 8;
*/
    F18AGPUADDCYCLE(8);    
};



void f18agpu_bad(void)
{
    // Base cycles: 6
    // 1 memory accesses:
    //  Read instruction (already done)

    F18AGPUADDCYCLE(6);
};

