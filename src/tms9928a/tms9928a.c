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
 * TMS9928A.c
 *
 * Based on   tms emulation (C) Marat Fayzullin 1994-2021
 *
 */


//---------------------------------------------------------------------------
#include <mem.h>

#include "tms9928a.h"

#include "z80.h"

void _TMS9928A_mode0(unsigned char uY);
void _TMS9928A_mode1(unsigned char uY);
void _TMS9928A_mode2(unsigned char uY);
void _TMS9928A_mode3(unsigned char uY);

// Screen handlers and masks for tms.VR table address registers
const tScrMode SCR[MAXSCREEN+1] = {
    { _TMS9928A_mode0,0x7F,0x00,0x3F,0x00,0x3F, 0x00,0x00,0x00,0x00 },   // SCREEN 0:TEXT 40x24
    { _TMS9928A_mode1,0x7F,0xFF,0x3F,0xFF,0x3F, 0x00,0x00,0x00,0x00 },   // SCREEN 1:TEXT 32x24
    { _TMS9928A_mode2,0x7F,0x80,0x3C,0xFF,0x3F, 0x00,0x7F,0x03,0x00 },   // SCREEN 2:BLOCK 256x192
    { _TMS9928A_mode3,0x7F,0x00,0x3F,0xFF,0x3F, 0x00,0x00,0x00,0x00 }    // SCREEN 3:GFX 64x48x16
};

tTMS9981A tms;

// ----------------------------------------------------------------------------------------

// Reset the tms.VR
void tms9918_reset(void) {
	unsigned char i;

	memset(tms.VR,0x00,sizeof(tms.VR));                     // Init tms.VR Registers
	memset(tms.ram,0x00, 0x4000);                           // Init tms.VR VRAM

	tms.VKey=1;                                             // tms.VR address latch key
	tms.SR=0x00;                                            // tms.VR status register
	tms.FGColor=tms.BGColor=0;                              // Fore/Background color
	tms.Mode=0;                                             // Current screenmode
	tms.CurLine=0;                                          // Current scanline
    tms.DLatch = 0;                                         // Current Data latch
    tms.ScanLines = TMS9918_LINES;                          // Default for NTSC

    tms.MaxSprites = TMS9918_MAXSPRITES;                    // Default for chipset
	tms.ChrTab=tms.ColTab=tms.ChrGen=tms.ram;               // tms.VR tables (screen)
	tms.SprTab=tms.SprGen=tms.ram;                          // tms.VR tables (sprites)

	tms.ColTabM = ~0;                                       // tms mask tables
	tms.ChrGenM = ~0;

	tms.UCount = 0;
	tms.VAddr = 0x0000;
	for (i=0;i<16;i++) tms.IdxPal[i] = i;

    // Init screen size
    TVW=TVW_TMS;
    TVH=TVH_TMS;
}

