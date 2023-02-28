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
 * f18a.cpp
 *
 * Based on js99'er - TI-99/4A emulator written in JavaScript
 *  written by Rasmus Moustgaard <rasmus.moustgaard@gmail.com>
 *
 */
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "f18a.h"

#include "f18agpu.h"
#include "tms9928a.h"
#include "z80.h"

#include "accdraw_.h"
#include "accsound_.h"

// Screen handlers and masks for tms.VR table address registers
// ----------------------------------------------------------------------------------------
#define F18A_MODE_GRAPHICS                  0
#define F18A_MODE_TEXT                      1
#define F18A_MODE_TEXT_80                   2
#define F18A_MODE_BITMAP                    3
#define F18A_MODE_MULTICOLOR                4

const tScrModeF18A F18ASCR[MAXF18ASCREEN+1] = {
    { _F18A_modegm1 },      // GRAPHIC MODE 1 (M4 M3 M2 M1 0 0 0 0) TEXT 32x24
    { _F18A_modet },        // TEXT MODE (M4 M3 M2 M1 0 0 0 1) TEXT 40x24
    { _F18A_modet80 },      // TEXT MODE 80 Col (M4 M3 M2 M1 1 0 0 1) TEXT 80x24
    { _F18A_modetgm2 },     // GRAPHIC MODE 2 (M4 M3 M2 M1 0 1 0 0)   BLOCK 256x192
    { _F18A_modem }         // MULTICOLOR MODE (M4 M3 M2 M1 0 0 1 0) GFX 64x48x16
};

const unsigned char F18A_palette[4*16*3] = {
    // Palette 0, original 9918A NTSC color approximations
    0,0,0,      //  0 Transparent
    0,0,0,      //  1 Black
    2,0xC,3,    //  2 Medium Green
    5,0xD,6, //  3 Light Green
    5,4,0xF, //  4 Dark Blue
    7,6,0xF, //  5 Light Blue
    0xD,5,4, //  6 Dark Red
    4,0xE,0xF, //  7 Cyan
    0xF,0x5,0x4, //  8 Medium Red
    0xF,7,6, //  9 Light Red
    0xD,0xC,3, // 10 Dark Yellow
    0xE,0xD,6, // 11 Light Yellow
    2,0xB,2, // 12 Dark Green
    0xC,5,0xC, // 13 Magenta
    0xC,0xC,0xC, // 14 Gray
    0xF,0xF,0xF, // 15 White*/

    // Palette 1, ECM1 (0 index is always 000) version of palette 0
    0,0,0, //  0 Black
    2,0xC,3, //  1 Medium Green
    0,0,0, //  2 Black
    5,4,0xF, //  3 Dark Blue
    0,0,0, //  4 Black
    0xD,5,4, //  5 Dark Red
    0,0,0, //  6 Black
    4,0xE,0xF, //  7 Cyan
    0,0,0, //  8 Black
    0xC,0xC,0xC, //  9 Gray
    0,0,0, // 10 Black
    0xD,0xC,3, // 11 Dark Yellow
    0,0,0, // 12 Black
    0xC,5,0xC, // 13 Magenta
    0,0,0, // 14 Black
    0xF,0xF,0xF, // 15 White*/

    // Palette 2, CGA colors
    0,0,0, //  0 >000000 (  0   0   0) black
    0,0,0xA, //  1 >0000AA (  0   0 170) blue
    0,0xA,0, //  2 >00AA00 (  0 170   0) green
    0,0xA,0xA, //  3 >00AAAA (  0 170 170) cyan
    0xA,0,0, //  4 >AA0000 (170   0   0) red
    0xA,0,0xA, //  5 >AA00AA (170   0 170) magenta
    0xA,5,0, //  6 >AA5500 (170  85   0) brown
    0xA,0xA,0xA, //  7 >AAAAAA (170 170 170) light gray
    5,5,5, //  8 >555555 ( 85  85  85) gray
    5,5,0xF, //  9 >5555FF ( 85  85 255) light blue
    5,0xF,5, // 10 >55FF55 ( 85 255  85) light green
    5,0xF,0xF, // 11 >55FFFF ( 85 255 255) light cyan
    0xF,5,5, // 12 >FF5555 (255  85  85) light red
    0xF,5,0xF, // 13 >FF55FF (255  85 255) light magenta
    0xF,0xF,5, // 14 >FFFF55 (255 255  85) yellow
    0xF,0xF,0xF, // 15 >FFFFFF (255 255 255) white

    // Palette 3, ECM1 (0 index is always 000) version of palette 2
    0,0,0, //  0 >000000 (  0   0   0) black
    5,5,5, //  1 >555555 ( 85  85  85) gray
    0,0,0, //  2 >000000 (  0   0   0) black
    0,0,0xA, //  3 >0000AA (  0   0 170) blue
    0,0,0, //  4 >000000 (  0   0   0) black
    0,0xA,0, //  5 >00AA00 (  0 170   0) green
    0,0,0, //  6 >000000 (  0   0   0) black
    0,0xA,0xA, //  7 >00AAAA (  0 170 170) cyan
    0,0,0, //  8 >000000 (  0   0   0) black
    0xA,0,0, //  9 >AA0000 (170   0   0) red
    0,0,0, // 10 >000000 (  0   0   0) black
    0xA,0,0xA, // 11 >AA00AA (170   0 170) magenta
    0,0,0, // 12 >000000 (  0   0   0) black
    0xA,5,0, // 13 >AA5500 (170  85   0) brown
    0,0,0, // 14 >000000 (  0   0   0) black
    0xF,0xF,0xF  // 15 >FFFFFF (255 255 255) white
};

tF18A f18a;

#pragma package(smart_init)
// ----------------------------------------------------------------------------------------

// Reset the F18A
void f18a_reset(void) {
    int i,j;

    // Init palette ONLY at power on, but reset is like that
    j=0;
    for (i = 0; i < 64; i++,j+=3)
    {
        cv_palette[j]=F18A_palette[j]*17;
        cv_palette[j+1]=F18A_palette[j+1]*17;
        cv_palette[j+2]=F18A_palette[j+2]*17;
    }

	for (i=0;i<16;i++) tms.IdxPal[i] = i;

    memset(tms.VR,0x00,sizeof(tms.VR));                     // Init tms.VR Registers
	memset(tms.ram,0x00, 0x10000);                          // Init tms.VR VRAM

    memset(f18a.VDPR,0x00,sizeof(f18a.VDPR));               // Init f18a.VR Registers

    // Init specific var regarding chipset
    f18a.SRSel=0x00;
    f18a.VAddrInc = 1;
    f18a.unlocked = 0;
    f18a.DPM = 0;
    f18a.PalAutoInc = 0;
    f18a.PalRegNo = 0;
    f18a.PalRegVal = 255;
    f18a.gpuAddressLatch = 0;
    f18a.Mode = F18A_MODE_GRAPHICS;
    f18a.PalRecalc = 1;
    f18a.TL1HOfs = 0;
    f18a.TL1VOfs = 0;
    f18a.TL2HOfs = 0;
    f18a.TL2VOfs = 0;
    f18a.Row30 = 0;
    f18a.TilColMode = 0;
    f18a.ColTab2 = tms.ram;
    f18a.ChrTab2 = tms.ram;
    f18a.HPSize1 = 0;
    f18a.VPSize1 = 0;
    f18a.HPSize2 = 0;
    f18a.VPSize2 = 0;
    f18a.TL1PalSel = 0;
    f18a.TL2PalSel = 0;
    f18a.SPPalSel = 0;
    f18a.SPMax = 32;
    f18a.SPMaxScanLine = F18A_MAX_SCANLINE_SPRITES_JUMPER ? 32 : 4;
    f18a.SPGSOfs = 0x800;
    f18a.TPGSOfs = 0x800;

    // Shared TMS9918 registers & variables
    tms.VKey=1;                                             // tms.VR address latch key
    tms.SR=0x00;                                            // tms.VR status register
    tms.ColTab = tms.ram;
    tms.ChrTab = tms.ram;
    tms.ChrGen = tms.ram;
    tms.SprTab = tms.ram;
    tms.SprGen = tms.ram;
    tms.ColTabM = 0x3FFF;
    tms.ChrGenM = 0x3FFF;
    tms.FGColor = 0;
    tms.BGColor = 7;
    tms.CurLine=0;                                          // Current scanline
    tms.DLatch = 0;                                         // Current Data latch
    tms.ScanLines = TMS9918_LINES;                          // Default for NTSC
	tms.UCount = 0;
	tms.VAddr = 0x0000;

    // Init all registers
    f18a_resetregs();

    // reset gpu
    f18agpu_reset();

    // Init screen size
    f18a_setwindowsize();
}
// ----------------------------------------------------------------------------------------

void f18a_resetregs(void)
{
    // 9918 registers
    WriteF18A(0, 0);
    WriteF18A(1, 0x40);
    WriteF18A(2, 0);
    WriteF18A(3, 0x10);
    WriteF18A(4, 0x01);
    WriteF18A(5, 0x0A);
    WriteF18A(6, 0x02);
    WriteF18A(7, 0xF2);

    // f18a registers
    WriteF18A(10, 0);
    WriteF18A(11, 0);
    WriteF18A(15, 0);
    WriteF18A(19, 0);
    WriteF18A(24, 0);
    WriteF18A(25, 0);
    WriteF18A(26, 0);
    WriteF18A(27, 0);
    WriteF18A(28, 0);
    WriteF18A(29, 0);
    WriteF18A(30, 0);
    WriteF18A(31, 0);
    WriteF18A(47, 0);
    WriteF18A(48, 1);
    WriteF18A(49, 0);
    WriteF18A(50, 0);
    WriteF18A(51, 32);
    WriteF18A(54, 0x40);
    WriteF18A(57, 0);
    WriteF18A(58, 6);
};
// ----------------------------------------------------------------------------------------

