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
 *
 */
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "f18a.h"

tF18A f18a;

//---------------------------------------------------------------------------
const unsigned char F18A_palette[4*16*3] = {
    // Palette 0, original 9918A NTSC color approximations
/*"000", //  0 Transparent
"000", //  1 Black
"2C3", //  2 Medium Green
"5D6", //  3 Light Green
"54F", //  4 Dark Blue
"76F", //  5 Light Blue
"D54", //  6 Dark Red
"4EF", //  7 Cyan
"F54", //  8 Medium Red
"F76", //  9 Light Red
"DC3", // 10 Dark Yellow
"ED6", // 11 Light Yellow
"2B2", // 12 Dark Green
"C5C", // 13 Magenta
"CCC", // 14 Gray
"FFF", // 15 White*/
    // Palette 1, ECM1 (0 index is always 000) version of palette 0
/*"000", //  0 Black
"2C3", //  1 Medium Green
"000", //  2 Black
"54F", //  3 Dark Blue
"000", //  4 Black
"D54", //  5 Dark Red
"000", //  6 Black
"4EF", //  7 Cyan
"000", //  8 Black
"CCC", //  9 Gray
"000", // 10 Black
"DC3", // 11 Dark Yellow
"000", // 12 Black
"C5C", // 13 Magenta
"000", // 14 Black
"FFF", // 15 White*/
    // Palette 2, CGA colors
    0,  0,   0, //  black
    0,   0, 170, //  blue
    0, 170,   0, //  green
    0, 170, 170, //  cyan
    170,   0,   0, //  red
    170,   0, 170, //  magenta
    170,  85,   0, //  brown
    170, 170, 170, //  light gray
    85,  85,  85, //  gray
    85,  85, 255, //  light blue
    85, 255,  85, //  light green
    85, 255, 255, //  light cyan
    255,  85,  85, //  light red
    255,  85, 255, //  light magenta
    255, 255,  85, //  yellow
    255, 255, 255, //  white

    // Palette 3, ECM1 (0 index is always 000) version of palette 2
    0,   0,   0, //  black
    85,  85,  85, //  gray
    0,   0,   0, //  black
    0,   0, 170, //  blue
    0,   0,   0, //  black
    0, 170,   0, //  green
    0,   0,   0, //  black
    0, 170, 170, //  cyan
    0,   0,   0, //  black
    170,   0,   0, //  red
    0,   0,   0, //  black
    170,   0, 170, //  magenta
    0,   0,   0, //  black
    170,  85,   0, //  brown
    0,   0,   0, //  black
    255, 255, 255, //  white
};

#pragma package(smart_init)
// ----------------------------------------------------------------------------------------