// ----------------------------------------------------------------------------------------
// Writing a data to a tms.VR register
unsigned char Write9918(int iReg,unsigned char value) {
	unsigned char J;
	unsigned int VRAMMask;
	unsigned char bIRQ;

	// Enabling IRQs may cause an IRQ here
	bIRQ  = (iReg==1) && ((tms.VR[1]^value)&value&TMS9918_REG1_IRQ) && (tms.SR&TMS9918_STAT_VBLANK) ? 1 : 0;

	// VRAM can either be 4kB or 16kB
	VRAMMask = (iReg==1) && ( (tms.VR[1]^value) & TMS9918_REG1_RAM16K ) ? 0 : TMS9918_VRAMMask;

	// Store value into the register
	tms.VR[iReg]=value;

	// Depending on the register, do...
    switch (iReg)
    {
	case 0: // Mode register 0
	case 1: // Mode register 1
	    // Figure out new screen mode number
		switch(TMS9918_Mode)
        {
		case 0x00: J=1;break;
		case 0x01: J=2;break;
        case 0x02: J=3;break; // ALEK20140906 0 before (breaks homebrewn ColecoVision games if 0 / 3
		case 0x04: J=0;break; // ALEK20140906 3  : Bankrupcy Builder, Search For Crown Jewels, Waterville Rescue)
		default:   J=tms.Mode;break;
        }

        // If mode was changed, recompute table addresses
		if((J!=tms.Mode)||!VRAMMask)
        {
		    VRAMMask    = TMS9918_VRAMMask;
		    tms.ChrTab=tms.ram+(((int)(tms.VR[2]&SCR[J].R2)<<10)&VRAMMask);
			tms.ColTab=tms.ram+(((int)(tms.VR[3]&SCR[J].R3)<<6)&VRAMMask);
			tms.ChrGen=tms.ram+(((int)(tms.VR[4]&SCR[J].R4)<<11)&VRAMMask);
			tms.SprTab=tms.ram+(((int)(tms.VR[5]&SCR[J].R5)<<7)&VRAMMask);
			tms.SprGen=tms.ram+(((int)(tms.VR[6]&SCR[J].R6)<<11)&VRAMMask);
            tms.ColTabM = ((int)(tms.VR[3]|~SCR[J].M3)<<6)|0x1C03F;
            tms.ChrGenM = ((int)(tms.VR[4]|~SCR[J].M4)<<11)|0x007FF;
			tms.Mode=J;
        }
        /*if ((iReg==1) && (tms.SR&TMS9918_STAT_VBLANK) )
        {
            if (value&TMS9918_REG1_IRQ)
            {
                z80_set_irq_line(INPUT_LINE_NMI, ASSERT_LINE);
            }
            else
            {
                z80_set_irq_line(INPUT_LINE_NMI, CLEAR_LINE);
            }
        }*/
        break;
    case  2: // Name Table
	    tms.ChrTab=tms.ram+(((int)(value&SCR[tms.Mode].R2)<<10)&VRAMMask);
		break;
    case  3: // Color Table
	    tms.ColTab=tms.ram+(((int)(value&SCR[tms.Mode].R3)<<6)&VRAMMask);
	    tms.ColTabM = ((int)(value|~SCR[tms.Mode].M3)<<6)|0x1C03F;
		break;
    case  4: // Pattern Table
        tms.ChrGen=tms.ram+(((int)(value&SCR[tms.Mode].R4)<<11)&VRAMMask);
        tms.ChrGenM = ((int)(value|~SCR[tms.Mode].M4)<<11)|0x007FF;
        break;
    case  5: // Sprite Attributes
	    tms.SprTab=tms.ram+(((int)(value&SCR[tms.Mode].R5)<<7)&VRAMMask);
        break;
    case  6: // Sprite Patterns
        tms.SprGen=tms.ram+(((int)(value&SCR[tms.Mode].R6)<<11)&VRAMMask);
		break;
    case  7:  // Foreground and background colors
	    tms.FGColor=tms.IdxPal[value>>4];
		value &= 0x0F;
		tms.IdxPal[0] = tms.IdxPal[ value ? value : 1];
		tms.BGColor=tms.IdxPal[value];
		break;
	}

	// Return IRQ, if generated
	return(bIRQ);
}

// ----------------------------------------------------------------------------------------
// Write a value to the tms.VR Data Port.
void tms9918_writedata(unsigned char value) {
    tms.DLatch = value;
    tms.ram[tms.VAddr] = value;
    tms.VAddr = (tms.VAddr+1)&0x3FFF;
}

// ----------------------------------------------------------------------------------------
// Read a value from the tms.VR Data Port
unsigned char tms9918_readdata(void) {
    unsigned char retval;

    retval = tms.DLatch;
    tms.DLatch = tms.ram[tms.VAddr];
	tms.VAddr = (tms.VAddr+1)&0x3FFF;

	return(retval);
}