void f18a_updatemasks(void)
{
    if (f18a.Mode == F18A_MODE_BITMAP) {
        tms.ColTabM = ((f18a.VDPR[3] & 0x7F) << 6) | 0x3F;
        tms.ChrGenM  = ((f18a.VDPR[4] & 0x03) << 11) | (tms.ColTabM & 0x7FF);
    }
    else {
        tms.ColTabM = 0x3FFF;
        tms.ChrGenM = 0x3FFF;
    }
};

// ----------------------------------------------------------------------------------------

void f18a_updatemode(BYTE reg0,BYTE reg1)
{
    BYTE oldmode=f18a.Mode;

    // Check bitmap mode bit, not text or multicolor
    if (((reg0 & 0x2) != 0) && ((reg1 & 0x18) == 0) ) {
        // Bitmap mode (graphic mode 2)
        f18a.Mode = F18A_MODE_BITMAP;
    }
    else
    {
        switch ((reg1 & 0x18) >> 3)     // 00011000
        {
        case 0: // Graphics mode 1
            f18a.Mode = F18A_MODE_GRAPHICS;
            break;
        case 1: // Multicolor mode
            f18a.Mode = F18A_MODE_MULTICOLOR;
            break;
        case 2: // Text mode
            if ((reg0 & 0x04) == 0) {
                f18a.Mode = F18A_MODE_TEXT;
            }
            else {
                f18a.Mode = F18A_MODE_TEXT_80;
            }
            break;
        }
    }
    if (f18a.Mode == F18A_MODE_BITMAP) {
        tms.ColTab =tms.ram+((f18a.VDPR[3] & 0x80) << 6);
        tms.ChrGen =tms.ram+((f18a.VDPR[4] & 0x4) << 11);
        f18a_updatemasks();
    }
    else {
        tms.ColTab =tms.ram+(f18a.VDPR[3] << 6);
        tms.ChrGen =tms.ram+((f18a.VDPR[4] & 0x7) << 11);
    }
    tms.ChrTab =tms.ram+((f18a.VDPR[2] & (f18a.Mode != F18A_MODE_TEXT_80 || f18a.unlocked ? 0xf : 0xc)) << 10);
    tms.SprTab =tms.ram+((f18a.VDPR[5] & 0x7f) << 7);
    tms.SprGen =tms.ram+((f18a.VDPR[6] & 0x7) << 11);
    if (oldmode != f18a.Mode)
    {
        f18a_setwindowsize();
    }
};
// ----------------------------------------------------------------------------------------

// Writing a data to a F18A register
unsigned char WriteF18A(int iReg,unsigned char value)
{
    unsigned char oldrunsr,runsr,oldvalue,gromclock;
	unsigned char bIRQ;

    // Enabling IRQs may cause an IRQ here
	bIRQ  = (iReg==1) && ((f18a.VDPR[1]^value)&value&F18A_REG1_IRQ) && (tms.SR&F18A_STAT_VBLANK) ? 1 : 0;

    // store value
    oldvalue = f18a.VDPR[iReg];
    f18a.VDPR[iReg] = value;
    tms.VR[iReg]=value; // to be sure to have also TMS synchronized

	// Depending on the register, do...
    switch (iReg)
    {
    // SHared TMS9918 & F18A registers
    case 0x00: // Mode
    case 0x01:
        f18a_updatemode(f18a.VDPR[0], f18a.VDPR[1]);
        break;
    case 0x02: // // Name table
        tms.ChrTab =tms.ram+( (f18a.VDPR[2] & (f18a.Mode != F18A_MODE_TEXT_80 || f18a.unlocked ? 0xf : 0xc)) << 10);
        break;
    case 0x03: // Color table
        if (f18a.Mode == F18A_MODE_BITMAP) {
            tms.ColTab =tms.ram+((f18a.VDPR[3] & 0x80) << 6);
        }
        else {
            tms.ColTab =tms.ram+(f18a.VDPR[3] << 6);
        }
        f18a_updatemasks();
        break;
    case 0x04: // Pattern table
        if (f18a.Mode == F18A_MODE_BITMAP) {
            tms.ChrGen =tms.ram+((f18a.VDPR[4] & 0x4) << 11);
        }
        else {
            tms.ChrGen =tms.ram+((f18a.VDPR[4] & 0x7) << 11);
        }
        f18a_updatemasks();
        break;
    case 0x05: // Sprite attribute table
        tms.SprTab = tms.ram+((f18a.VDPR[5] & 0x7f) << 7);
        break;
    case 0x06: // Sprite pattern table
        tms.SprGen = tms.ram+((f18a.VDPR[6] & 0x7) << 11);
        break;
    case 0x07:
	    tms.FGColor=tms.IdxPal[value>>4];
		value &= 0x0F;
		tms.IdxPal[0] = tms.IdxPal[ value ? value : 1];
		tms.BGColor=tms.IdxPal[value];
        break;

    // Specific F18 a registers
    case 0x0A: // Name table 2 base address
        f18a.ChrTab2 = tms.ram+((f18a.VDPR[0x0A] & 0x0f) << 10);
        break;
    case 0x0B: // Color Table 2 Base Address, 64-byte boundaries
        f18a.ColTab2 = tms.ram+(f18a.VDPR[0x0B] << 6);
        break;
    case 0x0F: // Status register select / counter control
        f18a.SRSel = f18a.VDPR[0x0F] & 0x0f;
        oldrunsr = (oldvalue & 0x10) != 0;
        runsr = (f18a.VDPR[0x0F] & 0x10) != 0;
        if (oldrunsr && !runsr) {
            // Stop
            f18a.counterElapsed += (0xDEAF/*this.getTime()*/ - f18a.counterStart);
        }
        else if (!oldrunsr && runsr) {
            // Start
            f18a.counterStart = 0xDEAF/*this.getTime()*/;
        }
        if ((f18a.VDPR[0x0F] & 0x20) != 0)
        {
            // Snapshot
            if (runsr) {
                // Started
                f18a.counterSnap = (0xDEAF/*this.getTime()*/ - f18a.counterStart); // + this.counterElapsed;
            }
            else {
                // Stopped
                f18a.counterSnap = f18a.counterElapsed;
            }
            f18a.VDPR[0x0F] &= 0xdf; // Clear trigger bit
        }
        if ((f18a.VDPR[0x0F] & 0x40) != 0) {
            // Reset
            f18a.counterElapsed = 0;
            f18a.counterStart = 0xDEAF/*this.getTime()*/;
            f18a.counterSnap = 0;
            f18a.VDPR[0x0F] &= 0xbf; // Clear trigger bit
        }
        break;
    case 0x13: // Horz interrupt scan line, 0 to disable
        f18a.interruptScanline = f18a.VDPR[0x13];
        break;
    case 0x18:// Palette select
        f18a.SPPalSel = f18a.VDPR[0x18] & 0x30;
        f18a.TL1PalSel = (f18a.VDPR[0x18] & 0x03) << 4; // Shift into position
        f18a.TL2PalSel = (f18a.VDPR[0x18] & 0x0C) << 2; // Shift into position
        break;
    case 0x19: // Horizontal scroll offset 2
        f18a.TL2HOfs = f18a.VDPR[0x19];
        break;
    case 0x1A: // Vertical scroll offset 2
        f18a.TL2VOfs = f18a.VDPR[0x1A];
        break;
    case 0x1B: // Horizontal scroll offset 1
        f18a.TL1HOfs = f18a.VDPR[0x1B];
        break;
    case 0x1C: // Vertical scroll offset 1
        f18a.TL1VOfs = f18a.VDPR[0x1C];
        break;
    case 0x1D: // Page size
        f18a.HPSize1 = (f18a.VDPR[0x1D] & 0x02) << 9;
        f18a.VPSize1 = (f18a.VDPR[0x1D] & 0x01) << 11;
        f18a.HPSize2 = (f18a.VDPR[0x1D] & 0x20) << 5;
        f18a.VPSize2 = (f18a.VDPR[0x1D] & 0x10) << 7;
        f18a.SPGSOfs = 0x100 << (3 - ((f18a.VDPR[0x1D] & 0xC0) >> 6));
        f18a.TPGSOfs = 0x100 << (3 - ((f18a.VDPR[0x1D] & 0x0C) >> 2));
        break;

    // Setting this to 0 restores the jumper value (4 or 32). Here assumed to be 32.
    // Setting this to 31 means all 32 sprites can be displayed.
    // You cannot choose to have 31 displayable sprites on a scanline.
    case 0x1E: // Max displayable sprites on a scanline
        if (f18a.VDPR[0x1E] == 0) {
            f18a.VDPR[0x1E] = F18A_MAX_SCANLINE_SPRITES_JUMPER ? 31 : 4;
        }
        f18a.SPMaxScanLine = f18a.VDPR[0x1E];
        if (f18a.SPMaxScanLine == 31) {
            f18a.SPMaxScanLine = 32;
        }
        break;
    case 0x1F: // Bitmap control
        //f18a.bitmapEnable = (f18a.VDPR[0x1F] & 0x80) != 0;
        f18a.bitmapPriority = (f18a.VDPR[0x1F] & 0x40) != 0;
        f18a.bitmapTransparent = (f18a.VDPR[0x1F] & 0x20) != 0;
        //f18a.bitmapFat = (f18a.VDPR[0x1F] & 0x10) != 0;
        f18a.bitmapPaletteSelect = (f18a.VDPR[0x1F] & (F18A_BMFat ? 0x0C : 0x0F)) << 2; // Shift into position
        break;
    case 0x20: // Bitmap base address
        f18a.bitmapBaseAddr = f18a.VDPR[0x20] << 6;
        break;
    case 0x21: // Bitmap x
        f18a.bitmapX = f18a.VDPR[0x21];
        break;
    case 0x22: // Bitmap y
        f18a.bitmapY = f18a.VDPR[0x22];
        break;
    case 0x23: // Bitmap width
        f18a.bitmapWidth = f18a.VDPR[0x23];
        if (f18a.bitmapWidth == 0) {
            f18a.bitmapWidth = 256;
        }
        break;
    case 0x24: // Bitmap height
        f18a.bitmapHeight = f18a.VDPR[0x24];
        break;
    case 0x2F: // Palette register
        f18a.DPM=(f18a.VDPR[0x2F] & 0x80)>>7;
        f18a.PalAutoInc=(f18a.VDPR[0x2F] & 0x70)>>6;
        f18a.PalRegNo=(f18a.VDPR[0x2F] & 0x3F);
        f18a.PalRegVal=255;
        break;
    case 0x30: // SIGNED two's-complement increment amount for VRAM address, defaults to 1
        f18a.VAddrInc = f18a.VDPR[0x30] < 128 ? f18a.VDPR[0x30] : f18a.VDPR[0x30] - 256;
        break;
    case 0x31: // Enhanced color mode
        f18a.tileLayer2Enabled = (f18a.VDPR[0x31] & 0x80) != 0;
        oldvalue = f18a.Row30;
        f18a.Row30 = (f18a.VDPR[0x31] & 0x40) != 0;
        if (oldvalue != f18a.Row30) {
            f18a_setwindowsize();
        }
        f18a.TilColMode = (f18a.VDPR[0x31] & 0x30) >> 4;
        //f18a.realSpriteYCoord = (f18a.VDPR[0x31] & 0x08) != 0;
        //f18a.spriteLinkingEnabled = (f18a.VDPR[0x31] & 0x04) != 0;
        //f18a.spriteColorMode = f18a.VDPR[0x31] & 0x03;
        break;
    case 0x32: // Position vs name attributes, TL2 always on top
        // Write 1 to reset all VDP registers
        if ((f18a.VDPR[0x32] & 0x80) != 0) {
            f18a_resetregs();
            f18a.unlocked = 0;
            f18a_updatemode(f18a.VDPR[0], f18a.VDPR[1]);
            return (0);
        }
        f18a.gpuHsyncTrigger = (f18a.VDPR[0x32] & 0x40) != 0;
        f18a.gpuVsyncTrigger = (f18a.VDPR[0x32] & 0x20) != 0;
        //f18a.tileLayer1Enabled = (f18a.VDPR[0x32] & 0x10) == 0; // 0 = normal, 1 = disable GM1, GM2, MCM, T40, T80
        f18a.reportMax = (f18a.VDPR[0x32] & 0x08) != 0; // Report sprite max vs 5th sprite
        tms.CurLine = (f18a.VDPR[0x32] & 0x04) != 0; // Draw scan lines
        f18a.ecmPositionAttributes = (f18a.VDPR[0x32] & 0x02) != 0; // 0 = per name attributes in ECMs, 1 = per position attributes
        f18a.tileMap2AlwaysOnTop = (f18a.VDPR[0x32] & 0x01) == 0; // 0 = TL2 always on top, 1 = TL2 vs sprite priority is considered
        break;
    case 0x33: // Stop Sprite (zero based) to limit the total number of sprites to process. Defaults to 32, i.e. no stop sprite
        f18a.SPMax = f18a.VDPR[0x33] & 0x3F;
        break;
    case 0x36: // GPU address MSB
        f18a.gpuAddressLatch = 1;
        break;
    case 0x37: // GPU address LSB
        if (f18a.gpuAddressLatch) {
            f18a.gpuAddressLatch = 0;
            f18agpu_intreset();
            f18agpu_setpc((f18a.VDPR[54] << 8 | f18a.VDPR[55]));
        }
        break;
    case 0x38:
        f18agpu.cpuIdle=((f18a.VDPR[0x38] & 1) != 0) ? 0 : 1;
        break;
    case 0x39:
        if (!f18a.unlocked) {
            if ((oldvalue & 0x1c) == 0x1c && (f18a.VDPR[0x39] & 0x1c) == 0x1c) {
                f18a.unlocked = 1;
            }
        }
        else {
            f18a.VDPR[0x39] = 0;
            f18a.unlocked = 0;
        }
        f18a_updatemode(f18a.VDPR[0], f18a.VDPR[1]);
        break;
    case 0x3A:
        gromclock = f18a.VDPR[0x3A] & 0x0F;
        if (gromclock < 7) {
            gromclock = 6;
        }
        gromclock = (gromclock << 4) | 0x0F;
        //this.tms9919.setGROMClock(gromClock);
        break;
    }

	// Return IRQ, if generated
	return(bIRQ);
};
// ----------------------------------------------------------------------------------------

