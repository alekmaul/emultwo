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

tF18A f18a;

//---------------------------------------------------------------------------
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

#pragma package(smart_init)
// ----------------------------------------------------------------------------------------

// Reset the tms.VR
void f18a_reset(void) {
    int i,j;

    // Init palette
    j=0;
    for (i = 0; i < 64; i++,j+=3)
    {
        cv_palette[j]=F18A_palette[j]*17;
        cv_palette[j+1]=F18A_palette[j+1]*17;
        cv_palette[j+2]=F18A_palette[j+2]*17;
    }
/*
    this.latch = false;
*/
    // Init specific var regarding chipset
    f18a.VAddrInc = 1;
    f18a.unlocked = 0;
    f18a.statusRegisterNo = 0;
    f18a.DPM = 0;
    f18a.PalAutoInc = 0;
    f18a.PalRegNo = 0;
    f18a.PalRegVal = 255;
    f18a.gpuAddressLatch = 0;
    f18a.Mode = F18A_MODE_GRAPHICS;
    f18a.PalRecalc = 1;

/*
        f18a.currentScanline = 0;
        f18a.fakeScanline = null;
        f18a.blanking = 0;
        this.displayOn = true;
        this.interruptsOn = false;
        tms.ColTab = 0;
        tms.ChrTab = 0;
        tms.ChrGen = 0;
        tms.SprTab = 0;
        tms.SprGen = 0;
        tms.ColTabMask = 0x3FFF;
        this.patternTableMask = 0x3FFF;
        tms.FGColor = 0;
        tms.BGColor = 7;
		this.spriteSize = 0;
		this.spriteMag = 0;

        this.tileColorMode = 0;
        this.tilePaletteSelect = 0;
        this.tilePaletteSelect2 = 0;
        this.spriteColorMode = 0;
        this.spritePaletteSelect = 0;
        this.realSpriteYCoord = 0;
        tms.ColTab2 = 0;
        tms.ChrTab2 = 0;
        this.tileLayer1Enabled = true;
        this.tileLayer2Enabled = false;
        f18a.Row30 = false;
        this.spriteLinkingEnabled = false;
        this.hScroll1 = 0;
        this.vScroll1 = 0;
        this.hScroll2 = 0;
        this.vScroll2 = 0;
        this.hPageSize1 = 0;
        this.vPageSize1 = 0;
        this.hPageSize2 = 0;
        this.vPageSize2 = 0;
        this.bitmapEnable = false;
        this.bitmapPriority = false;
        this.bitmapTransparent = false;
        this.bitmapFat = false;
        this.bitmapPaletteSelect = 0;
        this.bitmapBaseAddr = 0;
        this.bitmapX = 0;
        this.bitmapY = 0;
        this.bitmapWidth = 0;
        this.bitmapHeight = 0;
        this.interruptScanline = 0;
        this.maxScanlineSprites = F18A.MAX_SCANLINE_SPRITES_JUMPER && !this.enableFlicker ? 32 : 4;
        this.maxSprites = 32;
        this.tileMap2AlwaysOnTop = true;
        this.ecmPositionAttributes = false;
        this.reportMax = false;
        this.scanLines = F18A.SCANLINES_JUMPER;
        this.gpuHsyncTrigger = false;
        this.gpuVsyncTrigger = false;
        this.spritePlaneOffset = 0x800;
        this.tilePlaneOffset = 0x800;
        this.counterElapsed = 0;
        this.counterStart = this.getTime();
        this.counterSnap = 0;
*/
    // Init all registers
    f18a_resetregs();

    // reset gpu
    f18agpu_reset();

    // last, reset tms 9918
    tms9918_reset();
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

void f18a_updatemode(BYTE reg0,BYTE reg1)
{
    BYTE oldmode=f18a.Mode;

    // Check bitmap mode bit, not text or multicolor
    if ((reg0 & 0x2) != 0 && (reg1 & 0x18) == 0)            // #define TMS9918_Mode            (((tms.VR[0]&0x02)>>1)|(((tms.VR[1]&0x18)>>2)))
    {                                                       //                                               0 ou 1                   0 2 4 6
        // Bitmap mode
        f18a.Mode = F18A_MODE_BITMAP;
    }
    else
    {
        switch ((reg1 & 0x18) >> 3)     // 00011000
        {
        case 0: // Graphics mode 0
            f18a.Mode = F18A_MODE_GRAPHICS;
            break;
        case 1: // Multicolor mode
            f18a.Mode = F18A_MODE_MULTICOLOR;
            break;
        case 2: // Text mode
            if ((reg0 & 0x04) == 0)
            {
                f18a.Mode = F18A_MODE_TEXT;
            }
            else
            {
                f18a.Mode = F18A_MODE_TEXT_80;
            }
            break;
        }
    }
    if (f18a.Mode == F18A_MODE_BITMAP)
    {
        tms.ColTab = tms.ram+((f18a.VDPR[3] & 0x80) << 6);
        tms.ChrGen = tms.ram+((f18a.VDPR[4] & 0x4) << 11);
        //this.updateTableMasks();
    }
    else
    {
        tms.ColTab = tms.ram+(f18a.VDPR[3] << 6);
        tms.ChrGen = tms.ram+((f18a.VDPR[4] & 0x7) << 11);
    }
    tms.ChrTab = tms.ram+((f18a.VDPR[2] & (f18a.Mode != F18A_MODE_TEXT_80 || f18a.unlocked ? 0xf : 0xc)) << 10);
    tms.SprTab = tms.ram+((f18a.VDPR[5] & 0x7f) << 7);
    tms.SprGen = tms.ram+((f18a.VDPR[6] & 0x7) << 11);
    if (oldmode != f18a.Mode)
    {
        f18a_setwindowsize();
    }
};
// ----------------------------------------------------------------------------------------

// Writing a data to a F18A register
void WriteF18A(int iReg,unsigned char value) {
    unsigned char oldrunsr,runsr,oldvalue,gromclock;

    // store value
    oldvalue = f18a.VDPR[iReg];
    f18a.VDPR[iReg] = value;

	// Depending on the register, do...
    switch (iReg)
    {
    case 0x00: // Do it with TMS
        Write9918(iReg,value);
        f18a_updatemode(f18a.VDPR[0], f18a.VDPR[1]);
        break;
    case 0x01:
        Write9918(iReg,value);
        f18a_updatemode(f18a.VDPR[0], f18a.VDPR[1]);
        break;
    /*
                    this.displayOn = (f18a.VDPR[1] & 0x40) !== 0;
                this.interruptsOn = (f18a.VDPR[1] & 0x20) !== 0;
                this.spriteSize = (f18a.VDPR[1] & 0x02) >> 1;
                this.spriteMag = f18a.VDPR[1] & 0x01;
                this.updateMode(f18a.VDPR[0], f18a.VDPR[1]);
                */
    case 0x02: // // Name table
        //tms.ChrTab = tms.ram+(f18a.VDPR[2] & (f18a.Mode != F18A.MODE_TEXT_80 || this.unlocked ? 0xf : 0xc)) << 10;
        //tms.ChrTab = (f18a.VDPR[2] & (f18a.Mode !== F18A.MODE_TEXT_80 || this.unlocked ? 0xf : 0xc)) << 10;
        //break;
    case 0x03: // Color table
    /*
                    if (f18a.Mode === F18A.MODE_BITMAP) {
                    tms.ColTab = (f18a.VDPR[3] & 0x80) << 6;
                }
                else {
                    tms.ColTab = f18a.VDPR[3] << 6;
                }
                this.updateTableMasks();
                */
    case 0x04: // Pattern table
    /*
                    if (f18a.Mode === F18A.MODE_BITMAP) {
                    tms.ChrGen = (f18a.VDPR[4] & 0x4) << 11;
                }
                else {
                    tms.ChrGen = (f18a.VDPR[4] & 0x7) << 11;
                }
                this.updateTableMasks();
                */
    case 0x05: // Sprite attribute table
        //tms.SprTab = (f18a.VDPR[5] & 0x7f) << 7;
    case 0x06: // Sprite pattern table
        //tms.SprGen = (f18a.VDPR[6] & 0x7) << 11;
    case 0x07:
        Write9918(iReg,value);
        /*
                        tms.FGColor = (this.registers[7] & 0xf0) >> 4;
                tms.BGColor = this.registers[7] & 0x0f;
*/
        break;
    case 0x0A: // Name table 2 base address
        f18a.nameTable2 = (f18a.VDPR[0x0A] & 0x0f) << 10;
        break;
    case 0x0B: // Color Table 2 Base Address, 64-byte boundaries
        f18a.colorTable2 = f18a.VDPR[0x0B] << 6;
        break;
    case 0x0F: // Status register select / counter control
        f18a.statusRegisterNo = f18a.VDPR[0x0F] & 0x0f;
        oldrunsr = (oldvalue & 0x10) != 0;
        runsr = (f18a.VDPR[0x0F] & 0x10) != 0;
        if (oldrunsr && !runsr)
        {
            // Stop
            f18a.counterElapsed += (0xDEAF/*this.getTime()*/ - f18a.counterStart);
        }
        else if (!oldrunsr && runsr)
        {
            // Start
            f18a.counterStart = 0xDEAF/*this.getTime()*/;
        }
        if ((f18a.VDPR[0x0F] & 0x20) != 0)
        {
            // Snapshot
            if (runsr)
            {
                // Started
                f18a.counterSnap = (0xDEAF/*this.getTime()*/ - f18a.counterStart); // + this.counterElapsed;
            }
            else
            {
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
        f18a.spritePaletteSelect = f18a.VDPR[0x18] & 0x30;
        f18a.tilePaletteSelect = (f18a.VDPR[0x18] & 0x03) << 4; // Shift into position
        f18a.tilePaletteSelect2 = (f18a.VDPR[0x18] & 0x0C) << 2; // Shift into position
        break;
    case 0x19: // Horizontal scroll offset 2
        f18a.hScroll2 = f18a.VDPR[0x19];
        break;
    case 0x1A: // Vertical scroll offset 2
        f18a.vScroll2 = f18a.VDPR[0x1A];
        break;
    case 0x1B: // Horizontal scroll offset 1
        f18a.hScroll1 = f18a.VDPR[0x1B];
        break;
    case 0x1C: // Vertical scroll offset 1
        f18a.vScroll1 = f18a.VDPR[0x1C];
        break;
    case 0x1D: // Page size
        f18a.hPageSize1 = (f18a.VDPR[0x1D] & 0x02) << 9;
        f18a.vPageSize1 = (f18a.VDPR[0x1D] & 0x01) << 11;
        f18a.hPageSize2 = (f18a.VDPR[0x1D] & 0x20) << 5;
        f18a.vPageSize2 = (f18a.VDPR[0x1D] & 0x10) << 7;
        f18a.spritePlaneOffset = 0x100 << (3 - ((f18a.VDPR[0x1D] & 0xC0) >> 6));
        f18a.tilePlaneOffset = 0x100 << (3 - ((f18a.VDPR[0x1D] & 0x0C) >> 2));
        break;

    // Setting this to 0 restores the jumper value (4 or 32). Here assumed to be 32.
    // Setting this to 31 means all 32 sprites can be displayed.
    // You cannot choose to have 31 displayable sprites on a scanline.
    case 0x1E: // Max displayable sprites on a scanline
        if (f18a.VDPR[0x1E] == 0)
        {
            f18a.VDPR[0x1E] = F18A_MAX_SCANLINE_SPRITES_JUMPER ? 31 : 4;
        }
        f18a.maxScanlineSprites = f18a.VDPR[0x1E];
        if (f18a.maxScanlineSprites == 31)
        {
            f18a.maxScanlineSprites = 32;
        }
        break;
    case 0x1F: // Bitmap control
        f18a.bitmapEnable = (f18a.VDPR[0x1F] & 0x80) != 0;
        f18a.bitmapPriority = (f18a.VDPR[0x1F] & 0x40) != 0;
        f18a.bitmapTransparent = (f18a.VDPR[0x1F] & 0x20) != 0;
        f18a.bitmapFat = (f18a.VDPR[0x1F] & 0x10) != 0;
        f18a.bitmapPaletteSelect = (f18a.VDPR[0x1F] & (f18a.bitmapFat ? 0x0C : 0x0F)) << 2; // Shift into position
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
        if (f18a.bitmapWidth == 0)
        {
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
        if (oldvalue != f18a.Row30)
        {
            f18a_setwindowsize();
        }
        f18a.tileColorMode = (f18a.VDPR[0x31] & 0x30) >> 4;
        f18a.realSpriteYCoord = (f18a.VDPR[0x31] & 0x08) != 0;
        f18a.spriteLinkingEnabled = (f18a.VDPR[0x31] & 0x04) != 0;
        f18a.spriteColorMode = f18a.VDPR[0x31] & 0x03;
        break;
    case 0x32: // Position vs name attributes, TL2 always on top
        // Write 1 to reset all VDP registers
        if ((f18a.VDPR[0x32] & 0x80) != 0)
        {
            f18a_resetregs();
            f18a.unlocked = 0;
            //this.updateMode(f18a.VDPR[0], f18a.VDPR[1]);
            return;
        }
        f18a.gpuHsyncTrigger = (f18a.VDPR[0x32] & 0x40) != 0;
        f18a.gpuVsyncTrigger = (f18a.VDPR[0x32] & 0x20) != 0;
        f18a.tileLayer1Enabled = (f18a.VDPR[0x32] & 0x10) == 0; // 0 = normal, 1 = disable GM1, GM2, MCM, T40, T80
        f18a.reportMax = (f18a.VDPR[0x32] & 0x08) != 0; // Report sprite max vs 5th sprite
        f18a.scanLines = (f18a.VDPR[0x32] & 0x04) != 0; // Draw scan lines
        f18a.ecmPositionAttributes = (f18a.VDPR[0x32] & 0x02) != 0; // 0 = per name attributes in ECMs, 1 = per position attributes
        f18a.tileMap2AlwaysOnTop = (f18a.VDPR[0x32] & 0x01) == 0; // 0 = TL2 always on top, 1 = TL2 vs sprite priority is considered
        break;
    case 0x33: // Stop Sprite (zero based) to limit the total number of sprites to process. Defaults to 32, i.e. no stop sprite
        f18a.maxSprites = f18a.VDPR[0x33] & 0x3F;
        break;
    case 0x36: // GPU address MSB
        f18a.gpuAddressLatch = 1;
        break;
    case 0x37: // GPU address LSB
        if (f18a.gpuAddressLatch)
        {
            f18a.gpuAddressLatch = 0;
            f18agpu_intreset();
            f18agpu_setpc((f18a.VDPR[54] << 8 | f18a.VDPR[55]));
        }
        break;
    case 0x38:
        f18agpu.cpuIdle=((f18a.VDPR[0x38] & 1) != 0) ? 0 : 1;
        break;
    case 0x39:
        if (!f18a.unlocked)
        {
            if ((oldvalue & 0x1c) == 0x1c && (f18a.VDPR[0x39] & 0x1c) == 0x1c)
            {
                f18a.unlocked = 1;
            }
        }
        else
        {
            f18a.VDPR[0x39] = 0;
            f18a.unlocked = 0;
        }
        //this.updateMode(f18a.VDPR[0], f18a.VDPR[1]);
        break;
    case 0x3A:
        gromclock = f18a.VDPR[0x3A] & 0x0F;
        if (gromclock < 7)
        {
            gromclock = 6;
        }
        gromclock = (gromclock << 4) | 0x0F;
        //this.tms9919.setGROMClock(gromClock);
        break;
    }
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
                WriteF18A(reg, tms.VAddr & 0x00FF);
            }
            else
            {
                if ((f18a.VDPR[0] & 0x04) == 0) // 1.8 firmware: writes to registers > 7 are masked if 80 columns mode is not enabled
                {
                    Write9918(reg & 0x07, tms.VAddr & 0x00FF);
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
    if (f18a.statusRegisterNo==0)
    {
        // default TMS register
        retval = tms.SR;
        tms.SR &= TMS9918_STAT_5THNUM|TMS9918_STAT_5THSPR;

        z80_set_irq_line(INPUT_LINE_NMI, CLEAR_LINE);
    }
    else
    {
        switch (f18a.statusRegisterNo)
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
/*
    setDimensions: function (force) {
        var newCanvasWidth = this.screenMode === F18A.MODE_TEXT_80 ? 608 : 304;
        var newCanvasHeight = this.screenMode === F18A.MODE_TEXT_80 ? 480 : 240;
        var newDimensions = force || newCanvasWidth !== this.canvas.width || newCanvasHeight !== this.canvas.height;
        if (newDimensions) {
            this.canvas.width = this.canvasWidth = newCanvasWidth;
            this.canvas.height = this.canvasHeight = newCanvasHeight;
        }
    */
        TVW = (f18a.Mode == F18A_MODE_TEXT_80 ? TVW_F18A*2 : TVW_F18A);
        TVH = (f18a.Row30 ? TVH_F18A : TVH_TMS);
        AccurateInit(true);
/*
        this.leftBorder = Math.floor((this.canvasWidth - this.drawWidth) >> 1);
        this.topBorder = Math.floor(((this.canvasHeight >> (this.screenMode === F18A.MODE_TEXT_80 ? 1 : 0)) - this.drawHeight) >> 1);
        if (newDimensions) {
            this.fillCanvas(this.bgColor);
            this.imagedata = this.canvasContext.getImageData(0, 0, this.canvasWidth, this.canvasHeight);
            this.imagedataData = this.imagedata.data;
        }
    },
*/
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
    if((tms.CurLine>=TMS9918_START_LINE)&&(tms.CurLine<TMS9918_END_LINE))
    {

		if(tms.UCount>=100)
        {
	        (SCR[tms.Mode].Refresh)(tms.CurLine-TMS9918_START_LINE);
		}
        else
            ScanSprites(tms.CurLine-TMS9918_START_LINE,0);

    	// and execute GPU opcodes if needed
        f18agpu_execute(F18AGPU_CYCLES_PER_SCANLINE);
	}

	// Check if VBlank
	if(tms.CurLine==TMS9918_END_LINE)
    {
		// Check if we are currently drawing screen
		if(tms.UCount>=100)
        {
			// Refresh all screen
			//coleco_paint();
			// Reset update counter
			tms.UCount-=100;
		}

		// Decrement/reset update counter
		tms.UCount+=TMS9918_DRAWFRAMES;

		// Generate IRQ when enabled and when VBlank flag goes up
		if (TMS9918_VBlankON && !(tms.SR&TMS9918_STAT_VBLANK) )
        {
            bIRQ = 1;
        }

		// Set VBlank status flag
		tms.SR|=TMS9918_STAT_VBLANK;

		// Set Sprite Collision status flag
		if(!(tms.SR&TMS9918_STAT_OVRLAP))
			if(CheckSprites()) tms.SR|=TMS9918_STAT_OVRLAP;
	}

	// Return IRQ (1) or not (0)
	return(bIRQ);
}

// ----------------------------------------------------------------------------------------
unsigned char f18a_drawscanline(int line)
{
        //var imagedata = this.imagedataData;
        //var imagedataAddr = this.imagedataAddr;
#if 0
    if (F18A_DisplayON)
    {
//            y -= this.topBorder;
            // Prepare sprites
//            var spriteColorBuffer, spritePaletteBaseIndexBuffer;

        if (f18a.unlocked || (f18a.Mode != F18A_MODE_TEXT && f18a.Mode != F18A_MODE_TEXT_80))
        {
/*
            spriteColorBuffer = new Uint8Array(this.drawWidth);
            spritePaletteBaseIndexBuffer = new Uint8Array(this.drawWidth);
            var spritesOnLine = 0;
            var outOfScreenY = f18a.Row30 ? 0xF0 : 0xC0;
            var negativeScreenY = f18a.Row30 ? 0xF0 : 0xD0;
            var maxSpriteAttrAddr = tms.SprTab + (this.maxSprites << 2);
            for (var spriteAttrAddr = tms.SprTab, index = 0; (f18a.Row30 || this.ram[spriteAttrAddr] !== 0xd0) && spriteAttrAddr < maxSpriteAttrAddr && spritesOnLine <= this.maxScanlineSprites; spriteAttrAddr += 4, index++)
            {
                var parentSpriteAttrAddr = null;
                if (this.spriteLinkingEnabled)
                {
                    var spriteLinkingAttr = this.ram[tms.SprTab + 0x80 + ((spriteAttrAddr - tms.SprTab) >> 2)];
                    if ((spriteLinkingAttr & 0x20) !== 0)
                    {
                        parentSpriteAttrAddr = tms.SprTab + ((spriteLinkingAttr & 0x1F) << 2);
                    }
                }
                var spriteY = this.ram[spriteAttrAddr];
                if (parentSpriteAttrAddr !== null)
                {
                    spriteY = (spriteY + this.ram[parentSpriteAttrAddr]) & 0xFF;
                }
                if (!this.realSpriteYCoord)
                {
                    spriteY++;
                }
                if (spriteY < outOfScreenY || spriteY > negativeScreenY)
                {
                    if (spriteY > negativeScreenY)
                    {
                        spriteY -= 256;
                    }
                    var spriteAttr = this.ram[spriteAttrAddr + 3];
                    var spriteSize = !this.unlocked || (spriteAttr & 0x10) === 0 ? this.spriteSize : 1;
                    var spriteMag = this.spriteMag;
                    var spriteHeight = 8 << spriteSize; // 8 or 16
                    var spriteDimensionY = spriteHeight << spriteMag; // 8, 16 or 32
                    if (y >= spriteY && y < spriteY + spriteDimensionY)
                    {
                        if (spritesOnLine < this.maxScanlineSprites)
                        {
                            //noinspection JSSuspiciousNameCombination
                            var spriteWidth = spriteHeight;
                            //noinspection JSSuspiciousNameCombination
                            var spriteDimensionX = spriteDimensionY;
                            var spriteX = this.ram[spriteAttrAddr + 1];
                            if (parentSpriteAttrAddr === null)
                            {
                                if ((spriteAttr & 0x80) !== 0)
                                {
                                    spriteX -= 32; // Early clock
                                }
                            }
                            else
                            {
                                // Linked
                                spriteX = (spriteX + this.ram[parentSpriteAttrAddr + 1]) & 0xFF;
                                if ((this.ram[parentSpriteAttrAddr + 3] & 0x80) !== 0)
                                {
                                    spriteX -= 32; // Early clock of parent
                                }
                            }
                            var patternNo = (this.ram[spriteAttrAddr + 2] & (spriteSize !== 0 ? 0xFC : 0xFF));
                            var spriteFlipY = this.unlocked && (spriteAttr & 0x20) !== 0;
                            var spriteFlipX = this.unlocked && (spriteAttr & 0x40) !== 0;
                            var baseColor = spriteAttr & 0x0F;
                            var sprPaletteBaseIndex = 0;
                            switch (this.spriteColorMode)
                            {
                            case F18A.COLOR_MODE_NORMAL:
                                sprPaletteBaseIndex = this.spritePaletteSelect;
                                break;
                            case F18A.COLOR_MODE_ECM_1:
                                    sprPaletteBaseIndex = (this.spritePaletteSelect & 0x20) | (baseColor << 1);
                                break;
                            case F18A.COLOR_MODE_ECM_2:
                                sprPaletteBaseIndex = (baseColor << 2);
                                break;
                            case F18A.COLOR_MODE_ECM_3:
                                sprPaletteBaseIndex = ((baseColor & 0x0e) << 2);
                                break;
                            }
                            var spritePatternBaseAddr = tms.SprGen + (patternNo << 3);
                            var dy = (y - spriteY) >> spriteMag;
                            if (spriteFlipY)
                            {
                                dy = spriteHeight - dy - 1;
                            }
                            for (var dx = 0; dx < spriteWidth; dx += 8)
                            {
                                var spritePatternAddr = spritePatternBaseAddr + dy + (dx << 1);
                                var spritePatternByte0 = this.ram[spritePatternAddr];
                                var spritePatternByte1 = this.ram[spritePatternAddr + this.spritePlaneOffset];
                                var spritePatternByte2 = this.ram[spritePatternAddr + (this.spritePlaneOffset << 1)];
                                var spriteBit = 0x80;
                                var spriteBitShift2 = 7;
                                for (var spriteBitShift1 = 0; spriteBitShift1 < 8; spriteBitShift1++)
                                {
                                    var sprColor;
                                    var pixelOn = 0;
                                    switch (this.spriteColorMode)
                                    {
                                    case F18A.COLOR_MODE_NORMAL:
                                        pixelOn = (spritePatternByte0 & spriteBit) !== 0;
                                        sprColor = pixelOn ? baseColor : 0;
                                        break;
                                    case F18A.COLOR_MODE_ECM_1:
                                        sprColor = (spritePatternByte0 & spriteBit) >> spriteBitShift2;
                                        break;
                                    case F18A.COLOR_MODE_ECM_2:
                                        sprColor =
                                                        ((spritePatternByte0 & spriteBit) >> spriteBitShift2) |
                                                        (((spritePatternByte1 & spriteBit) >> spriteBitShift2) << 1);
                                            break;
                                        case F18A.COLOR_MODE_ECM_3:
                                            sprColor =
                                                        ((spritePatternByte0 & spriteBit) >> spriteBitShift2) |
                                                        (((spritePatternByte1 & spriteBit) >> spriteBitShift2) << 1) |
                                                        (((spritePatternByte2 & spriteBit) >> spriteBitShift2) << 2);
                                            break;
                                    }
                                    if (sprColor > 0 || pixelOn)
                                    {
                                        var x2 = spriteX + ((spriteFlipX ? spriteDimensionX - (dx + spriteBitShift1) - 1 : dx + spriteBitShift1) << spriteMag);
                                        if (x2 >= 0 && x2 < this.drawWidth)
                                        {
                                            if (spriteColorBuffer[x2] === 0)
                                            {
                                                spriteColorBuffer[x2] = sprColor + 1; // Add one here so 0 means uninitialized. Subtract one before drawing.
                                                spritePaletteBaseIndexBuffer[x2] = sprPaletteBaseIndex;
                                            }
                                            else
                                            {
                                                this.collision = true;
                                            }
                                        }
                                        if (spriteMag)
                                        {
                                            x2++;
                                            if (x2 >= 0 && x2 < this.drawWidth)
                                            {
                                                if (spriteColorBuffer[x2] === 0)
                                                {
                                                    spriteColorBuffer[x2] = sprColor + 1; // Add one here so 0 means uninitialized. Subtract one before drawing.
                                                    spritePaletteBaseIndexBuffer[x2] = sprPaletteBaseIndex;
                                                }
                                                else
                                                {
                                                    this.collision = true;
                                                }
                                            }
                                        }
                                    }
                                    spriteBit >>= 1;
                                    spriteBitShift2--;
                                }
                            }
                        }
                        spritesOnLine++;
                        if (spritesOnLine === 5 && !this.fifthSprite)
                        {
                            this.fifthSprite = true;
                            this.fifthSpriteIndex = index;
                        }
                    }
                }
            }
            if (f18a.Mode === F18A.MODE_TEXT_80)
            {
                for (x1 = this.drawWidth >> 1; x1 >= 0; x1--)
                {
                    spriteColorBuffer[x1 << 1] = spriteColorBuffer[x1];
                    spritePaletteBaseIndexBuffer[x1 << 1] = spritePaletteBaseIndexBuffer[x1];
                    spriteColorBuffer[(x1 << 1) + 1] = spriteColorBuffer[x1];
                    spritePaletteBaseIndexBuffer[(x1 << 1) + 1] = spritePaletteBaseIndexBuffer[x1];
                }
            }
        }
*/
            var scrollWidth = this.drawWidth;
            var scrollHeight = this.drawHeight;
            // Border in text modes
            var borderWidth = f18a.Mode === F18A.MODE_TEXT ? 8 : (f18a.Mode === F18A.MODE_TEXT_80 ? 16 : 0);
            scrollWidth -= (borderWidth << 1);
            // Prepare values for Tile layer 1
            var nameTableCanonicalBase = this.vPageSize1 ? tms.ChrTab & 0x3000 : (this.hPageSize1 ? tms.ChrTab & 0x3800 : tms.ChrTab);
            var nameTableBaseAddr = tms.ChrTab;
            var y1 = y + this.vScroll1;
            if (y1 >= scrollHeight) {
                y1 -= scrollHeight;
                nameTableBaseAddr ^= this.vPageSize1;
            }
            var rowOffset;
            switch (f18a.Mode) {
                case F18A.MODE_GRAPHICS:
                case F18A.MODE_BITMAP:
                case F18A.MODE_MULTICOLOR:
                    rowOffset = (y1 >> 3) << 5;
                    break;
                case F18A.MODE_TEXT:
                    rowOffset = (y1 >> 3) * 40;
                    break;
                case F18A.MODE_TEXT_80:
                    rowOffset = (y1 >> 3) * 80;
                    break;
            }
            var lineOffset = y1 & 7;
            // Prepare values for Bitmap layer
            if (this.bitmapEnable) {
                var bitmapX2 = this.bitmapX + this.bitmapWidth;
                var bitmapY1 = y - this.bitmapY;
                var bitmapY2 = this.bitmapY + this.bitmapHeight;
                var bitmapYOffset = bitmapY1 * this.bitmapWidth;
            }
            // Prepare values for Tile layer 2
            var rowOffset2, nameTableBaseAddr2, lineOffset2;
            if (this.tileLayer2Enabled) {
                var nameTableCanonicalBase2 = this.vPageSize2 ? tms.ChrTab2 & 0x3000 : (this.hPageSize2 ? tms.ChrTab2 & 0x3800 : tms.ChrTab2);
                nameTableBaseAddr2 = tms.ChrTab2;
                var y12 = y + this.vScroll2;
                if (y12 >= scrollHeight) {
                    y12 -= scrollHeight;
                    nameTableBaseAddr2 ^= this.vPageSize2;
                }
                switch (f18a.Mode) {
                    case F18A.MODE_GRAPHICS:
                    case F18A.MODE_BITMAP:
                    case F18A.MODE_MULTICOLOR:
                        rowOffset2 = (y12 >> 3) << 5;
                        break;
                    case F18A.MODE_TEXT:
                        rowOffset2 = (y12 >> 3) * 40;
                        break;
                    case F18A.MODE_TEXT_80:
                        rowOffset2 = (y12 >> 3) * 80;
                        break;
                }
                lineOffset2 = y12 & 7;
            }
            // Draw line
            for (var xc = 0; xc < this.canvasWidth; xc++) {
                // Draw pixel
                var color = tms.BGColor;
                var paletteBaseIndex = 0;
                if (xc >= this.leftBorder && xc < this.leftBorder + this.drawWidth) {
                    var x = xc - this.leftBorder;
                    // Tile layer 1
                    if (this.tileLayer1Enabled) {
                        var nameTableAddr = nameTableBaseAddr;
                        var x1 = x - borderWidth + (this.hScroll1 << (f18a.Mode === F18A.MODE_TEXT_80 ? 1 : 0));
                        if (x1 >= scrollWidth) {
                            x1 -= scrollWidth;
                            nameTableAddr ^= this.hPageSize1;
                        }
                        var charNo, bitShift, bit, patternAddr, patternByte;
                        var tileColor, tilePriority, tileAttributeByte, transparentColor0;
                        var tilePaletteBaseIndex, lineOffset1;
                        switch (f18a.Mode) {
                            case F18A.MODE_GRAPHICS:
                                nameTableAddr += (x1 >> 3) + rowOffset;
                                charNo = this.ram[nameTableAddr];
                                bitShift = x1 & 7;
                                lineOffset1 = lineOffset;
                                if (this.tileColorMode !== F18A.COLOR_MODE_NORMAL) {
                                    tileAttributeByte = this.ram[tms.ColTab + (this.ecmPositionAttributes ? nameTableAddr - nameTableCanonicalBase : charNo)];
                                    tilePriority = (tileAttributeByte & 0x80) !== 0;
                                    if ((tileAttributeByte & 0x40) !== 0) {
                                        // Flip X
                                        bitShift = 7 - bitShift;
                                    }
                                    if ((tileAttributeByte & 0x20) !== 0) {
                                        // Flip y
                                        lineOffset1 = 7 - lineOffset1;
                                    }
                                    transparentColor0 = (tileAttributeByte & 0x10) !== 0;
                                }
                                bit = 0x80 >> bitShift;
                                patternAddr = tms.ChrGen + (charNo << 3) + lineOffset1;
                                patternByte = this.ram[patternAddr];
                                switch (this.tileColorMode) {
                                    case F18A.COLOR_MODE_NORMAL:
                                        var colorSet = this.ram[tms.ColTab + (charNo >> 3)];
                                        tileColor = (patternByte & bit) !== 0 ? (colorSet & 0xF0) >> 4 : colorSet & 0x0F;
                                        tilePaletteBaseIndex = this.tilePaletteSelect;
                                        transparentColor0 = true;
                                        tilePriority = false;
                                        break;
                                    case F18A.COLOR_MODE_ECM_1:
                                        tileColor = ((patternByte & bit) >> (7 - bitShift));
                                        tilePaletteBaseIndex = (this.tilePaletteSelect & 0x20) | ((tileAttributeByte & 0x0f) << 1);
                                        break;
                                    case F18A.COLOR_MODE_ECM_2:
                                        tileColor =
                                            ((patternByte & bit) >> (7 - bitShift)) |
                                            (((this.ram[patternAddr + this.tilePlaneOffset] & bit) >> (7 - bitShift)) << 1);
                                        tilePaletteBaseIndex = ((tileAttributeByte & 0x0f) << 2);
                                        break;
                                    case F18A.COLOR_MODE_ECM_3:
                                        tileColor =
                                            ((patternByte & bit) >> (7 - bitShift)) |
                                            (((this.ram[patternAddr + this.tilePlaneOffset] & bit) >> (7 - bitShift)) << 1) |
                                            (((this.ram[patternAddr + (this.tilePlaneOffset << 1)] & bit) >> (7 - bitShift)) << 2);
                                        tilePaletteBaseIndex = ((tileAttributeByte & 0x0e) << 2);
                                        break;
                                }
                                if (tileColor > 0 || !transparentColor0) {
                                    color = tileColor;
                                    paletteBaseIndex = tilePaletteBaseIndex;
                                }
                                break;
                            case F18A.MODE_BITMAP:
                                charNo = this.ram[nameTableAddr + (x1 >> 3) + rowOffset];
                                bitShift = x1 & 7;
                                bit = 0x80 >> bitShift;
                                var charSetOffset = (y & 0xC0) << 5;
                                patternByte = this.ram[tms.ChrGen + (((charNo << 3) + charSetOffset) & this.patternTableMask) + lineOffset];
                                var colorAddr = tms.ColTab + (((charNo << 3) + charSetOffset) & tms.ColTabMask) + lineOffset;
                                var colorByte = this.ram[colorAddr];
                                tileColor = (patternByte & bit) !== 0 ? (colorByte & 0xF0) >> 4 : (colorByte & 0x0F);
                                if (tileColor > 0) {
                                    color = tileColor;
                                    paletteBaseIndex = this.tilePaletteSelect;
                                }
                                break;
                            case F18A.MODE_TEXT:
                            case F18A.MODE_TEXT_80:
                                if (x >= borderWidth && x < this.drawWidth - borderWidth) {
                                    nameTableAddr += Math.floor(x1 / 6) + rowOffset;
                                    charNo = this.ram[nameTableAddr];
                                    bitShift = x1 % 6;
                                    lineOffset1 = lineOffset;
                                    if (this.tileColorMode !== F18A.COLOR_MODE_NORMAL) {
                                        tileAttributeByte = this.ram[tms.ColTab + (this.ecmPositionAttributes ? nameTableAddr - nameTableCanonicalBase : charNo)];
                                        tilePriority = (tileAttributeByte & 0x80) !== 0;
                                        if ((tileAttributeByte & 0x40) !== 0) {
                                            // Flip X
                                            bitShift = 5 - bitShift;
                                        }
                                        if ((tileAttributeByte & 0x20) !== 0) {
                                            // Flip y
                                            lineOffset1 = 7 - lineOffset1;
                                        }
                                        transparentColor0 = (tileAttributeByte & 0x10) !== 0;
                                    }
                                    bit = 0x80 >> bitShift;
                                    patternAddr = tms.ChrGen + (charNo << 3) + lineOffset1;
                                    patternByte = this.ram[patternAddr];
                                    switch (this.tileColorMode) {
                                        case F18A.COLOR_MODE_NORMAL:
                                            if (this.unlocked && this.ecmPositionAttributes) {
                                                tileAttributeByte = this.ram[tms.ColTab + nameTableAddr - nameTableCanonicalBase];
                                                tileColor = (patternByte & bit) !== 0 ? tileAttributeByte >> 4 : tileAttributeByte & 0xF;
                                            }
                                            else {
                                                tileColor = (patternByte & bit) !== 0 ? tms.FGColor : tms.BGColor;
                                            }
                                            tilePaletteBaseIndex = this.tilePaletteSelect;
                                            transparentColor0 = true;
                                            tilePriority = false;
                                            break;
                                        case F18A.COLOR_MODE_ECM_1:
                                            tileColor = ((patternByte & bit) >> (7 - bitShift));
                                            tilePaletteBaseIndex = (this.tilePaletteSelect & 0x20) | ((tileAttributeByte & 0x0f) << 1);
                                            break;
                                        case F18A.COLOR_MODE_ECM_2:
                                            tileColor =
                                                ((patternByte & bit) >> (7 - bitShift)) |
                                                (((this.ram[patternAddr + this.tilePlaneOffset] & bit) >> (7 - bitShift)) << 1);
                                            tilePaletteBaseIndex = ((tileAttributeByte & 0x0f) << 2);
                                            break;
                                        case F18A.COLOR_MODE_ECM_3:
                                            tileColor =
                                                ((patternByte & bit) >> (7 - bitShift)) |
                                                (((this.ram[patternAddr + this.tilePlaneOffset] & bit) >> (7 - bitShift)) << 1) |
                                                (((this.ram[patternAddr + (this.tilePlaneOffset << 1)] & bit) >> (7 - bitShift)) << 2);
                                            tilePaletteBaseIndex = ((tileAttributeByte & 0x0e) << 2);
                                            break;
                                    }
                                    if (tileColor > 0 || !transparentColor0) {
                                        color = tileColor;
                                        paletteBaseIndex = tilePaletteBaseIndex;
                                    }
                                }
                                else {
                                    color = tms.BGColor;
                                }
                                break;
                            case F18A.MODE_MULTICOLOR:
                                charNo = this.ram[nameTableAddr + (x1 >> 3) + rowOffset];
                                colorByte = this.ram[tms.ChrGen + (charNo << 3) + ((y1 & 0x1c) >> 2)];
                                tileColor = (x1 & 4) === 0 ? (colorByte & 0xf0) >> 4 : (colorByte & 0x0f);
                                if (tileColor > 0) {
                                    color = tileColor;
                                    paletteBaseIndex = this.tilePaletteSelect;
                                }
                                break;
                        }
                    }
                    // Bitmap layer
                    if (this.bitmapEnable) {
                        var bmpX = f18a.Mode !== F18A.MODE_TEXT_80 ? x : x >> 1;
                        if (bmpX >= this.bitmapX && bmpX < bitmapX2 && y >= this.bitmapY && y < bitmapY2) {
                            var bitmapX1 = x - this.bitmapX;
                            var bitmapPixelOffset = bitmapX1 + bitmapYOffset;
                            var bitmapByte = this.ram[this.bitmapBaseAddr + (bitmapPixelOffset >> 2)];
                            var bitmapBitShift, bitmapColor;
                            if (this.bitmapFat) {
                                // 16 color bitmap with fat pixels
                                bitmapBitShift = (2 - (bitmapPixelOffset & 2)) << 1;
                                bitmapColor = (bitmapByte >> bitmapBitShift) & 0x0F;
                            }
                            else {
                                // 4 color bitmap
                                bitmapBitShift = (3 - (bitmapPixelOffset & 3)) << 1;
                                bitmapColor = (bitmapByte >> bitmapBitShift) & 0x03;
                            }
                            if ((bitmapColor > 0 || !this.bitmapTransparent) && (color === tms.BGColor || this.bitmapPriority)) {
                                color = bitmapColor;
                                paletteBaseIndex = this.bitmapPaletteSelect;
                            }
                        }
                    }
                    // Sprite layer
                    var spriteColor = null;
                    if ((this.unlocked || (f18a.Mode !== F18A.MODE_TEXT && f18a.Mode !== F18A.MODE_TEXT_80)) && (!tilePriority || transparentColor0 && color === 0)) {
                        spriteColor = spriteColorBuffer[x] - 1;
                        if (spriteColor > 0) {
                            color = spriteColor;
                            paletteBaseIndex = spritePaletteBaseIndexBuffer[x];
                        }
                        else {
                            spriteColor = null;
                        }
                    }
                    // Tile layer 2
                    // The following is almost just a copy of the code from TL1, so this could be coded more elegantly
                    if (this.tileLayer2Enabled) {
                        var nameTableAddr2 = nameTableBaseAddr2;
                        var x12 = x - borderWidth + (this.hScroll2 << (f18a.Mode === F18A.MODE_TEXT_80 ? 1 : 0));
                        if (x12 >= scrollWidth) {
                            x12 -= scrollWidth;
                            nameTableAddr2 ^= this.hPageSize2;
                        }
                        var charNo2, bitShift2, bit2, patternAddr2, patternByte2;
                        var tileColor2, tilePriority2, tileAttributeByte2, transparentColor02;
                        var tilePaletteBaseIndex2, lineOffset12;
                        switch (f18a.Mode) {
                            case F18A.MODE_GRAPHICS:
                                nameTableAddr2 += (x12 >> 3) + rowOffset2;
                                charNo2 = this.ram[nameTableAddr2];
                                bitShift2 = x12 & 7;
                                lineOffset12 = lineOffset2;
                                if (this.tileColorMode !== F18A.COLOR_MODE_NORMAL) {
                                    tileAttributeByte2 = this.ram[tms.ColTab2 + (this.ecmPositionAttributes ? nameTableAddr2 - nameTableCanonicalBase2 : charNo2)];
                                    tilePriority2 = (tileAttributeByte2 & 0x80) !== 0;
                                    if ((tileAttributeByte2 & 0x40) !== 0) {
                                        // Flip X
                                        bitShift2 = 7 - bitShift2;
                                    }
                                    if ((tileAttributeByte2 & 0x20) !== 0) {
                                        // Flip y
                                        lineOffset12 = 7 - lineOffset12;
                                    }
                                    transparentColor02 = (tileAttributeByte2 & 0x10) !== 0;
                                }
                                bit2 = 0x80 >> bitShift2;
                                patternAddr2 = tms.ChrGen + (charNo2 << 3) + lineOffset12;
                                patternByte2 = this.ram[patternAddr2];
                                switch (this.tileColorMode) {
                                    case F18A.COLOR_MODE_NORMAL:
                                        var colorSet2 = this.ram[tms.ColTab2 + (charNo2 >> 3)];
                                        tileColor2 = (patternByte2 & bit2) !== 0 ? (colorSet2 & 0xF0) >> 4 : colorSet2 & 0x0F;
                                        tilePaletteBaseIndex2 = this.tilePaletteSelect2;
                                        transparentColor02 = true;
                                        tilePriority2 = false;
                                        break;
                                    case F18A.COLOR_MODE_ECM_1:
                                        tileColor2 = ((patternByte2 & bit2) >> (7 - bitShift2));
                                        tilePaletteBaseIndex2 = (this.tilePaletteSelect2 & 0x20) | ((tileAttributeByte2 & 0x0f) << 1);
                                        break;
                                    case F18A.COLOR_MODE_ECM_2:
                                        tileColor2 =
                                            ((patternByte2 & bit2) >> (7 - bitShift2)) |
                                            (((this.ram[patternAddr2 + this.tilePlaneOffset] & bit2) >> (7 - bitShift2)) << 1);
                                        tilePaletteBaseIndex2 = ((tileAttributeByte2 & 0x0f) << 2);
                                        break;
                                    case F18A.COLOR_MODE_ECM_3:
                                        tileColor2 =
                                            ((patternByte2 & bit2) >> (7 - bitShift2)) |
                                            (((this.ram[patternAddr2 + this.tilePlaneOffset] & bit2) >> (7 - bitShift2)) << 1) |
                                            (((this.ram[patternAddr2 + (this.tilePlaneOffset << 1)] & bit2) >> (7 - bitShift2)) << 2);
                                        tilePaletteBaseIndex2 = ((tileAttributeByte2 & 0x0e) << 2);
                                        break;
                                }
                                break;
                            case F18A.MODE_BITMAP:
                                charNo2 = this.ram[nameTableAddr2 + (x12 >> 3) + rowOffset2];
                                bitShift2 = x12 & 7;
                                bit2 = 0x80 >> bitShift2;
                                var charSetOffset2 = (y & 0xC0) << 5;
                                patternByte2 = this.ram[tms.ChrGen + (((charNo2 << 3) + charSetOffset2) & this.patternTableMask) + lineOffset2];
                                var colorAddr2 = tms.ColTab2 + (((charNo2 << 3) + charSetOffset2) & tms.ColTabMask) + lineOffset2;
                                var colorByte2 = this.ram[colorAddr2];
                                tileColor2 = (patternByte2 & bit2) !== 0 ? (colorByte2 & 0xF0) >> 4 : (colorByte2 & 0x0F);
                                tilePaletteBaseIndex2 = this.tilePaletteSelect2;
                                transparentColor02 = true;
                                tilePriority2 = false;
                                break;
                            case F18A.MODE_TEXT:
                            case F18A.MODE_TEXT_80:
                                if (x >= borderWidth && x < this.drawWidth - borderWidth) {
                                    nameTableAddr2 += Math.floor(x12 / 6) + rowOffset2;
                                    charNo2 = this.ram[nameTableAddr2];
                                    bitShift2 = x12 % 6;
                                    lineOffset12 = lineOffset2;
                                    if (this.tileColorMode !== F18A.COLOR_MODE_NORMAL) {
                                        tileAttributeByte2 = this.ram[tms.ColTab2 + (this.ecmPositionAttributes ? nameTableAddr2 - nameTableCanonicalBase2 : charNo2)];
                                        tilePriority2 = (tileAttributeByte2 & 0x80) !== 0;
                                        if ((tileAttributeByte2 & 0x40) !== 0) {
                                            // Flip X
                                            bitShift2 = 5 - bitShift2;
                                        }
                                        if ((tileAttributeByte2 & 0x20) !== 0) {
                                            // Flip y
                                            lineOffset12 = 7 - lineOffset12;
                                        }
                                        transparentColor02 = (tileAttributeByte2 & 0x10) !== 0;
                                    }
                                    bit2 = 0x80 >> bitShift2;
                                    patternAddr2 = tms.ChrGen + (charNo2 << 3) + lineOffset12;
                                    patternByte2 = this.ram[patternAddr2];
                                    switch (this.tileColorMode) {
                                        case F18A.COLOR_MODE_NORMAL:
                                            if (this.unlocked && this.ecmPositionAttributes) {
                                                tileAttributeByte2 = this.ram[tms.ColTab2 + nameTableAddr2 - nameTableCanonicalBase2];
                                                tileColor2 = (patternByte2 & bit2) !== 0 ? tileAttributeByte2 >> 4 : tileAttributeByte2 & 0xF;
                                            }
                                            else {
                                                tileColor2 = (patternByte2 & bit2) !== 0 ? tms.FGColor : tms.BGColor;
                                            }
                                            tilePaletteBaseIndex2 = this.tilePaletteSelect2;
                                            transparentColor02 = true;
                                            tilePriority2 = false;
                                            break;
                                        case F18A.COLOR_MODE_ECM_1:
                                            tileColor2 = ((patternByte2 & bit2) >> (7 - bitShift2));
                                            tilePaletteBaseIndex2 = (this.tilePaletteSelect2 & 0x20) | ((tileAttributeByte2 & 0x0f) << 1);
                                            break;
                                        case F18A.COLOR_MODE_ECM_2:
                                            tileColor2 =
                                                ((patternByte2 & bit2) >> (7 - bitShift2)) |
                                                (((this.ram[patternAddr2 + this.tilePlaneOffset] & bit2) >> (7 - bitShift2)) << 1);
                                            tilePaletteBaseIndex2 = ((tileAttributeByte2 & 0x0f) << 2);
                                            break;
                                        case F18A.COLOR_MODE_ECM_3:
                                            tileColor2 =
                                                ((patternByte2 & bit2) >> (7 - bitShift2)) |
                                                (((this.ram[patternAddr2 + this.tilePlaneOffset] & bit2) >> (7 - bitShift2)) << 1) |
                                                (((this.ram[patternAddr2 + (this.tilePlaneOffset << 1)] & bit2) >> (7 - bitShift2)) << 2);
                                            tilePaletteBaseIndex2 = ((tileAttributeByte2 & 0x0e) << 2);
                                            break;
                                    }
                                }
                                else {
                                    tileColor2 = 0;
                                    transparentColor02 = true;
                                    tilePriority2 = false;
                                }
                                break;
                            case F18A.MODE_MULTICOLOR:
                                charNo2 = this.ram[nameTableAddr2 + (x12 >> 3) + rowOffset2];
                                colorByte2 = this.ram[tms.ChrGen + (charNo2 << 3) + ((y12 & 0x1c) >> 2)];
                                tileColor2 = (x12 & 4) === 0 ? (colorByte2 & 0xf0) >> 4 : (colorByte2 & 0x0f);
                                tilePaletteBaseIndex2 = this.tilePaletteSelect2;
                                transparentColor02 = true;
                                tilePriority2 = false;
                                break;
                        }
                        if ((tileColor2 > 0 || !transparentColor02) && (this.tileMap2AlwaysOnTop || tilePriority2 || spriteColor === null)) {
                            color = tileColor2;
                            paletteBaseIndex = tilePaletteBaseIndex2;
                        }
                    }
                }
                // Draw pixel
                var rgbColor = this.palette[color + paletteBaseIndex];
                imagedata[imagedataAddr++] = rgbColor[0];
                imagedata[imagedataAddr++] = rgbColor[1];
                imagedata[imagedataAddr++] = rgbColor[2];
                imagedataAddr++;
            }
        }
        else {
            // Empty scanline
            rgbColor = this.palette[tms.BGColor];
            for (xc = 0; xc < this.canvasWidth; xc++) {
                imagedata[imagedataAddr++] = rgbColor[0]; // R
                imagedata[imagedataAddr++] = rgbColor[1]; // G
                imagedata[imagedataAddr++] = rgbColor[2]; // B
                imagedataAddr++; // Skip alpha
            }
        }
        if (this.scanLines && (y & 1) !== 0) {
            // Dim last scan line
            var imagedataAddr2 = imagedataAddr - (this.canvasWidth << 2);
            for (xc = 0; xc < this.canvasWidth; xc++) {
                imagedata[imagedataAddr2++] *= 0.75;
                imagedata[imagedataAddr2++] *= 0.75;
                imagedata[imagedataAddr2++] *= 0.75;
                imagedataAddr2++;
            }
        }
        this.imagedataAddr = imagedataAddr;
    }
#endif
};