// ----------------------------------------------------------------------------------------
// Write a value to the tms.VR Control Port. Enabling IRQs or not
unsigned char tms9918_writectrl(unsigned char value) {
    if (tms.VKey)
    {
	    tms.VKey=0;
        tms.VAddr=((tms.VAddr&0xFF00)|value)&0x3FFF;
	}
	else
        {
		tms.VKey=1;
        tms.VAddr = ((tms.VAddr&0x00FF)|((int)value<<8))&0x3FFF;
		switch(value&0xC0)
        {
        case 0x00:
            // In READ mode, read the first unsigned char from VRAM
            tms.DLatch = tms.ram[tms.VAddr];
            tms.VAddr  = (tms.VAddr+1)&0x3FFF;
			break;
        case 0x80:
            // Enabling IRQs may cause an IRQ here
            return( Write9918(value&0x0F,tms.VAddr&0x00FF) );
		}
	}

	// No interrupts
	return(0);
}

// ----------------------------------------------------------------------------------------
// Read a value from the tms.VR Control Port
unsigned char tms9918_readctrl(void) {
    unsigned char retval;

    retval = tms.SR;
    tms.SR &= TMS9918_STAT_5THNUM|TMS9918_STAT_5THSPR;

    z80_set_irq_line(INPUT_LINE_NMI, CLEAR_LINE);

    return(retval);
}

// ----------------------------------------------------------------------------------------
// Compute bitmask of sprites shown in a given scanline.
// Returns the first sprite to show or -1 if none shown.
//  Also updates 5th sprite fields in the status register.
int ScanSprites(unsigned char Y,unsigned int *Mask)
{
    const unsigned char SprHeights[4] = { 8,16,16,32 };
    unsigned char OH,IH,*AT;
    int L,K,C1,C2;
    unsigned int M;

    // No 5th sprite yet
    tms.SR &= ~(TMS9918_STAT_5THNUM|TMS9918_STAT_5THSPR);

    // Must have MODE1+ and screen enabled
    if (!tms.Mode || !TMS9918_ScreenON)
    {
        if(Mask) *Mask=0;
        return(-1);
    }

    OH = SprHeights[tms.VR[1]&0x03];
    IH = SprHeights[tms.VR[1]&0x02];
    AT = tms.SprTab;
    C1 = tms.MaxSprites+1;
    C2 = 5;
    M  = 0;

    for (L=0;L<32;++L,AT+=4)
    {
        K=AT[0];             // K = sprite Y coordinate
        if (K==208) break;    // Iteration terminates if Y=208
        if (K>256-IH) K-=256; //Y coordinate may be negative */

        // Mark all valid sprites with 1s, break at MaxSprites
        if((Y>K)&&(Y<=K+OH))
        {
            // If we exceed four sprites per line, set 5th sprite flag
            if(!--C2) tms.SR|=TMS9918_STAT_5THSPR|L;

            // If we exceed maximum number of sprites per line, stop here
            if (!--C1) break;

            // Mark sprite as ready to draw
            M|=(1<<L);
        }
    }

    // Set last checked sprite number (5th sprite, or Y=208, or sprite #31) 
    if(C2>0) tms.SR |= L<32? L:31;

    // Return first shown sprite and bit mask of shown sprites
    if (Mask) *Mask=M;
    return(L-1);
}