void f18a_writedata(unsigned char value)
{
    // tms9918 mode or not ?
    if (!f18a.DPM) {
        tms9918_writedata(value);
    }
    else
    {
        // Write data to F18A palette registers | ----rrrr | ggggbbbb |
        if (f18a.PalRegVal == 255)
        {
            // Read first byte
            f18a.PalRegVal = value & 0x0F;
        }
        else
        {
            // Read second byte
            cv_palette[f18a.PalRegNo*3+0] = f18a.PalRegVal * 17;
            cv_palette[f18a.PalRegNo*3+1] = ((value & 0xf0) >> 4) * 17;
            cv_palette[f18a.PalRegNo*3+2] = (value & 0x0f) * 17;
            if (f18a.PalAutoInc)
            {
                f18a.PalRegNo++;
            }
            // The F18A turns off DPM after each register is written if auto increment is off
            // or after writing to last register if auto increment in on
            if (!f18a.PalAutoInc || f18a.PalRegNo == 64)
            {
                f18a.DPM = 0;
                f18a.PalRegNo = 0;
            }
            f18a.PalRegVal = 255;
            f18a.PalRecalc = 1;
        }
    }
};
// ----------------------------------------------------------------------------------------

unsigned char f18a_readdata(void)
{
    // same as tms9918a
    return tms9918_readdata();
};
// ----------------------------------------------------------------------------------------

unsigned char f18a_writectrl(unsigned char value)
{
    BYTE cmd,msb,reg;

    if (tms.VKey)
    {
	    tms.VKey=0;
        tms.VAddr=((tms.VAddr&0xFF00)|value)&0x3FFF;
	}
	else
    {
		tms.VKey=1;
        cmd = (value & 0xc0) >> 6;
        msb = value & 0x3f;
        switch (cmd)
        {
        case 0: // Set read address
            tms.VAddr = (msb << 8) | (tms.VAddr & 0x00FF);
            tms.DLatch = tms.ram[tms.VAddr];
            tms.VAddr += f18a.VAddrInc;
            tms.VAddr &= 0x3FFF;
            f18a.VDPR[15] = f18a.VDPR[msb];
            break;
        case 1: // Set write address
            tms.VAddr =  (msb << 8) | (tms.VAddr & 0x00FF);
            break;
        case 2: // Write register
        case 3:
            reg = msb;
            if (f18a.unlocked || (reg < 8) || (reg == 57))
            {
                return (WriteF18A(reg, tms.VAddr & 0x00FF));
            }
            else
            {
                if ((f18a.VDPR[0] & 0x04) == 0) // 1.8 firmware: writes to registers > 7 are masked if 80 columns mode is not enabled
                {
                    return (WriteF18A(reg & 0x07, tms.VAddr & 0x00FF));
                }
            }
            break;
        }
	}

	// No interrupts
	return(0);
};
// ----------------------------------------------------------------------------------------

unsigned char f18a_readctrl(void)
{
    unsigned char retval=0xFF;

    // check which Status register is active
    if (f18a.SRSel==0)
    {
        // default TMS register
        retval = tms.SR;
        tms.SR &= TMS9918_STAT_5THNUM|TMS9918_STAT_5THSPR;

        z80_set_irq_line(machine.interrupt, CLEAR_LINE);
    }
    else
    {
        switch (f18a.SRSel)
        {
        case 1: // ID
            return 0xe0;
        case 2: // GPU status
            return ( (f18agpu.cpuIdle ? 0 : 0x80) | (tms.ram[0xb000] & 0x7f) );
        case 3: // Current scanline
            return 0; // todo f18a.getCurrentScanline();
        case 4: // Counter nanos LSB
            return 0; //(floor((f18a.counterSnap * 1000000) / 10) * 10 % 1000) & 0x00ff;
        case 5:
            // Counter nanos MSB
            return 0; //((floor((f18a.counterSnap * 1000000) / 10) * 10 % 1000) & 0x0300) >> 8;
        case 6: // Counter micros LSB
            return ((f18a.counterSnap * 1000) % 1000) & 0x00ff;
        case 7: // Counter micros MSB
            return (((f18a.counterSnap * 1000) % 1000) & 0x0300) >> 8;
        case 8: // Counter millis LSB
            return (f18a.counterSnap % 1000) & 0x00ff;
        case 9: // Counter millis MSB
            return ((f18a.counterSnap % 1000) & 0x0300) >> 8;
        case 10: // Counter seconds LSB
            return (f18a.counterSnap / 1000) & 0x00ff;
        case 11: // Counter seconds MSB
            return ((f18a.counterSnap / 1000) & 0xff00) >> 8;
        case 14: // Major/Minor version, 00011000 = v1.8
            return F18A_VERSION;
        case 15: // VDP read register value (see VR15).  Updated any time a VRAM address is set
            return f18a.VDPR[15];
        }
    }

    //tms.VKey = 0; // According to Matthew  -> bug with nothing on screen
    
    return(retval);
};
// ----------------------------------------------------------------------------------------

