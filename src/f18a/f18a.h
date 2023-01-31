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

#define F18A_COLOR_MODE_NORMAL              0
#define F18A_COLOR_MODE_ECM_1               1
#define F18A_COLOR_MODE_ECM_2               2
#define F18A_COLOR_MODE_ECM_3               3

#include "tms9928a.h"

//---------------------------------------------------------------------------
typedef struct {
    unsigned char Version;                                      // Current code version

    unsigned char unlocked;                                     // Status locked(0)/unlocked (1)

    unsigned char dataPortMode;                                 // Data port mode
    unsigned char autoIncPaletteReg;                            // AutoInc palette register
    unsigned char paletteRegisterNo;                            // Palette resiter number 0..63
    unsigned char paletteRegisterData;
    unsigned char statusRegisterNo;                             // Status register number 0..15

    unsigned int counterElapsed,counterStart,counterSnap;       // Counter management

    unsigned short nameTable2;                                  // Name Table 2 Base Address, 1K boundaries. 768-bytes per table for 24 rows, 960-bytes per table for 30 rows
    unsigned short colorTable2;                                 // Color Table 2 Base Address, 64-byte boundaries. Works the same as VR3 in Enhanced Color Modes / Position-Attribute Mode

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

    signed char addressIncrement;                               // SIGNED two's-complement increment amount for VRAM address, defaults to 1

    unsigned char tileLayer2Enabled;                            // ECM = enhanced color mode, (T)ile and (S)prite
    unsigned char row30Enabled;
    unsigned char tileColorMode;
    unsigned char realSpriteYCoord;
    unsigned char spriteLinkingEnabled;
    unsigned char spriteColorMode;

    unsigned char gpuHsyncTrigger;
    unsigned char gpuVsyncTrigger;
    unsigned char tileLayer1Enabled;
    unsigned char reportMax;
    unsigned char scanLines;
    unsigned char ecmPositionAttributes;
    unsigned char tileMap2AlwaysOnTop;

    unsigned char maxSprites;                                   // Stop Sprite to limit the total number of sprites to process, defaults to 32
    unsigned char gpuAddressLatch;

    unsigned short palette[64];                                 // 64 colors palette registers
    unsigned char SR[16];                                       // Status Regiters
    unsigned char VDPR[64];                                     // VDP registers (only 58 used)
} tF18A;

extern tF18A f18a;

//---------------------------------------------------------------------------

extern void WriteF18A(int iReg,unsigned char value);

extern void f18a_reset(void);
extern void f18a_resetregs(void);
extern void f18a_writedata(unsigned char value);
extern unsigned char f18a_readdata(void);
extern unsigned char f18a_writectrl(unsigned char value);
extern unsigned char f18a_readctrl(void);

#endif