// ----------------------------------------------------------------------------------------
// Check for the sprite collisions and 5th sprite, and set appropriate
//  bits in the tms.VR status register.
unsigned char CheckSprites(void) {
    unsigned int I,J,LS,LD;
    unsigned char *S,*D,*PS,*PD,*T;
    int DH,DV;

    // Find valid, displayed sprites
    DV = TMS9918_Sprites16 ? -16:-8;
    for  (I=J=0,S=tms.SprTab;(I<32)&&(S[0]!=208);++I,S+=4)
    {
        if (((S[0]<191)||(S[0]>255+DV))&&((int)S[1]-(S[3]&0x80? 32:0)>DV))
            J|=1<<I;
    }
    //DH = 255-(TMS9918_Sprites16? 16:8);
    // for (I=J=0,S=tms.SprTab;(I<32)&&(S[0]!=208);++I,S+=4)
    //if((S[0]<191)||(S[0]>DH)) J|=1<<I;

    if(TMS9918_Sprites16)
    {
        for(S=tms.SprTab;J;J>>=1,S+=4)
        {
            if(J&1)
            {
                for(I=J>>1,D=S+4;I;I>>=1,D+=4)
                {
                    if(I&1)
                    {
                        DV=(int) S[0]-(int) D[0];
                        if ((DV<16)&&(DV>-16))
                        //if((DV<16)||(DV>240))
                        {
                            DH=S[1]-D[1];
                            DH=(int)S[1]-(int)D[1]-(S[3]&0x80? 32:0)+(D[3]&0x80? 32:0);
                            if ((DH<16)&&(DH>-16))
                            //if((DH<16)||(DH>240))
                            {
                                PS=tms.SprGen+((int)(S[2]&0xFC)<<3);
                                PD=tms.SprGen+((int)(D[2]&0xFC)<<3);
                                if (DV>0) PD+=DV; else { DV=-DV;PS+=DV; }
                                if (DH<0) { DH=-DH;T=PS;PS=PD;PD=T; }
                                //if(DV<16) PD+=DV; else { DV=256-DV;PS+=DV; }
                                //if(DH>240) { DH=256-DH;T=PS;PS=PD;PD=T; }
                                while(DV<16)
                                {
                                    LS=((unsigned int)PS[0]<<8)+PS[16];
                                    LD=((unsigned int)PD[0]<<8)+PD[16];
                                    if (LD&(LS>>DH)) break;
                                    else { ++DV;++PS;++PD; }
                                }
                                if(DV<16) return(1);
                            }
                        }
                    } // (I&1)
                }
            } // (J&1)
        }
    }
    else
    {
        for(S=tms.SprTab;J;J>>=1,S+=4)
        {
            if(J&1)
            {
                for(I=J>>1,D=S+4;I;I>>=1,D+=4)
                {
                    if(I&1)
                    {
                        DV=(int)S[0]-(int)D[0];
                        if((DV<8)&&(DV>-8))
                        //DV=S[0]-D[0];
                        //if((DV<8)||(DV>248))
                        {
                            //DH=S[1]-D[1];
                            //if((DH<8)||(DH>248))
                            DH=(int)S[1]-(int)D[1]-(S[3]&0x80? 32:0)+(D[3]&0x80? 32:0);
                            if ((DH<8)&&(DH>-8))
                            {
                                PS=tms.SprGen+((int)S[2]<<3);
                                PD=tms.SprGen+((int)D[2]<<3);
                                //if (DV<8) PD+=DV; else { DV=256-DV;PS+=DV; }
                                //if(DH>248) { DH=256-DH;T=PS;PS=PD;PD=T; }
                                if (DV>0) PD+=DV; else { DV=-DV;PS+=DV; }
                                if (DH<0) { DH=-DH;T=PS;PS=PD;PD=T; }
                                while((DV<8)&&!(*PD&(*PS>>DH))) { ++DV;++PS;++PD; }
                                if(DV<8) return(1);
                            }
                        }
                    } // (I&1)
                }
            } // (J&1)
        }
    }

    return(0);
}

// ----------------------------------------------------------------------------------------
// This function is called from RefreshLine#() to refresh
//  the screen border.
void RefreshBorder(unsigned char Y)
{
    unsigned char *P,BC;
    int J,N;

    // Border color
    BC=tms.BGColor;

    // Screen buffer
    P=cv_display;
    J=TVW*(Y+(TVH-192)/2);

    // For the first line, refresh top border
    if (Y) P+=J;
    else for (;J;J--) *P++=BC;

    // Calculate number of pixels
    N=(TVW-(tms.Mode ? 256:240))/2;

    // Refresh left border
    for (J=N;J;J--) *P++=BC;

    // Refresh right border
    P+=TVW-(N<<1);
    for (J=N;J;J--) *P++=BC;

    // For the last line, refresh bottom border
    if (Y==191)
        for (J=TVW*(TVH-192)/2;J;J--) *P++=BC;
}