void f18a_setwindowsize(void)
{
    TVW = (f18a.Mode == F18A_MODE_TEXT_80 ? TVW_F18A*2 : TVW_F18A);
    TVH = (f18a.Row30 ? TVH_F18A : TVH_TMS);
    f18a.WinHeight=(f18a.Row30 ? TVH_ROW30 : TVH_DEF);
    tms.ScanLines=emul2.NTSC ? TMS9918_LINES : TMS9929_LINES;
    tms.ScanLines+=(f18a.Row30 ? 27 : 0);

    // Reinit soud and graphics
    Sound.SoundPrepSmpTab(tms.ScanLines);
    AccurateInit(true);
};
// ----------------------------------------------------------------------------------------

// Call this routine on every scanline to update the screen buffer.
unsigned char f18a_loop(void) {
	unsigned char bIRQ;

	// No IRQ yet
	bIRQ=0;

    // Recalc palette if needed
    if (f18a.PalRecalc)
    {
        RenderCalcPalette(cv_palette,16*4);
        f18a.PalRecalc=0;
    }

	// Increment scanline
	if(++tms.CurLine>=tms.ScanLines) tms.CurLine=0;

	// If refreshing display area, call scanline handler
    if((tms.CurLine>=TMS9918_START_LINE)&&(tms.CurLine<(TMS9918_START_LINE+f18a.WinHeight)) ) {
        if(tms.UCount>=100) {
	        (F18ASCR[f18a.Mode].Refresh)(tms.CurLine-TMS9918_START_LINE);
		}
        else
            ScanSprites(tms.CurLine-TMS9918_START_LINE,0);

    	// and execute GPU opcodes if needed
        f18agpu_execute(F18AGPU_CYCLES_PER_SCANLINE);
	}

	// Check if VBlank
	if(tms.CurLine==(TMS9918_START_LINE+f18a.WinHeight)) {
		// Check if we are currently drawing screen
		if(tms.UCount>=100) {
			// Refresh all screen
			//coleco_paint();
			// Reset update counter
			tms.UCount-=100;
		}

		// Decrement/reset update counter
		tms.UCount+=TMS9918_DRAWFRAMES;

		// Generate IRQ when enabled and when VBlank flag goes up
		if (F18A_InterruptON && !(tms.SR&F18A_STAT_VBLANK) ) {
            bIRQ = 1;
        }

		// Set VBlank status flag
		tms.SR|=F18A_STAT_VBLANK;

		// Set Sprite Collision status flag (done in each line)
		//if(!(tms.SR&TMS9918_STAT_OVRLAP))
		  //	if(CheckSprites()) tms.SR|=TMS9918_STAT_OVRLAP;
	}

	// Return IRQ (1) or not (0)
	return(bIRQ);
}

// ----------------------------------------------------------------------------------------
// This function is called from each graphic mode to refresh the screen border.
void _F18A_refreshborder(unsigned char uY)
{
    unsigned char *P;
    BYTE ColPix;
    int J,N;

    // Border color
    ColPix=tms.BGColor;

    // Screen buffer
    P=cv_display;
    J=TVW*(uY+(TVH-192)/2);

    // For the first line, refresh top border
    if (uY) P+=J;
    else for (;J;J--) *P++=ColPix;

    // Calculate number of pixels
    N=(TVW-( ((f18a.Mode!=F18A_MODE_TEXT) && (f18a.Mode!=F18A_MODE_TEXT_80)) ? 256:240) )/2;

    // Refresh left border
    for (J=N;J;J--) *P++=ColPix;

    // Refresh right border
    P+=TVW-(N<<1);
    for (J=N;J;J--) *P++=ColPix;

    // For the last line, refresh bottom border
    if (uY==f18a.WinHeight-1)
        for (J=TVW*(TVH-f18a.WinHeight)/2;J;J--) *P++=ColPix;
}

