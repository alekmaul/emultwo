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
 * f18a.h
 *
 * Based on js99'er - TI-99/4A emulator written in JavaScript
 *  written by Rasmus Moustgaard <rasmus.moustgaard@gmail.com>
 *
 */
//---------------------------------------------------------------------------

#ifndef f18aH
#define f18aH

#define F18A_VERSION                        0x18

#define F18A_MAX_SCANLINE_SPRITES_JUMPER    1
#define F18A_SCANLINES_JUMPER               0

#define F18A_MODE_GRAPHICS                  0
#define F18A_MODE_TEXT                      1
#define F18A_MODE_TEXT_80                   2
#define F18A_MODE_BITMAP                    3
#define F18A_MODE_MULTICOLOR                4

#define MAXF18ASCREEN                       4   // maximum mode supported

#define F18A_COLOR_MODE_NORMAL              0
#define F18A_COLOR_MODE_ECM_1               1
#define F18A_COLOR_MODE_ECM_2               2
#define F18A_COLOR_MODE_ECM_3               3

#define F18A_REG1_SCREEN                    0x40 // 1: Enable display
#define F18A_REG1_IRQ                       0x20 // 1: IRQs on VBlanks
#define F18A_REG1_SPR16                     0x02 // 1: 16x16 sprites (0=8x8)
#define F18A_REG1_BIGSPR                    0x01 // 1: Magnify sprites x2

#define F18A_STAT_VBLANK                    0x80 // 1: VBlank has occured

#define F18A_DisplayON                      (f18a.VDPR[1]&F18A_REG1_SCREEN)
#define F18A_InterruptON                    (f18a.VDPR[1]&F18A_REG1_IRQ)
#define F18A_SpriteSize                     ((f18a.VR[1]&F18A_REG1_SPR16)>>1)
#define F18A_SpriteMag                      (f18a.VR[1]&F18A_REG1_BIGSPR)

#include "tms9928a.h"

//---------------------------------------------------------------------------
typedef struct {
  void (*Refresh)(unsigned char uY);
} tScrModeF18A;


typedef struct {
    BYTE Version;                                      // Current code version

    BYTE VDPR[64];                                     // VDP registers (only 58 used)

    BYTE Mode;                                         // Current screen mode
    BYTE unlocked;                                     // Status locked(0)/unlocked (1)
    BYTE Row30;                                        // Default height (24 or 30 8 pix tiles)

    //BYTE FGColor,BGColor;                              // Colors ForeGround and BackGround

    //unsigned short DLatch;                             // Data register latch
    //BYTE VKey;                                         // VDP address latch key

    BYTE DPM;                                          // Data Port Mode
    BYTE PalAutoInc;                                   // AutoInc palette register
    BYTE PalRegNo;                                     // Palette register number 0..63
    BYTE PalRegVal;                                    // Palette register value
    BYTE PalRecalc;                                    // 1 if we need to recalc palette
    //BYTE SR;                                           // Status register number 0..15

    //unsigned short ChrGen,ChrTab,ColTab;               // VDP tables (screens)
    //unsigned short SprGen,SprTab;                      // VDP tables (sprites)
    unsigned char *ChrTab2;                            // VDP Name Table 2 Base Address, 1K boundaries. 768-bytes per table for 24 rows, 960-bytes per table for 30 rows
    unsigned char *ColTab2;                            // VDP Color Table 2 Base Address, 64-byte boundaries. Works the same as VR3 in Enhanced Color Modes / Position-Attribute Mode
    //unsigned short ColTabM,ChrGenM;                    // VDP tables mask

    signed char VAddrInc;                              // SIGNED two's-complement increment amount for VRAM address, defaults to 1
    //unsigned short VAddr;                              // Storage for VIDRAM addresses

    BYTE IdxPal[16];                                    // Palette color index

    //unsigned short CurLine;                             // Current scanline
    //unsigned short ScanLines;                           // Scanlines per frame
    //BYTE UCount;                                        // Screen update counter



    unsigned int counterElapsed,counterStart,counterSnap;       // Counter management


    unsigned char interruptScanline;                            // Horizontal Interrupt scan line, 0 to disable, VR0 IE1-bit must = 1
    unsigned char maxScanlineSprites;                           // Max sprites per scan line, set to 0 to reset sprite max to jumper setting

    unsigned char spritePaletteSelect;                          // Extra palette-select bits for original color modes. Sprite, Tile-2, and Tile-1 layers
    unsigned char tilePaletteSelect;
    unsigned char tilePaletteSelect2;

    unsigned char hScroll2,vScroll2;                            // Tile-2 horizontal & vertical scroll offset
    unsigned char hScroll1,vScroll1;                            // Tile-1 horizontal & vertical scroll offset

    unsigned short hPageSize1,vPageSize1;                       // HPSIZE = horizontal page size, 0 = 1 page, 1 = 2 pages
    unsigned short hPageSize2,vPageSize2;                       // VPSIZE = vertical page size, 0 = 1 page, 1 = 2 pages
    unsigned short spritePlaneOffset;                           // SPGS = sprite pattern generator offset size, 11=256, 10=512, 01=1K, 00=2K
    unsigned short tilePlaneOffset;                             // TPGS = tile pattern generator offset size, 11=256, 10=512, 01=1K, 00=2K

    unsigned char bitmapEnable;                                 // BML: Enable, Priority over sprites, Transparent, Fat pixels, Palette select
    unsigned char bitmapPriority;
    unsigned char bitmapTransparent;
    unsigned char bitmapFat;
    unsigned char bitmapPaletteSelect;

    unsigned short bitmapBaseAddr;                              // Bitmap Layer Base Address, 64-byte boundaries
    unsigned short bitmapX,bitmapY;                             // Bitmap x,y
    unsigned short bitmapWidth,bitmapHeight;                    // Bitmap width & height


    unsigned char tileLayer2Enabled;                            // ECM = enhanced color mode, (T)ile and (S)prite
    unsigned char tileColorMode;
    unsigned char realSpriteYCoord;
    unsigned char spriteLinkingEnabled;
    unsigned char spriteColorMode;

    unsigned char gpuHsyncTrigger;
    unsigned char gpuVsyncTrigger;
    unsigned char tileLayer1Enabled;
    unsigned char reportMax;
    unsigned char ecmPositionAttributes;
    unsigned char tileMap2AlwaysOnTop;

    unsigned char maxSprites;                                   // Stop Sprite to limit the total number of sprites to process, defaults to 32
    unsigned char gpuAddressLatch;

} tF18A;

extern tF18A f18a;

//---------------------------------------------------------------------------
extern void f18a_updatemasks(void);
extern void f18a_updatemode(BYTE reg0,BYTE reg1);
extern void f18a_setwindowsize(void);

extern unsigned char WriteF18A(int iReg,unsigned char value);

extern void f18a_reset(void);
extern void f18a_resetregs(void);
extern void f18a_writedata(unsigned char value);
extern unsigned char f18a_readdata(void);
extern unsigned char f18a_writectrl(unsigned char value);
extern unsigned char f18a_readctrl(void);

extern void _F18A_modegm1(unsigned char uY);
extern void _F18A_modet(unsigned char uY);
extern void _F18A_modet80(unsigned char uY);
extern void _F18A_modetgm2(unsigned char uY);
extern void _F18A_modem(unsigned char uY);

extern unsigned char f18a_loop(void);

#endif