// ----------------------------------------------------------------------------------------
// This function is called from RefreshLine#() to refresh
//  sprites.
void RefreshSprites(unsigned char Y)
{
    const unsigned char SprHeights[4] = { 8,16,16,32 };
    unsigned char OH,IH,*PT,*AT;
    unsigned char *P,*T,sColor;
    signed int L,K,N;
    unsigned int M;


    // Find sprites to show, update 5th sprite status
    N = ScanSprites(Y,&M);
    if ((N<0) || !M) return;

    T  = cv_display + TVW*(Y+(TVH-192)/2) + TVW/2-128;
    OH = SprHeights[tms.VR[1]&0x03];
    IH = SprHeights[tms.VR[1]&0x02];
    AT = tms.SprTab+(N<<2); // AT = tms.SprTab-4;

    // For each possibly shown sprite...
    for( ; N>=0 ; --N, AT-=4)
    {
        if(M&(1<<N))
        {
            sColor=AT[3];                   // C = sprite attributes
            L=sColor&0x80? AT[1]-32:AT[1];  // Sprite may be shifted left by 32
            sColor&=0x0F;      // C = sprite color

            if((L<256)&&(L>-OH)&&sColor)
            {
                K=AT[0];    // K = sprite Y coordinate
                if (K>256-IH) K-=256;    // Y coordinate may be negative

		        sColor = tms.IdxPal[sColor];
                P  = T+L;
                K  = Y-K-1;
                PT = tms.SprGen
                    + ((int)(IH>8? (AT[2]&0xFC):AT[2])<<3)
                    + (OH>IH? (K>>1):K);

                // Mask 1: clip left sprite boundary
                K=L>=0? 0xFFFF:(0x10000>>(OH>IH? (-L>>1):-L))-1;

                // Mask 2: clip right sprite boundary
                L+=OH-257;
                if(L>=0)
                {
                    L=(IH>8? 0x0002:0x0200)<<(OH>IH? (L>>1):L);
                    K&=~(L-1);
                }

                // Get and clip the sprite data
                K&=((int)PT[0]<<8)|(IH>8? PT[16]:0x00);

                if(OH>IH)
                {
                    // Big (zoomed) sprite
                    // Draw left 16 pixels of the sprite
                    if(K&0xFF00)
                    {
                        if (K&0x8000) { P[0] = sColor; P[1] = sColor; }
                        if (K&0x4000) { P[2] = sColor; P[3] = sColor; }
                        if (K&0x2000) { P[4] = sColor; P[5] = sColor; }
                        if (K&0x1000) { P[6] = sColor; P[7] = sColor; }
                        if (K&0x0800) { P[8] = sColor; P[9] = sColor; }
                        if (K&0x0400) { P[10] = sColor; P[11] = sColor; }
                        if (K&0x0200) { P[12] = sColor; P[13] = sColor; }
                        if (K&0x0100) { P[14] = sColor; P[15] = sColor; }
                    }

                    // Draw right 16 pixels of the sprite
                    if(K&0x00FF)
                    {
                        if (K&0x0080) { P[16] = sColor; P[17] = sColor; }
                        if (K&0x0040) { P[18] = sColor; P[19] = sColor; }
                        if (K&0x0020) { P[20] = sColor; P[21] = sColor; }
                        if (K&0x0010) { P[22] = sColor; P[23] = sColor; }
                        if (K&0x0008) { P[24] = sColor; P[25] = sColor; }
                        if (K&0x0004) { P[26] = sColor; P[27] = sColor; }
                        if (K&0x0002) { P[28] = sColor; P[29] = sColor; }
                        if (K&0x0001) { P[30] = sColor; P[31] = sColor; }
                    }
                }
                else
                {
                    // Normal (unzoomed) sprite
                    // Draw left 8 pixels of the sprite
                    if(K&0xFF00)
                    {
                        if (K&0x8000) P[0]=sColor;
                        if (K&0x4000) P[1]=sColor;
                        if (K&0x2000) P[2]=sColor;
                        if (K&0x1000) P[3]=sColor;
                        if (K&0x0800) P[4]=sColor;
                        if (K&0x0400) P[5]=sColor;
                        if (K&0x0200) P[6]=sColor;
                        if (K&0x0100) P[7]=sColor;
                    }

                    // Draw right 8 pixels of the sprite
                    if(K&0x00FF)
                    {
                        if (K&0x0080) P[8]=sColor;
                        if (K&0x0040) P[9]=sColor;
                        if (K&0x0020) P[10]=sColor;
                        if (K&0x0010) P[11]=sColor;
                        if (K&0x0008) P[12]=sColor;
                        if (K&0x0004) P[13]=sColor;
                        if (K&0x0002) P[14]=sColor;
                        if (K&0x0001) P[15]=sColor;
                    }
                } // else
            } // if((L<256)&&(L>-OH)&&sColor)
        } //if(M&(1<<N))
    }
}