// ----------------------------------------------------------------------------------------
void _F18A_modegm1(unsigned char uY)
{
    int i,x,bmpX, index,dx,dy,x1,y1,x2;
    BYTE *P,ColPix,PalBaseIdx,tilePalBaseIdx;
    unsigned short ChrTabCanonicalBase,ChrTabBaseAddr,ChrTabAddr,GenTabAddr;
    unsigned short rowOfs,lineOfs,lineOfs1;
    unsigned short BMX1, BMX2,BMY1,BMY2,BMYOfs,BMPixOfs;
    byte BMByte ;
    BYTE charNo,bit,bitShift,TilAttrByte,TilPrio,TilCol,TransCol0, ColSet,GenByte;
    BYTE BMBitShift, BMColor;
    BYTE SPColor;
    BYTE SPColBuf[256],SPPalBasIdxBuf[256];
    unsigned short SPMaxAttrAddr,SPParentAttrAddr,SPAttrAddr,SPGenBaseAddr,SPGenAddr;
    BYTE SCRYOut,SCRYNeg,SPOnline,SPLinkAttr,SPX,SPY;
    BYTE SPAttr,SPSize,SPMag,SPWidth,SPHeight,SPDimX,SPDimY;
    BYTE patternNo,SPFlipX,SPFlipY, baseColor,SPPalBaseIdx,pixelOn,sprColor;
    BYTE SPGenByte0,SPGenByte1,SPGenByte2,SPBit,SPBitShift1,SPBitShift2;

    // GO to current line
    P  = cv_display + TVW*(uY+(TVH-f18a.WinHeight)/2) + TVW/2-128;

    if (F18A_DisplayON) {
        // Prepare sprite colors
        memset(SPColBuf,0x00,sizeof(SPColBuf));
        memset(SPPalBasIdxBuf,0x00,sizeof(SPPalBasIdxBuf));
        SPOnline = 0;
        SCRYOut = f18a.Row30 ? 0xF0 : 0xC0;
        SCRYNeg = f18a.Row30 ? 0xF0 : 0xD0;
        SPMaxAttrAddr = tms.SprTab -tms.ram + (f18a.SPMax << 2);
        for (SPAttrAddr = tms.SprTab-tms.ram, index = 0; (f18a.Row30 || tms.ram[SPAttrAddr] != 0xd0) && (SPAttrAddr < SPMaxAttrAddr) && (SPOnline <= f18a.SPMaxScanLine); SPAttrAddr += 4, index++) {
            SPParentAttrAddr = 0;
            if (F18A_SPLinkEnabled) {
                SPLinkAttr = tms.ram[(tms.SprTab -tms.ram) + 0x80 + ((SPAttrAddr - (tms.SprTab - tms.ram) ) >> 2)];
                if ((SPLinkAttr & 0x20) != 0)  {
                    SPParentAttrAddr = (tms.SprTab-tms.ram) + ((SPLinkAttr & 0x1F) << 2);
                }
            }
            SPY = tms.ram[SPAttrAddr];
            if (SPParentAttrAddr != 0) {
                SPY = (SPY + tms.ram[SPParentAttrAddr]) & 0xFF;
            }
            if (!F18A_SPRealYCoord) {
                SPY++;
            }
            if ( (SPY < SCRYOut) || (SPY > SCRYNeg)) {
                if (SPY > SCRYNeg)  {
                    SPY -= 256;
                }
                SPAttr = tms.ram[SPAttrAddr + 3];
                SPSize = !f18a.unlocked || (SPAttr & 0x10) == 0 ? F18A_SpriteSize : 1;
                SPMag = F18A_SpriteMag;
                SPHeight = 8 << SPSize; // 8 or 16
                SPDimY = SPHeight << SPMag; // 8, 16 or 32
                if ((uY >= SPY) && (uY < SPY + SPDimY) ) {
                    if (SPOnline < f18a.SPMaxScanLine) {
                        SPWidth = SPHeight;
                        SPDimX = SPDimY;
                        SPX = tms.ram[SPAttrAddr + 1];
                        if (SPParentAttrAddr == 0)  {
                            if ((SPAttr & 0x80) != 0)  {
                                SPX -= 32; // Early clock
                            }
                        }
                        else {
                            // Linked
                            SPX = (SPX + tms.ram[SPParentAttrAddr + 1]) & 0xFF;
                            if ((tms.ram[SPParentAttrAddr + 3] & 0x80) != 0) {
                                SPX -= 32; // Early clock of parent
                            }
                        }
                        patternNo = (tms.ram[SPAttrAddr + 2] & (SPSize != 0 ? 0xFC : 0xFF));
                        SPFlipY = f18a.unlocked && (SPAttr & 0x20) != 0;
                        SPFlipX = f18a.unlocked && (SPAttr & 0x40) != 0;
                        baseColor = SPAttr & 0x0F;
                        SPPalBaseIdx = 0;
                        switch (F18A_SPColorMode)
                        {
                        case F18A_COLOR_MODE_NORMAL:
                            SPPalBaseIdx = f18a.SPPalSel;
                            break;
                        case F18A_COLOR_MODE_ECM_1:
                            SPPalBaseIdx = (f18a.SPPalSel & 0x20) | (baseColor << 1);
                            break;
                        case F18A_COLOR_MODE_ECM_2:
                            SPPalBaseIdx = (baseColor << 2);
                            break;
                        case F18A_COLOR_MODE_ECM_3:
                            SPPalBaseIdx = ((baseColor & 0x0e) << 2);
                            break;
                        }
                        SPGenBaseAddr = (tms.SprGen-tms.ram) + (patternNo << 3);
                        dy = (uY - SPY) >> SPMag;
                        if (SPFlipY) {
                            dy = SPHeight - dy - 1;
                        }
                        for (dx = 0; dx < SPWidth; dx += 8) {
                            SPGenAddr = SPGenBaseAddr + dy + (dx << 1);
                            SPGenByte0 = tms.ram[SPGenAddr];
                            SPGenByte1 = tms.ram[SPGenAddr + f18a.SPGSOfs];
                            SPGenByte2 = tms.ram[SPGenAddr + (f18a.SPGSOfs << 1)];
                            SPBit = 0x80;
                            SPBitShift2 = 7;
                            for (SPBitShift1 = 0; SPBitShift1 < 8; SPBitShift1++) {
                                pixelOn = 0;
                                switch (F18A_SPColorMode)
                                {
                                case F18A_COLOR_MODE_NORMAL:
                                    pixelOn = (SPGenByte0 & SPBit) != 0;
                                    sprColor = pixelOn ? baseColor : 0;
                                    break;
                                case F18A_COLOR_MODE_ECM_1:
                                    sprColor = (SPGenByte0 & SPBit) >> SPBitShift2;
                                    break;
                                case F18A_COLOR_MODE_ECM_2:
                                    sprColor =((SPGenByte0 & SPBit) >> SPBitShift2) |
                                                        (((SPGenByte1 & SPBit) >> SPBitShift2) << 1);
                                    break;
                                case F18A_COLOR_MODE_ECM_3:
                                    sprColor =((SPGenByte0 & SPBit) >> SPBitShift2) |
                                                        (((SPGenByte1 & SPBit) >> SPBitShift2) << 1) |
                                                        (((SPGenByte2 & SPBit) >> SPBitShift2) << 2);
                                    break;
                                }
                                if ( (sprColor > 0) || pixelOn) {
                                    x2 = SPX + ((SPFlipX ? SPDimX - (dx + SPBitShift1) - 1 : dx + SPBitShift1) << SPMag);
                                    if ((x2 >= 0) && (x2 < 256)) {
                                        if (SPColBuf[x2] == 0) {
                                            SPColBuf[x2] = sprColor + 1; // Add one here so 0 means uninitialized. Subtract one before drawing.
                                            SPPalBasIdxBuf[x2] = SPPalBaseIdx;
                                        }
                                        else {
                                            tms.SR|=TMS9918_STAT_OVRLAP;
                                        }
                                    }
                                    if (SPMag) {
                                        x2++;
                                        if ( (x2 >= 0) && (x2 < 256) ) {
                                            if (SPColBuf[x2] == 0) {
                                                SPColBuf[x2] = sprColor + 1; // Add one here so 0 means uninitialized. Subtract one before drawing.
                                                SPPalBasIdxBuf[x2] = SPPalBaseIdx;
                                            }
                                            else {
                                                tms.SR|=TMS9918_STAT_OVRLAP;
                                            }
                                        }
                                    } // if (SPMag) {
                                }
                                SPBit >>= 1;
                                SPBitShift2--;
                            } // for (SPBitShift1 = 0; SPBitShift1 < 8; SPBitShift1++) {
                        }
                    }
                }
                SPOnline++;
/*TODO                if (SPOnline == 5 && !this.fifthSprite) {
                    this.fifthSprite = true;
                    this.fifthSpriteIndex = index;
                }*/
            }
        }

        // Prepare values for Tile layer 1
        ChrTabCanonicalBase = f18a.VPSize1 ? (tms.ChrTab-tms.ram) & 0x3000 : (f18a.HPSize1 ? (tms.ChrTab-tms.ram) & 0x3800 : (tms.ChrTab-tms.ram));
        ChrTabBaseAddr = (tms.ChrTab-tms.ram);
        y1 = uY + f18a.TL1VOfs;
        if (y1 >= f18a.WinHeight) {
            y1 -= f18a.WinHeight;
            ChrTabBaseAddr ^= f18a.VPSize1;
        }
        rowOfs=(y1 >> 3) << 5;
        lineOfs = y1 & 7;

        // Prepare values for Bitmap layer
        if (F18A_BMLEnabled) {
            BMX2 = f18a.bitmapX + f18a.bitmapWidth;
            BMY1 = uY - f18a.bitmapY;
            BMY2 = f18a.bitmapY + f18a.bitmapHeight;
            BMYOfs = BMY1 * f18a.bitmapWidth;
        }

        // Draw line
        for (x = 0; x < 256; x++) {
            // Init default color / palette
            ColPix = tms.BGColor;
            PalBaseIdx=0;
            // Tile layer 1
            if (F18A_TL1Enabled) {
                // Graphic mode 1 specific code
                ChrTabAddr = ChrTabBaseAddr;
                x1 = x - (f18a.TL1HOfs << 0);
                if (x1 >= 256) {
                    x1 -= 256;
                    ChrTabAddr ^= f18a.HPSize1;
                }
                ChrTabAddr += (x1 >> 3) + rowOfs;
                charNo = tms.ram[ChrTabAddr];
                bitShift = x1 & 7;
                lineOfs1 = lineOfs;
                if (f18a.TilColMode != F18A_COLOR_MODE_NORMAL) {
                    TilAttrByte = tms.ram[tms.ColTab-tms.ram + (f18a.ecmPositionAttributes ? ChrTabAddr - ChrTabCanonicalBase : charNo)];
                    TilPrio = (TilAttrByte & 0x80) != 0;
                    if ((TilAttrByte & 0x40) != 0) {
                        // Flip X
                        bitShift = 7 - bitShift;
                    }
                    if ((TilAttrByte & 0x20) != 0) {
                        // Flip y
                        lineOfs1 = 7 - lineOfs1;
                    }
                    TransCol0 = (TilAttrByte & 0x10) != 0;
                }
                bit = 0x80 >> bitShift;
                GenTabAddr = tms.ChrGen -tms.ram + (charNo << 3) + lineOfs1;
                GenByte = tms.ram[GenTabAddr];
                switch (f18a.TilColMode)
                {
                case F18A_COLOR_MODE_NORMAL:
                    ColSet = tms.ram[tms.ColTab-tms.ram + (charNo >> 3)];
                    TilCol = (GenByte & bit) != 0 ? (ColSet & 0xF0) >> 4 : ColSet & 0x0F;
                    tilePalBaseIdx = f18a.TL1PalSel;
                    TransCol0 = true;
                    TilPrio = false;
                    break;
                case F18A_COLOR_MODE_ECM_1:
                    TilCol = ((GenByte & bit) >> (7 - bitShift));
                    tilePalBaseIdx = (f18a.TL1PalSel & 0x20) | ((TilAttrByte & 0x0f) << 1);
                    break;
                case F18A_COLOR_MODE_ECM_2:
                    TilCol = ((GenByte & bit) >> (7 - bitShift)) |
                            (((tms.ram[GenTabAddr + f18a.TPGSOfs] & bit) >> (7 - bitShift)) << 1);
                    tilePalBaseIdx = ((TilAttrByte & 0x0f) << 2);
                    break;
                case F18A_COLOR_MODE_ECM_3:
                    TilCol =((GenByte & bit) >> (7 - bitShift)) |
                        (((tms.ram[GenTabAddr + f18a.TPGSOfs] & bit) >> (7 - bitShift)) << 1) |
                        (((tms.ram[GenTabAddr + (f18a.TPGSOfs << 1)] & bit) >> (7 - bitShift)) << 2);
                    tilePalBaseIdx = ((TilAttrByte & 0x0e) << 2);
                    break;
                }
                if (TilCol > 0 || !TransCol0) {
                    ColPix = TilCol;
                    PalBaseIdx = tilePalBaseIdx;
                }
                // End of Graphic mode 1 specific code
            } // if (F18A_TL1Enabled) {

            // Bitmap layer
            if (F18A_BMLEnabled) {
#if 1
                bmpX = x;
                if ( (bmpX >= f18a.bitmapX) && (bmpX < BMX2) && (uY >= f18a.bitmapY) && (uY < BMY2)) {
                    BMX1 = x - f18a.bitmapX;
                    BMPixOfs = BMX1 + BMYOfs;
                    BMByte = tms.ram[f18a.bitmapBaseAddr + (BMPixOfs >> 2)];
                    if (F18A_BMFat) {
                        // 16 color bitmap with fat pixels
                        BMBitShift = (2 - (BMPixOfs & 2)) << 1;
                        BMColor = (BMByte >> BMBitShift) & 0x0F;
                    }
                    else {
                        // 4 color bitmap
                        BMBitShift = (3 - (BMPixOfs & 3)) << 1;
                        BMColor = (BMByte >> BMBitShift) & 0x03;
                    }
                    if ((BMColor > 0 || !f18a.bitmapTransparent) && (ColPix == tms.BGColor || f18a.bitmapPriority)) {
                        ColPix = BMColor;
                        PalBaseIdx = f18a.bitmapPaletteSelect;
                    }
                }
#endif
            }

            // Sprite layer
            SPColor = 0;
            if ((!TilPrio || (TransCol0)) /*&& (ColPix == tms.BGColor)*/ && SPColBuf[x]) {
                SPColor = SPColBuf[x] - 1;
                if (SPColor > 0) {
                    ColPix = SPColor;
                    PalBaseIdx = SPPalBasIdxBuf[x];
                }
                else {
                    SPColor = 0;
                }
            }

            // Draw pixel
            *P++=ColPix+PalBaseIdx;
        }
    }
    else {
        // Empty scanline
        for (i = 0; i < 256; i++) {
            *P++=tms.BGColor;
        }
    }

    // Refresh border
    _F18A_refreshborder(uY);
}