// Reset the tms.VR
void f18a_reset(void) {
    // 1st, reset tms 9918
    tms9918_reset();

    /*
        var i;
        for (i = 0; i < 0x4000; i++) {
            this.ram[i] = 0;
        }
        for (i = 0; i < this.registers.length; i++) {
            this.registers[i] = 0;
        }
        this.addressRegister = 0;
        this.statusRegister = 0;
        this.palette = [];
        for (i = 0; i < 64; i++) {
            var rgbColor = F18A.PALETTE[i];
            this.palette[i] = [
                parseInt(rgbColor.charAt(0), 16) * 17,
                parseInt(rgbColor.charAt(1), 16) * 17,
                parseInt(rgbColor.charAt(2), 16) * 17
            ];
        }

        this.prefetchByte = 0;
        this.latch = false;
*/
        f18a.addressIncrement = 1;
        f18a.unlocked = 0;
        f18a.statusRegisterNo = 0;
        f18a.dataPortMode = 0;
        f18a.autoIncPaletteReg = 0;
        f18a.paletteRegisterNo = 0;
        f18a.paletteRegisterData = 255;
        f18a.gpuAddressLatch = 0;
/*
        f18a.currentScanline = 0;
        f18a.fakeScanline = null;
        f18a.blanking = 0;
        this.displayOn = true;
        this.interruptsOn = false;
		this.screenMode = F18A.MODE_GRAPHICS;
        this.colorTable = 0;
        this.nameTable = 0;
        this.charPatternTable = 0;
        this.spriteAttributeTable = 0;
        this.spritePatternTable = 0;
        this.colorTableMask = 0x3FFF;
        this.patternTableMask = 0x3FFF;
        this.fgColor = 0;
        this.bgColor = 7;
		this.spriteSize = 0;
		this.spriteMag = 0;

        this.tileColorMode = 0;
        this.tilePaletteSelect = 0;
        this.tilePaletteSelect2 = 0;
        this.spriteColorMode = 0;
        this.spritePaletteSelect = 0;
        this.realSpriteYCoord = 0;
        this.colorTable2 = 0;
        this.nameTable2 = 0;
        this.tileLayer1Enabled = true;
        this.tileLayer2Enabled = false;
        this.row30Enabled = false;
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
    f18a_resetregs();
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

// Writing a data to a F18A register
void WriteF18A(int iReg,unsigned char value) {
    unsigned char oldrunsr,runsr,oldvalue,gromclock;

    // store value
    oldvalue = f18a.VDPR[iReg];
    f18a.VDPR[iReg] = value;

	// Depending on the register, do...
    switch (iReg)
    {
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
        f18a.dataPortMode=(f18a.VDPR[0x2F] & 0x80)>>7;
        f18a.autoIncPaletteReg=(f18a.VDPR[0x2F] & 0x70)>>6;
        f18a.paletteRegisterNo=(f18a.VDPR[0x2F] & 0x3F);
        f18a.paletteRegisterData=255;
        break;
    case 0x30: // SIGNED two's-complement increment amount for VRAM address, defaults to 1
        f18a.addressIncrement = f18a.VDPR[0x30] < 128 ? f18a.VDPR[0x30] : f18a.VDPR[0x30] - 256;
        break;
    case 0x31: // Enhanced color mode
        f18a.tileLayer2Enabled = (f18a.VDPR[0x31] & 0x80) != 0;
        oldvalue = f18a.row30Enabled;
        f18a.row30Enabled = (f18a.VDPR[0x31] & 0x40) != 0;
        if (oldvalue != f18a.row30Enabled)
        {
            //this.setDimensions();
        }
        f18a.tileColorMode = (f18a.VDPR[0x31] & 0x30) >> 4;
        f18a.realSpriteYCoord = (f18a.VDPR[0x31] & 0x08) != 0;
        f18a.spriteLinkingEnabled = (f18a.VDPR[0x31] & 0x04) != 0;
        f18a.spriteColorMode = f18a.VDPR[0x31] & 0x03;
        break;
    case 0x32: // Position vs name attributes, TL2 always on top
        // Write 1 to reset all VDP registers
        if ((f18a.VDPR[0x32] & 0x80) != 0) {
            f18a_resetregs();
            f18a.unlocked = 0;
            //this.updateMode(this.registers[0], this.registers[1]);
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
        f18a.gpuAddressLatch = 0;
        break;
    case 0x37: // GPU address LSB
        if (f18a.gpuAddressLatch)
        {
            f18a.gpuAddressLatch = 0;
            //this.gpu.intReset();
            //this.gpu.setPC(this.registers[54] << 8 | this.registers[55]);
        }
        break;
    case 0x38:
        if ((f18a.VDPR[0x38] & 1) != 0)
        {
            //this.gpu.setIdle(false);
        }
        else
        {
            //this.gpu.setIdle(true);
        }
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
        //this.updateMode(this.registers[0], this.registers[1]);
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
    if (!f18a.dataPortMode) {
        tms9918_writedata(value);
    }
    else
    {
        // Write data to F18A palette registers
        if (f18a.paletteRegisterData == 255)
        {
            // Read first byte
            f18a.paletteRegisterData = value;
        }
        else
        {
        /*
            // Read second byte
            f18a.palette[f18a.paletteRegisterNo][0] = this.paletteRegisterData * 17;
            f18a.palette[f18a.paletteRegisterNo][1] = ((value & 0xf0) >> 4) * 17;
            f18a.palette[f18a.paletteRegisterNo][2] = (value & 0x0f) * 17;
            if (f18a.autoIncPaletteReg)
            {
                f18a.paletteRegisterNo++;
            }
            // The F18A turns off DPM after each register is written if auto increment is off
            // or after writing to last register if auto increment in on
            if (!f18a.autoIncPaletteReg || f18a.paletteRegisterNo == 64) {
                f18a.dataPortMode = false;
                f18a.paletteRegisterNo = 0;
            }
            f18a.paletteRegisterData = -1;
*/
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
};
// ----------------------------------------------------------------------------------------

unsigned char f18a_readctrl(void)
{
};
// ----------------------------------------------------------------------------------------