//---------------------------------------------------------------------------
// Refresh line Y (0..191) of SCREEN0, including sprites in this line
void _TMS9928A_mode0(unsigned char uY) {
    unsigned char X,K,Offset,FC,BC;
    unsigned char *P;
    unsigned char *T;
    unsigned char *PGT;
    unsigned int I;

    P  = cv_display
        + TVW*(uY+(TVH-192)/2)
        + TVW/2-128+8;

    if(!TMS9918_ScreenON)
        for(X=0;X<240;X++) *P++=BC;
    else
    {
        BC=tms.BGColor;
        FC=tms.FGColor;
        Offset=(uY & 0x07);

        PGT = tms.ChrGen+Offset;

        // 0x80 before 0x40 to avoid pb
        if (tms.VR[0]&2)
        {
            if (uY >= 0x80)
            {
                switch (tms.VR[4]&3)
                {
                case 0: break;
                case 1: PGT+=0x1000; break;
                case 2: break; //PGT-=0x800; break;
                case 3: PGT+=0x1000; break; //PGT+=0x800; break;
                }
            }
            else if (uY >= 0x40)
            {
                if (tms.VR[4]&2) PGT+=0x800;
            }
        }
        T=tms.ChrTab+(uY>>3)*40;
        for (X=0;X<40;X++)
        {
            I = ((int)*T<<3);
            K=PGT[I];
            P[0]=K&0x80? FC:BC;P[1]=K&0x40? FC:BC;
            P[2]=K&0x20? FC:BC;P[3]=K&0x10? FC:BC;
            P[4]=K&0x08? FC:BC;P[5]=K&0x04? FC:BC;
            P+=6;T++;
        }
    }

    // Refresh screen border
    RefreshBorder(uY);
}

//---------------------------------------------------------------------------
// Refresh line Y (0..191) of SCREEN1, including sprites in this line
void _TMS9928A_mode1(unsigned char uY) {
	unsigned char X,K, Offset,FC,BC;
    unsigned int J;
	unsigned char *T;
	unsigned char *P;

    P  = cv_display + TVW*(uY+(TVH-192)/2) + TVW/2-128;

	if(!TMS9918_ScreenON)
    {
        BC=tms.BGColor;
		for (J=0;J<256;J++) *P++=BC;
	}
	else
    {
		T=tms.ChrTab+((uY & 0xF8)<<2);
		Offset=uY & 0x07;

		for(X=0;X<32;X++)
        {
			K=*T;
			BC=tms.ColTab[K>>3];
			K=tms.ChrGen[((int)K<<3)+Offset];
			FC=tms.IdxPal[BC>>4];
			BC=tms.IdxPal[BC&0x0F];
			P[0]=K&0x80? FC:BC;P[1]=K&0x40? FC:BC;
			P[2]=K&0x20? FC:BC;P[3]=K&0x10? FC:BC;
			P[4]=K&0x08? FC:BC;P[5]=K&0x04? FC:BC;
			P[6]=K&0x02? FC:BC;P[7]=K&0x01? FC:BC;
			P+=8;T++;
		}
		RefreshSprites(uY);
	}

    // Refresh screen border
    RefreshBorder(uY);
}