void _F18A_modet(unsigned char uY)
{
    unsigned char *P;
    int i;

    P  = cv_display + TVW*(uY+(TVH-192)/2) + TVW/2-128;

    if (F18A_DisplayON) {
    }
    else {
        // Empty scanline
        for (i = 0; i < 256; i++) {
            *P++=tms.BGColor;
        }
    }
/*
                            case F18A.MODE_TEXT:
                            case F18A.MODE_TEXT_80:
                                if (x >= borderWidth && x < this.drawWidth - borderWidth) {
                                    ChrTabAddr += Math.floor(x1 / 6) + rowOffset;
                                    charNo = this.ram[ChrTabAddr];
                                    bitShift = x1 % 6;
                                    lineOffset1 = lineOffset;
                                    if (this.TilColMode !== F18A.COLOR_MODE_NORMAL) {
                                        TilAttrByte = this.ram[tms.ColTab + (this.ecmPositionAttributes ? ChrTabAddr - ChrTabCanonicalBase : charNo)];
                                        TilPrio = (TilAttrByte & 0x80) !== 0;
                                        if ((TilAttrByte & 0x40) !== 0) {
                                            // Flip X
                                            bitShift = 5 - bitShift;
                                        }
                                        if ((TilAttrByte & 0x20) !== 0) {
                                            // Flip y
                                            lineOffset1 = 7 - lineOffset1;
                                        }
                                        TransCol0 = (TilAttrByte & 0x10) !== 0;
                                    }
                                    bit = 0x80 >> bitShift;
                                    GenTabAddr = tms.ChrGen + (charNo << 3) + lineOffset1;
                                    GenByte = this.ram[GenTabAddr];
                                    switch (this.TilColMode) {
                                        case F18A.COLOR_MODE_NORMAL:
                                            if (this.unlocked && this.ecmPositionAttributes) {
                                                TilAttrByte = this.ram[tms.ColTab + ChrTabAddr - ChrTabCanonicalBase];
                                                TilCol = (GenByte & bit) !== 0 ? TilAttrByte >> 4 : TilAttrByte & 0xF;
                                            }
                                            else {
                                                TilCol = (GenByte & bit) !== 0 ? tms.FGColor : tms.BGColor;
                                            }
                                            tilePalBaseIdx = this.TL1PalSel;
                                            TransCol0 = true;
                                            TilPrio = false;
                                            break;
                                        case F18A.COLOR_MODE_ECM_1:
                                            TilCol = ((GenByte & bit) >> (7 - bitShift));
                                            tilePalBaseIdx = (this.TL1PalSel & 0x20) | ((TilAttrByte & 0x0f) << 1);
                                            break;
                                        case F18A.COLOR_MODE_ECM_2:
                                            TilCol =
                                                ((GenByte & bit) >> (7 - bitShift)) |
                                                (((this.ram[GenTabAddr + this.TPGSOfs] & bit) >> (7 - bitShift)) << 1);
                                            tilePalBaseIdx = ((TilAttrByte & 0x0f) << 2);
                                            break;
                                        case F18A.COLOR_MODE_ECM_3:
                                            TilCol =
                                                ((GenByte & bit) >> (7 - bitShift)) |
                                                (((this.ram[GenTabAddr + this.TPGSOfs] & bit) >> (7 - bitShift)) << 1) |
                                                (((this.ram[GenTabAddr + (this.TPGSOfs << 1)] & bit) >> (7 - bitShift)) << 2);
                                            tilePalBaseIdx = ((TilAttrByte & 0x0e) << 2);
                                            break;
                                    }
                                    if (TilCol > 0 || !TransCol0) {
                                        color = TilCol;
                                        PalBaseIdx = tilePalBaseIdx;
                                    }
                                }
                                else {
                                    color = tms.BGColor;
                                }
                                */



}

void _F18A_modet80(unsigned char uY)
{
    unsigned char *P;
    int i;

    P  = cv_display + TVW*(uY+(TVH-192)/2) + TVW/2-128;

    if (F18A_DisplayON) {
    }
    else {
        // Empty scanline
        for (i = 0; i < 256; i++) {
            *P++=tms.BGColor;
        }
    }
}

void _F18A_modetgm2(unsigned char uY)
{
    int i,x,bmpX, index,dx,dy,x1,y1,x2;
    BYTE *P,ColPix,PalBaseIdx;
    unsigned short ChrTabBaseAddr,ChrTabAddr;
    unsigned short rowOfs,lineOfs,charSetOfs;
    unsigned short BMX1, BMX2,BMY1,BMY2,BMYOfs,BMPixOfs;
    byte BMByte ;
    BYTE charNo,bit,bitShift,TilPrio,TilCol,TransCol0,GenByte,ColByte;
    BYTE BMBitShift, BMColor;
    BYTE SPColor;
    BYTE SPColBuf[256],SPPalBasIdxBuf[256];
    unsigned short SPMaxAttrAddr,SPParentAttrAddr,SPAttrAddr,SPGenBaseAddr,SPGenAddr;
    BYTE SCRYOut,SCRYNeg,SPOnline,SPLinkAttr,SPX,SPY;
    BYTE SPAttr,SPSize,SPMag,SPWidth,SPHeight,SPDimX,SPDimY;
    BYTE patternNo,SPFlipX,SPFlipY, baseColor,SPPalBaseIdx,pixelOn,sprColor;
    BYTE SPGenByte0,SPGenByte1,SPGenByte2,SPBit,SPBitShift1,SPBitShift2;

    // GO to current line
    P  = cv_display + TVW*(uY+(TVH-f18a.WinHeight)/2) + TVW/2-128;

    if (F18A_DisplayON) {
        // Prepare sprite colors
        memset(SPColBuf,0x00,sizeof(SPColBuf));
        memset(SPPalBasIdxBuf,0x00,sizeof(SPPalBasIdxBuf));
        SPOnline = 0;
        SCRYOut = f18a.Row30 ? 0xF0 : 0xC0;
        SCRYNeg = f18a.Row30 ? 0xF0 : 0xD0;
        SPMaxAttrAddr = tms.SprTab -tms.ram + (f18a.SPMax << 2);
        for (SPAttrAddr = tms.SprTab-tms.ram, index = 0; (f18a.Row30 || tms.ram[SPAttrAddr] != 0xd0) && (SPAttrAddr < SPMaxAttrAddr) && (SPOnline <= f18a.SPMaxScanLine); SPAttrAddr += 4, index++) {
            SPParentAttrAddr = 0;
            if (F18A_SPLinkEnabled) {
                SPLinkAttr = tms.ram[(tms.SprTab -tms.ram) + 0x80 + ((SPAttrAddr - (tms.SprTab - tms.ram) ) >> 2)];
                if ((SPLinkAttr & 0x20) != 0)  {
                    SPParentAttrAddr = (tms.SprTab-tms.ram) + ((SPLinkAttr & 0x1F) << 2);
                }
            }
            SPY = tms.ram[SPAttrAddr];
            if (SPParentAttrAddr != 0) {
                SPY = (SPY + tms.ram[SPParentAttrAddr]) & 0xFF;
            }
            if (!F18A_SPRealYCoord) {
                SPY++;
            }
            if ( (SPY < SCRYOut) || (SPY > SCRYNeg)) {
                if (SPY > SCRYNeg)  {
                    SPY -= 256;
                }
                SPAttr = tms.ram[SPAttrAddr + 3];
                SPSize = !f18a.unlocked || (SPAttr & 0x10) == 0 ? F18A_SpriteSize : 1;
                SPMag = F18A_SpriteMag;
                SPHeight = 8 << SPSize; // 8 or 16
                SPDimY = SPHeight << SPMag; // 8, 16 or 32
                if ((uY >= SPY) && (uY < SPY + SPDimY) ) {
                    if (SPOnline < f18a.SPMaxScanLine) {
                        SPWidth = SPHeight;
                        SPDimX = SPDimY;
                        SPX = tms.ram[SPAttrAddr + 1];
                        if (SPParentAttrAddr == 0)  {
                            if ((SPAttr & 0x80) != 0)  {
                                SPX -= 32; // Early clock
                            }
                        }
                        else {
                            // Linked
                            SPX = (SPX + tms.ram[SPParentAttrAddr + 1]) & 0xFF;
                            if ((tms.ram[SPParentAttrAddr + 3] & 0x80) != 0) {
                                SPX -= 32; // Early clock of parent
                            }
                        }
                        patternNo = (tms.ram[SPAttrAddr + 2] & (SPSize != 0 ? 0xFC : 0xFF));
                        SPFlipY = f18a.unlocked && (SPAttr & 0x20) != 0;
                        SPFlipX = f18a.unlocked && (SPAttr & 0x40) != 0;
                        baseColor = SPAttr & 0x0F;
                        SPPalBaseIdx = 0;
                        switch (F18A_SPColorMode)
                        {
                        case F18A_COLOR_MODE_NORMAL:
                            SPPalBaseIdx = f18a.SPPalSel;
                            break;
                        case F18A_COLOR_MODE_ECM_1:
                            SPPalBaseIdx = (f18a.SPPalSel & 0x20) | (baseColor << 1);
                            break;
                        case F18A_COLOR_MODE_ECM_2:
                            SPPalBaseIdx = (baseColor << 2);
                            break;
                        case F18A_COLOR_MODE_ECM_3:
                            SPPalBaseIdx = ((baseColor & 0x0e) << 2);
                            break;
                        }
                        SPGenBaseAddr = (tms.SprGen-tms.ram) + (patternNo << 3);
                        dy = (uY - SPY) >> SPMag;
                        if (SPFlipY) {
                            dy = SPHeight - dy - 1;
                        }
                        for (dx = 0; dx < SPWidth; dx += 8) {
                            SPGenAddr = SPGenBaseAddr + dy + (dx << 1);
                            SPGenByte0 = tms.ram[SPGenAddr];
                            SPGenByte1 = tms.ram[SPGenAddr + f18a.SPGSOfs];
                            SPGenByte2 = tms.ram[SPGenAddr + (f18a.SPGSOfs << 1)];
                            SPBit = 0x80;
                            SPBitShift2 = 7;
                            for (SPBitShift1 = 0; SPBitShift1 < 8; SPBitShift1++) {
                                pixelOn = 0;
                                switch (F18A_SPColorMode)
                                {
                                case F18A_COLOR_MODE_NORMAL:
                                    pixelOn = (SPGenByte0 & SPBit) != 0;
                                    sprColor = pixelOn ? baseColor : 0;
                                    break;
                                case F18A_COLOR_MODE_ECM_1:
                                    sprColor = (SPGenByte0 & SPBit) >> SPBitShift2;
                                    break;
                                case F18A_COLOR_MODE_ECM_2:
                                    sprColor =((SPGenByte0 & SPBit) >> SPBitShift2) |
                                                        (((SPGenByte1 & SPBit) >> SPBitShift2) << 1);
                                    break;
                                case F18A_COLOR_MODE_ECM_3:
                                    sprColor =((SPGenByte0 & SPBit) >> SPBitShift2) |
                                                        (((SPGenByte1 & SPBit) >> SPBitShift2) << 1) |
                                                        (((SPGenByte2 & SPBit) >> SPBitShift2) << 2);
                                    break;
                                }
                                if ( (sprColor > 0) || pixelOn) {
                                    x2 = SPX + ((SPFlipX ? SPDimX - (dx + SPBitShift1) - 1 : dx + SPBitShift1) << SPMag);
                                    if ((x2 >= 0) && (x2 < 256)) {
                                        if (SPColBuf[x2] == 0) {
                                            SPColBuf[x2] = sprColor + 1; // Add one here so 0 means uninitialized. Subtract one before drawing.
                                            SPPalBasIdxBuf[x2] = SPPalBaseIdx;
                                        }
                                        else {
                                            tms.SR|=TMS9918_STAT_OVRLAP;
                                        }
                                    }
                                    if (SPMag) {
                                        x2++;
                                        if ( (x2 >= 0) && (x2 < 256) ) {
                                            if (SPColBuf[x2] == 0) {
                                                SPColBuf[x2] = sprColor + 1; // Add one here so 0 means uninitialized. Subtract one before drawing.
                                                SPPalBasIdxBuf[x2] = SPPalBaseIdx;
                                            }
                                            else {
                                                tms.SR|=TMS9918_STAT_OVRLAP;
                                            }
                                        }
                                    } // if (SPMag) {
                                }
                                SPBit >>= 1;
                                SPBitShift2--;
                            } // for (SPBitShift1 = 0; SPBitShift1 < 8; SPBitShift1++) {
                        }
                    }
                }
                SPOnline++;
/*TODO                if (SPOnline == 5 && !this.fifthSprite) {
                    this.fifthSprite = true;
                    this.fifthSpriteIndex = index;
                }*/
            }
        }

        // Prepare values for Tile layer 1
        ChrTabBaseAddr = (tms.ChrTab-tms.ram);
        y1 = uY + f18a.TL1VOfs;
        if (y1 >= f18a.WinHeight) {
            y1 -= f18a.WinHeight;
            ChrTabBaseAddr ^= f18a.VPSize1;
        }
        rowOfs=(y1 >> 3) << 5;
        lineOfs = y1 & 7;

        // Prepare values for Bitmap layer
        if (F18A_BMLEnabled) {
            BMX2 = f18a.bitmapX + f18a.bitmapWidth;
            BMY1 = uY - f18a.bitmapY;
            BMY2 = f18a.bitmapY + f18a.bitmapHeight;
            BMYOfs = BMY1 * f18a.bitmapWidth;
        }

        // Draw line
        for (x = 0; x < 256; x++) {
            // Init default color / palette
            ColPix = tms.BGColor;
            PalBaseIdx=0;
            // Tile layer 1
            if (F18A_TL1Enabled) {
                // Graphic mode 2 specific code
                ChrTabAddr = ChrTabBaseAddr;
                x1 = x - (f18a.TL1HOfs << 0);
                if (x1 >= 256) {
                    x1 -= 256;
                    ChrTabAddr ^= f18a.HPSize1;
                }
                charNo = tms.ram[ChrTabAddr + (x1 >> 3) + rowOfs];
                bitShift = x1 & 7;
                bit = 0x80 >> bitShift;
                charSetOfs = (uY & 0xC0) << 5;
                GenByte = tms.ram[(tms.ChrGen -tms.ram) + (((charNo << 3) + charSetOfs) & tms.ChrGenM) + lineOfs];
                ColByte = tms.ram[(tms.ColTab -tms.ram) + (((charNo << 3) + charSetOfs) & tms.ColTabM) + lineOfs];
                TilCol = (GenByte & bit) != 0 ? (ColByte & 0xF0) >> 4 : (ColByte & 0x0F);
                if (TilCol > 0) {
                    ColPix = TilCol;
                    PalBaseIdx = f18a.TL1PalSel;
                }
                // End Graphic mode 2 specific code
            } // if (F18A_TL1Enabled) {

            // Bitmap layer
            if (F18A_BMLEnabled) {
                bmpX = x;
                if ( (bmpX >= f18a.bitmapX) && (bmpX < BMX2) && (uY >= f18a.bitmapY) && (uY < BMY2)) {
                    BMX1 = x - f18a.bitmapX;
                    BMPixOfs = BMX1 + BMYOfs;
                    BMByte = tms.ram[f18a.bitmapBaseAddr + (BMPixOfs >> 2)];
                    if (F18A_BMFat) {
                        // 16 color bitmap with fat pixels
                        BMBitShift = (2 - (BMPixOfs & 2)) << 1;
                        BMColor = (BMByte >> BMBitShift) & 0x0F;
                    }
                    else {
                        // 4 color bitmap
                        BMBitShift = (3 - (BMPixOfs & 3)) << 1;
                        BMColor = (BMByte >> BMBitShift) & 0x03;
                    }
                    if ((BMColor > 0 || !f18a.bitmapTransparent) && (ColPix == tms.BGColor || f18a.bitmapPriority)) {
                        ColPix = BMColor;
                        PalBaseIdx = f18a.bitmapPaletteSelect;
                    }
                }
            }

            // Sprite layer
            SPColor = 0;
            if ((!TilPrio || (TransCol0)) /*&& (ColPix == tms.BGColor)*/ && SPColBuf[x]) {
                SPColor = SPColBuf[x] - 1;
                if (SPColor > 0) {
                    ColPix = SPColor;
                    PalBaseIdx = SPPalBasIdxBuf[x];
                }
                else {
                    SPColor = 0;
                }
            }

            // Draw pixel
            *P++=ColPix+PalBaseIdx;
        }
    }
    else {
        // Empty scanline
        for (i = 0; i < 256; i++) {
            *P++=tms.BGColor;
        }
    }

    // Refresh border
    _F18A_refreshborder(uY);
}