//---------------------------------------------------------------------------
// Refresh line Y (0..191) of SCREEN2, including sprites in this line
void _TMS9928A_mode2(unsigned char uY) {
	unsigned int X,K,J, FC,BC,Offset;
	unsigned char *T;
	unsigned char *PGT,*CLT;
	unsigned int I;
	unsigned char *P;

    P  = cv_display + TVW*(uY+(TVH-192)/2) + TVW/2-128;

	if (!TMS9918_ScreenON)
    {
        BC=tms.BGColor;
		for (J=0;J<256;J++) *P++=BC;
	}
	else
    {
        Offset=uY&0x07;
        PGT = tms.ChrGen+Offset;
        CLT = tms.ColTab+Offset;

        // 0x80 before 0x40 to avoid pb
        if (uY >= 0x80)
        {
            switch (tms.VR[4]&3)
            {
            case 0: break;
            case 1: PGT+=0x1000; break;
            case 2: break; //PGT-=0x800; break;
            case 3: PGT+=0x1000; break; //PGT+=0x800; break;
            }
            switch (tms.VR[3]&0x60)
            {
            case 0: break;
            case 0x20: CLT+=0x1000; break;
            case 0x40: break; //CLT-=0x800; break;
            case 0x60: CLT+=0x1000; break; //CLT+=0x800; break;
            }
        }
        else if (uY >= 0x40)
        {
            if (tms.VR[4]&2) PGT+=0x800;
            if (tms.VR[3]&0x40) CLT+=0x800;
        }
        T = tms.ChrTab + ((uY & 0xF8) << 2);


        for(X=0;X<32;X++)
        {
            I=((int)*T<<3);
/*      K    = CLT[(J+I)&CLTMask];
      FC   = VDP->XPal[K>>4];
      BC   = VDP->XPal[K&0x0F];
      K    = PGT[(J+I)&PGTMask];
*/

            K=PGT[I];
            BC=tms.IdxPal[CLT[I]&0x0F];
            FC=tms.IdxPal[CLT[I]>>4];

            P[0] = K & 0x80? FC:BC;
            P[1] = K & 0x40? FC:BC;
            P[2] = K & 0x20? FC:BC;
            P[3] = K & 0x10? FC:BC;
            P[4] = K & 0x08? FC:BC;
            P[5] = K & 0x04? FC:BC;
            P[6] = K & 0x02? FC:BC;
            P[7] = K & 0x01? FC:BC;
            P+=8;
            T++;
        }
        RefreshSprites(uY);
    }

    // Refresh screen border
    RefreshBorder(uY);
}

//---------------------------------------------------------------------------
// Refresh line Y (0..191) of SCREEN3, including sprites in this line
void _TMS9928A_mode3(unsigned char uY) {
    unsigned char X,K,Offset;
    unsigned char *P,*T;
    unsigned char *PGT;
    unsigned int I,BC,J;

    P  = cv_display + TVW*(uY+(TVH-192)/2) + TVW/2-128;

    if(!TMS9918_ScreenON)
    {
        BC=tms.BGColor;
		for (J=0;J<256;J++) *P++=BC;
    }
    else {
        Offset=(uY&0x1C)>>2;
        PGT = tms.ChrGen+Offset;

        // 0x80 before 0x40 to avoid pb
        if (tms.VR[0]&2)
        {
            if (uY >= 0x80)
            {
                switch (tms.VR[4]&3)
                {
                case 0: break;
                case 1: PGT+=0x1000; break;
                case 2: break; //PGT-=0x800; break;
                case 3: PGT+=0x1000; break; //PGT+=0x800; break;
                }
            }
            else if (uY >= 0x40)
            {
                if (tms.VR[4]&2) PGT+=0x800;
            }
        }

        T=tms.ChrTab+((int)(uY&0xF8)<<2);
        for(X=0;X<32;X++)
        {
            I = ((int)*T<<3);
            K=PGT[I];
            P[0]=P[1]=P[2]=P[3]=tms.IdxPal[K>>4];
            P[4]=P[5]=P[6]=P[7]=tms.IdxPal[K&0x0F];
            P+=8;T++;
        }
        RefreshSprites(uY);
    }

    // Refresh screen border
    RefreshBorder(uY);
}

// Call this routine on every scanline to update the screen buffer.
unsigned char tms9918_loop(void) {
	unsigned char bIRQ;
	//unsigned int inX,inColor;

	// No IRQ yet
	bIRQ=0;

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