void _F18A_modem(unsigned char uY)
{
    int i,x,bmpX, index,dx,dy,x1,y1,x2;
    BYTE *P,ColPix,PalBaseIdx;
    unsigned short ChrTabBaseAddr,ChrTabAddr;
    unsigned short rowOfs;
    unsigned short BMX1, BMX2,BMY1,BMY2,BMYOfs,BMPixOfs;
    byte BMByte ;
    BYTE charNo,TilPrio,TilCol,TransCol0, ColByte;
    BYTE BMBitShift, BMColor;
    BYTE SPColor;
    BYTE SPColBuf[256],SPPalBasIdxBuf[256];
    unsigned short SPMaxAttrAddr,SPParentAttrAddr,SPAttrAddr,SPGenBaseAddr,SPGenAddr;
    BYTE SCRYOut,SCRYNeg,SPOnline,SPLinkAttr,SPX,SPY;
    BYTE SPAttr,SPSize,SPMag,SPWidth,SPHeight,SPDimX,SPDimY;
    BYTE patternNo,SPFlipX,SPFlipY, baseColor,SPPalBaseIdx,pixelOn,sprColor;
    BYTE SPGenByte0,SPGenByte1,SPGenByte2,SPBit,SPBitShift1,SPBitShift2;

    // GO to current line
    P  = cv_display + TVW*(uY+(TVH-f18a.WinHeight)/2) + TVW/2-128;

    if (F18A_DisplayON) {
        // Prepare sprite colors
        memset(SPColBuf,0x00,sizeof(SPColBuf));
        memset(SPPalBasIdxBuf,0x00,sizeof(SPPalBasIdxBuf));
        SPOnline = 0;
        SCRYOut = f18a.Row30 ? 0xF0 : 0xC0;
        SCRYNeg = f18a.Row30 ? 0xF0 : 0xD0;
        SPMaxAttrAddr = tms.SprTab -tms.ram + (f18a.SPMax << 2);
        for (SPAttrAddr = tms.SprTab-tms.ram, index = 0; (f18a.Row30 || tms.ram[SPAttrAddr] != 0xd0) && (SPAttrAddr < SPMaxAttrAddr) && (SPOnline <= f18a.SPMaxScanLine); SPAttrAddr += 4, index++) {
            SPParentAttrAddr = 0;
            if (F18A_SPLinkEnabled) {
                SPLinkAttr = tms.ram[(tms.SprTab -tms.ram) + 0x80 + ((SPAttrAddr - (tms.SprTab - tms.ram) ) >> 2)];
                if ((SPLinkAttr & 0x20) != 0)  {
                    SPParentAttrAddr = (tms.SprTab-tms.ram) + ((SPLinkAttr & 0x1F) << 2);
                }
            }
            SPY = tms.ram[SPAttrAddr];
            if (SPParentAttrAddr != 0) {
                SPY = (SPY + tms.ram[SPParentAttrAddr]) & 0xFF;
            }
            if (!F18A_SPRealYCoord) {
                SPY++;
            }
            if ( (SPY < SCRYOut) || (SPY > SCRYNeg)) {
                if (SPY > SCRYNeg)  {
                    SPY -= 256;
                }
                SPAttr = tms.ram[SPAttrAddr + 3];
                SPSize = !f18a.unlocked || (SPAttr & 0x10) == 0 ? F18A_SpriteSize : 1;
                SPMag = F18A_SpriteMag;
                SPHeight = 8 << SPSize; // 8 or 16
                SPDimY = SPHeight << SPMag; // 8, 16 or 32
                if ((uY >= SPY) && (uY < SPY + SPDimY) ) {
                    if (SPOnline < f18a.SPMaxScanLine) {
                        SPWidth = SPHeight;
                        SPDimX = SPDimY;
                        SPX = tms.ram[SPAttrAddr + 1];
                        if (SPParentAttrAddr == 0)  {
                            if ((SPAttr & 0x80) != 0)  {
                                SPX -= 32; // Early clock
                            }
                        }
                        else {
                            // Linked
                            SPX = (SPX + tms.ram[SPParentAttrAddr + 1]) & 0xFF;
                            if ((tms.ram[SPParentAttrAddr + 3] & 0x80) != 0) {
                                SPX -= 32; // Early clock of parent
                            }
                        }
                        patternNo = (tms.ram[SPAttrAddr + 2] & (SPSize != 0 ? 0xFC : 0xFF));
                        SPFlipY = f18a.unlocked && (SPAttr & 0x20) != 0;
                        SPFlipX = f18a.unlocked && (SPAttr & 0x40) != 0;
                        baseColor = SPAttr & 0x0F;
                        SPPalBaseIdx = 0;
                        switch (F18A_SPColorMode)
                        {
                        case F18A_COLOR_MODE_NORMAL:
                            SPPalBaseIdx = f18a.SPPalSel;
                            break;
                        case F18A_COLOR_MODE_ECM_1:
                            SPPalBaseIdx = (f18a.SPPalSel & 0x20) | (baseColor << 1);
                            break;
                        case F18A_COLOR_MODE_ECM_2:
                            SPPalBaseIdx = (baseColor << 2);
                            break;
                        case F18A_COLOR_MODE_ECM_3:
                            SPPalBaseIdx = ((baseColor & 0x0e) << 2);
                            break;
                        }
                        SPGenBaseAddr = (tms.SprGen-tms.ram) + (patternNo << 3);
                        dy = (uY - SPY) >> SPMag;
                        if (SPFlipY) {
                            dy = SPHeight - dy - 1;
                        }
                        for (dx = 0; dx < SPWidth; dx += 8) {
                            SPGenAddr = SPGenBaseAddr + dy + (dx << 1);
                            SPGenByte0 = tms.ram[SPGenAddr];
                            SPGenByte1 = tms.ram[SPGenAddr + f18a.SPGSOfs];
                            SPGenByte2 = tms.ram[SPGenAddr + (f18a.SPGSOfs << 1)];
                            SPBit = 0x80;
                            SPBitShift2 = 7;
                            for (SPBitShift1 = 0; SPBitShift1 < 8; SPBitShift1++) {
                                pixelOn = 0;
                                switch (F18A_SPColorMode)
                                {
                                case F18A_COLOR_MODE_NORMAL:
                                    pixelOn = (SPGenByte0 & SPBit) != 0;
                                    sprColor = pixelOn ? baseColor : 0;
                                    break;
                                case F18A_COLOR_MODE_ECM_1:
                                    sprColor = (SPGenByte0 & SPBit) >> SPBitShift2;
                                    break;
                                case F18A_COLOR_MODE_ECM_2:
                                    sprColor =((SPGenByte0 & SPBit) >> SPBitShift2) |
                                                        (((SPGenByte1 & SPBit) >> SPBitShift2) << 1);
                                    break;
                                case F18A_COLOR_MODE_ECM_3:
                                    sprColor =((SPGenByte0 & SPBit) >> SPBitShift2) |
                                                        (((SPGenByte1 & SPBit) >> SPBitShift2) << 1) |
                                                        (((SPGenByte2 & SPBit) >> SPBitShift2) << 2);
                                    break;
                                }
                                if ( (sprColor > 0) || pixelOn) {
                                    x2 = SPX + ((SPFlipX ? SPDimX - (dx + SPBitShift1) - 1 : dx + SPBitShift1) << SPMag);
                                    if ((x2 >= 0) && (x2 < 256)) {
                                        if (SPColBuf[x2] == 0) {
                                            SPColBuf[x2] = sprColor + 1; // Add one here so 0 means uninitialized. Subtract one before drawing.
                                            SPPalBasIdxBuf[x2] = SPPalBaseIdx;
                                        }
                                        else {
                                            tms.SR|=TMS9918_STAT_OVRLAP;
                                        }
                                    }
                                    if (SPMag) {
                                        x2++;
                                        if ( (x2 >= 0) && (x2 < 256) ) {
                                            if (SPColBuf[x2] == 0) {
                                                SPColBuf[x2] = sprColor + 1; // Add one here so 0 means uninitialized. Subtract one before drawing.
                                                SPPalBasIdxBuf[x2] = SPPalBaseIdx;
                                            }
                                            else {
                                                tms.SR|=TMS9918_STAT_OVRLAP;
                                            }
                                        }
                                    } // if (SPMag) {
                                }
                                SPBit >>= 1;
                                SPBitShift2--;
                            } // for (SPBitShift1 = 0; SPBitShift1 < 8; SPBitShift1++) {
                        }
                    }
                }
                SPOnline++;
/*TODO                if (SPOnline == 5 && !this.fifthSprite) {
                    this.fifthSprite = true;
                    this.fifthSpriteIndex = index;
                }*/
            }
        }

        // Prepare values for Tile layer 1
        ChrTabBaseAddr = (tms.ChrTab-tms.ram);
        y1 = uY + f18a.TL1VOfs;
        if (y1 >= f18a.WinHeight) {
            y1 -= f18a.WinHeight;
            ChrTabBaseAddr ^= f18a.VPSize1;
        }
        rowOfs=(y1 >> 3) << 5;

        // Prepare values for Bitmap layer
        if (F18A_BMLEnabled) {
            BMX2 = f18a.bitmapX + f18a.bitmapWidth;
            BMY1 = uY - f18a.bitmapY;
            BMY2 = f18a.bitmapY + f18a.bitmapHeight;
            BMYOfs = BMY1 * f18a.bitmapWidth;
        }

        // Draw line
        for (x = 0; x < 256; x++) {
            // Init default color / palette
            ColPix = tms.BGColor;
            PalBaseIdx=0;
            // Tile layer 1
            if (F18A_TL1Enabled) {
                // Multicolor mode specific code
                ChrTabAddr = ChrTabBaseAddr;
                x1 = x - (f18a.TL1HOfs << 0);
                if (x1 >= 256) {
                    x1 -= 256;
                    ChrTabAddr ^= f18a.HPSize1;
                }
                charNo = tms.ram[ChrTabAddr + (x1 >> 3) + rowOfs];
                ColByte = tms.ram[(tms.ChrGen -tms.ram) + (charNo << 3) + ((y1 & 0x1c) >> 2)];
                TilCol = (x1 & 4) == 0 ? (ColByte & 0xf0) >> 4 : (ColByte & 0x0f);
                if (TilCol > 0) {
                    ColPix = TilCol;
                    PalBaseIdx = f18a.TL1PalSel;
                }

                // End Multicolor mode specific code
            } // if (F18A_TL1Enabled) {

            // Bitmap layer
            if (F18A_BMLEnabled) {
                bmpX = x;
                if ( (bmpX >= f18a.bitmapX) && (bmpX < BMX2) && (uY >= f18a.bitmapY) && (uY < BMY2)) {
                    BMX1 = x - f18a.bitmapX;
                    BMPixOfs = BMX1 + BMYOfs;
                    BMByte = tms.ram[f18a.bitmapBaseAddr + (BMPixOfs >> 2)];
                    if (F18A_BMFat) {
                        // 16 color bitmap with fat pixels
                        BMBitShift = (2 - (BMPixOfs & 2)) << 1;
                        BMColor = (BMByte >> BMBitShift) & 0x0F;
                    }
                    else {
                        // 4 color bitmap
                        BMBitShift = (3 - (BMPixOfs & 3)) << 1;
                        BMColor = (BMByte >> BMBitShift) & 0x03;
                    }
                    if ((BMColor > 0 || !f18a.bitmapTransparent) && (ColPix == tms.BGColor || f18a.bitmapPriority)) {
                        ColPix = BMColor;
                        PalBaseIdx = f18a.bitmapPaletteSelect;
                    }
                }
            }

            // Sprite layer
            SPColor = 0;
            if ((!TilPrio || (TransCol0)) /*&& (ColPix == tms.BGColor)*/ && SPColBuf[x]) {
                SPColor = SPColBuf[x] - 1;
                if (SPColor > 0) {
                    ColPix = SPColor;
                    PalBaseIdx = SPPalBasIdxBuf[x];
                }
                else {
                    SPColor = 0;
                }
            }

            // Draw pixel
            *P++=ColPix+PalBaseIdx;
        }
    }
    else {
        // Empty scanline
        for (i = 0; i < 256; i++) {
            *P++=tms.BGColor;
        }
    }

    // Refresh border
    _F18A_refreshborder(uY);
}

