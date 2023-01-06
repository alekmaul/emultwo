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

const unsigned char TMS9918A_palette[6*16*3] = {
    // Palette taken from the datasheet and scaled by 255/224
    0x00,0x00,0x00 , 0x00,0x00,0x00 , 0x24,0xDA,0x24 , 0x6D,0xFF,0x6D , 0x24,0x24,0xFF , 0x48,0x6D,0xFF , 0xB6,0x24,0x24 , 0x48,0xDA,0xFF ,
    0xFF,0x24,0x24 , 0xFF,0x6D,0x6D , 0xDA,0xDA,0x24 , 0xDA,0xDA,0x91 , 0x24,0x91,0x24 , 0xDA,0x48,0xB6 , 0xB6,0xB6,0xB6 , 0xFF,0xFF,0xFF ,

    // TMS9918 Palette from the datasheet
   0x00,0x00,0x00 , 0x00,0x00,0x00 , 0x20,0xC0,0x20 , 0x60,0xE0,0x60 , 0x20,0x20,0xE0 , 0x40,0x60,0xE0 , 0xA0,0x20,0x20 , 0x40,0xC0,0xE0 ,
   0xE0,0x20,0x20 , 0xE0,0x60,0x60 , 0xC0,0xC0,0x20 , 0xC0,0xC0,0x80 , 0x20,0x80,0x20 , 0xC0,0x40,0xA0 , 0xA0,0xA0,0xA0 , 0xE0,0xE0,0xE0 ,

    // TMS9918 NTSC Palette by Richard F. Drushel */
    0x00,0x00,0x00 , 0x00,0x00,0x00 , 0x47,0xB7,0x3B , 0x7C,0xCF,0x6F , 0x5D,0x4E,0xFF , 0x80,0x72,0xFF , 0xB6,0x62,0x47 , 0x5D,0xC8,0xED ,
    0xD7,0x6B,0x48 , 0xFB,0x8F,0x6C , 0xC3,0xCD,0x41 , 0xD3,0xDA,0x76 , 0x3E,0x9F,0x2F , 0xB6,0x64,0xC7 , 0xCC,0xCC,0xCC , 0xFF,0xFF,0xFF ,

    // // black and white
      0,  0,  0,    0,  0,  0,  136,136,136,  172,172,172, 102,102,102,  134,134,134,  120,120,120,  172,172,172,
    136,136,136,  172,172,172,  187,187,187,  205,205,205, 118,118,118,  135,135,135,  204,204,204,  255,255,255,

    // Green scales
      0,  0,  0,    0,  0,  0,    0,118,  0,   43,153, 43, 0, 81,  0,    0,118,  0,   43, 81, 43,   43,153, 43,
     43, 81, 43,   43,118, 43,   43,153, 43,   43,187, 43, 43, 81, 43,   43,118, 43,   43,221, 43,    0,255,  0,

    // Ambre scale
      0,  0,  0,    0,  0,  0,  118, 81, 43,  153,118,  0, 81, 43,  0,  118, 81,  0,   81, 43,  0,  187,118, 43,
    118, 81,  0,  153,118, 43,  187,118, 43,  221,153,  0, 118, 81,  0,  153,118, 43,  221,153,  0,  255,187,  0
};

tTMS9981A tms;

// ----------------------------------------------------------------------------------------
// Reset the tms.VR
void tms9918_reset(void) {
	unsigned char i;

	memset(tms.VR,0x00,sizeof(tms.VR));                  // Init tms.VR Registers
	memset(tms.ram,0x00, 0x4000);               // Init tms.VR VRAM

	tms.VKey=1;                                        // tms.VR address latch key
	tms.SR=0x00;                                // tms.VR status register
	tms.FGColor=tms.BGColor=0;                             // Fore/Background color
	tms.Mode=0;                                     // Current screenmode
	tms.CurLine=0;                                     // Current scanline
        tms.DLatch = 0;                                 // Current Data latch
        tms.ScanLines = TMS9918_LINES;                     // Default for NTSC

	tms.ChrTab=tms.ColTab=tms.ChrGen=tms.ram;               // tms.VR tables (screen)
	tms.SprTab=tms.SprGen=tms.ram;                      // tms.VR tables (sprites)

	tms.UCount = 0;
	tms.VAddr = 0x0000;
    // ??? CurLine   = TMS9918_LINES;
	for (i=0;i<16;i++) tms.IdxPal[i] = i;
}

// ----------------------------------------------------------------------------------------
// Writing a data to a tms.VR register
unsigned char Write9918(int iReg,unsigned char value) {
	unsigned int J;
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
			switch(TMS9918_Mode) {
				case 0x00: J=1;break;
				case 0x01: J=2;break;
				case 0x02: J=3;break; // ALEK20140906 0 before
				case 0x04: J=0;break; // ALEK20140906 3 before
				default:   J=tms.Mode;break;
			}

			// If mode was changed, recompute table addresses
			if((J!=tms.Mode)||!VRAMMask) {
				VRAMMask    = TMS9918_VRAMMask;
				tms.ChrTab=tms.ram+(((int)(tms.VR[2]&SCR[J].R2)<<10)&VRAMMask);
				tms.ColTab=tms.ram+(((int)(tms.VR[3]&SCR[J].R3)<<6)&VRAMMask);
				tms.ChrGen=tms.ram+(((int)(tms.VR[4]&SCR[J].R4)<<11)&VRAMMask);
				tms.SprTab=tms.ram+(((int)(tms.VR[5]&SCR[J].R5)<<7)&VRAMMask);
				tms.SprGen=tms.ram+(((int)(tms.VR[6]&SCR[J].R6)<<11)&VRAMMask);
				tms.Mode=J;
			}
                        //
                        /*if ((iReg==1) && (tms.SR&TMS9918_STAT_VBLANK) ) {
                                if (value&TMS9918_REG1_IRQ)
                                        z80_set_irq_line(machine.typeinterrupt, ASSERT_LINE);
                                else z80_set_irq_line(machine.typeinterrupt, CLEAR_LINE);
                        }*/
			break;
		case  2: // Name Table
			tms.ChrTab=tms.ram+(((int)(value&SCR[tms.Mode].R2)<<10)&VRAMMask);
			break;
		case  3: // Color Table
			tms.ColTab=tms.ram+(((int)(value&SCR[tms.Mode].R3)<<6)&VRAMMask);
			break;
		case  4: // Pattern Table
			tms.ChrGen=tms.ram+(((int)(value&SCR[tms.Mode].R4)<<11)&VRAMMask);
			break;
		case  5: // Sprite Attributes
			tms.SprTab=tms.ram+(((int)(value&SCR[tms.Mode].R5)<<7)&VRAMMask);
			break;
		case  6: // Sprite Patterns
			//value&=0x3F;
			//tms.SprGen=tms.ram+((long)value<<11);
			tms.SprGen=tms.ram+(((int)(value&SCR[tms.Mode].R6)<<11)&VRAMMask);
			break;
		case  7:  // Foreground and background colors
			tms.FGColor=tms.IdxPal[value>>4];
			value &= 0x0F;
			tms.IdxPal[0] = tms.IdxPal[ value ? value : 1];
			tms.BGColor=tms.IdxPal[value]; // value&0x0F;
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
/* ALEK 01/10/2018
	if(WKey) {
	//  if(!tms.VKey) {
		// tms.VR in the proper WRITE mode
		tms.ram[tms.VAddr]=value;
		tms.VAddr  = (tms.VAddr+1)&0x3FFF;
	}
	else {
		// tms.VR in the READ mode
		tms.VAddr  = (tms.VAddr+1)&0x3FFF;
		tms.ram[tms.VAddr]=value;
	}
*/
}

// ----------------------------------------------------------------------------------------
// Read a value from the tms.VR Data Port
unsigned char tms9918_readdata(void) {
	unsigned char retval;

// ALEK 01/10/2018    retval = tms.ram[tms.VAddr];
    retval = tms.DLatch;
    tms.DLatch = tms.ram[tms.VAddr];
	tms.VAddr = (tms.VAddr+1)&0x3FFF;

	//ALEK 10/06/2013 tms.VKey   = 1;
	return(retval);
}

// ----------------------------------------------------------------------------------------
// Write a value to the tms.VR Control Port. Enabling IRQs or not
unsigned char tms9918_writectrl(unsigned char value) {
	if (tms.VKey)
        {
	    tms.VKey=0;
		// ALEK 01/10/2018 tms.VRClatch=value;
                tms.VAddr=((tms.VAddr&0xFF00)|value)&0x3FFF;
	}
	else
        {
		tms.VKey=1;
                tms.VAddr = ((tms.VAddr&0x00FF)|((int)value<<8))&0x3FFF; // 01/10/2018
		switch(value&0xC0) {
			case 0x00:
                                // In READ mode, read the first unsigned char from VRAM
			        //ALEK 01/10/2018 case 0x40:
				// ALEK 01/10/2018 tms.VAddr=(tms.VRClatch|((int)value<<8))&0x3FFF;
				// ALEK 01/10/2018 WKey=value&0x40;
                                tms.DLatch = tms.ram[tms.VAddr];
                                tms.VAddr  = (tms.VAddr+1)&0x3FFF;
				break;
                        case 0x80:
                        // Enabling IRQs may cause an IRQ here
	    		//ALEK 01/10/2018 return( Write9918(value&0x0F,tms.VRClatch) );
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
/*ALEK  01/10/2018
	if (tms.SR & TMS9918_STAT_VBLANK)
		tms.SR&=(TMS9918_STAT_5THNUM | TMS9918_STAT_OVRLAP);
	else
		tms.SR=TMS9918_STAT_5THNUM;
// @@@ Resetting it here breaks ColecoVision Sir Lancelot!
	tms.VKey   = 1;
    */
        tms.SR &= TMS9918_STAT_5THNUM|TMS9918_STAT_5THSPR;    // 0x1F | 0x40 -> 0x0101111

        //z80_set_irq_line(machine.typeinterrupt, CLEAR_LINE);

        return(retval);
}

// ----------------------------------------------------------------------------------------
// Check for the sprite collisions and 5th sprite, and set appropriate
//  bits in the tms.VR status register.
unsigned char CheckSprites(void) {
  unsigned int I,J,LS,LD;
  unsigned char DH,DV,*S,*D,*PS,*PD,*T;

    // Find valid, displayed sprites
    DH = 255-(TMS9918_Sprites16? 16:8);
    for(I=J=0,S=tms.SprTab;(I<32)&&(S[0]!=208);++I,S+=4)
        if((S[0]<191)||(S[0]>DH)) J|=1<<I;

    if(TMS9918_Sprites16)
    {
        for(S=tms.SprTab;J;J>>=1,S+=4)
            if(J&1)
                for(I=J>>1,D=S+4;I;I>>=1,D+=4)
                    if(I&1)
                    {
                        DV=S[0]-D[0];
                        if((DV<16)||(DV>240))
            {
              DH=S[1]-D[1];
              if((DH<16)||(DH>240))
              {
                PS=tms.SprGen+((int)(S[2]&0xFC)<<3);
                PD=tms.SprGen+((int)(D[2]&0xFC)<<3);
                if(DV<16) PD+=DV; else { DV=256-DV;PS+=DV; }
                if(DH>240) { DH=256-DH;T=PS;PS=PD;PD=T; }
                while(DV<16)
                {
                  LS=((unsigned int)PS[0]<<8)+PS[16];
                  LD=((unsigned int)PD[0]<<8)+PD[16];
                  if(LD&(LS>>DH)) break;
                  else { ++DV;++PS;++PD; }
                }
                if(DV<16) return(1);
              }
            }
          }
  }
  else
  {
    for(S=tms.SprTab;J;J>>=1,S+=4)
      if(J&1)
        for(I=J>>1,D=S+4;I;I>>=1,D+=4)
          if(I&1)
          {
            DV=S[0]-D[0];
            if((DV<8)||(DV>248))
            {
              DH=S[1]-D[1];
              if((DH<8)||(DH>248))
              {
                PS=tms.SprGen+((int)S[2]<<3);
                PD=tms.SprGen+((int)D[2]<<3);
                if(DV<8) PD+=DV; else { DV=256-DV;PS+=DV; }
                if(DH>248) { DH=256-DH;T=PS;PS=PD;PD=T; }
                while((DV<8)&&!(*PD&(*PS>>DH))) { ++DV;++PS;++PD; }
                if(DV<8) return(1);
              }
            }
          }
  }

  return(0);
}

/** RefreshBorder() ******************************************/
/** This function is called from RefreshLine#() to refresh  **/
/** the screen border.                                      **/
/*************************************************************/
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
    if(Y) P+=J;
    else for(;J;J--) *P++=BC;

    // Calculate number of pixels
    N=(TVW-(tms.Mode ? 256:240))/2;

    // Refresh left border
    for(J=N;J;J--) *P++=BC;

    // Refresh right border
    P+=TVW-(N<<1);
    for(J=N;J;J--) *P++=BC;

    // For the last line, refresh bottom border
    if(Y==191)
        for(J=TVW*(TVH-192)/2;J;J--) *P++=BC;
}

/** RefreshSprites() *****************************************/
/** This function is called from RefreshLine#() to refresh  **/
/** sprites.                                                **/
/*************************************************************/
void RefreshSprites(unsigned char Y)
{
    const unsigned char SprHeights[4] = { 8,16,16,32 };
    unsigned char OH,IH,*PT,*AT;
    unsigned char *P,*T,sColor;
    signed int L,K;
    unsigned int M;

    //  T  = cv_display+256*Y;
    T  = cv_display + TVW*(Y+(TVH-192)/2) + TVW/2-128;

    OH = SprHeights[tms.VR[1]&0x03];
    IH = SprHeights[tms.VR[1]&0x02];
    AT = tms.SprTab-4;
    sColor  = 4;
    M  = 0;
    L  = 0;

    do {
        M<<=1;AT+=4;L++;     /* Iterate through tms.SprTab */
        K=AT[0];             /* K = sprite Y coordinate */
        if(K==208) break;    /* Iteration terminates if Y=208 */
        if(K>256-IH) K-=256; /* Y coordinate may be negative */

    /* Mark all valid sprites with 1s, break at MaxSprites */
    if((Y>K)&&(Y<=K+OH)) { M|=1;if(!--sColor) break; }
  }
  while(L<32);

  for(;M;M>>=1,AT-=4)
    if(M&1) {
      sColor=AT[3];                  /* C = sprite attributes */
      L=sColor&0x80? AT[1]-32:AT[1]; /* Sprite may be shifted left by 32 */
      sColor&=0x0F;                  /* C = sprite color */

      if((L<256)&&(L>-OH)&&sColor) {
        K=AT[0];                /* K = sprite Y coordinate */
        if(K>256-IH) K-=256;    /* Y coordinate may be negative */

		sColor = tms.IdxPal[sColor];
        P  = T+L;
        K  = Y-K-1;
        PT = tms.SprGen
           + ((int)(IH>8? (AT[2]&0xFC):AT[2])<<3)
           + (OH>IH? (K>>1):K);

        /* Mask 1: clip left sprite boundary */
        K=L>=0? 0xFFFF:(0x10000>>(OH>IH? (-L>>1):-L))-1;

        /* Mask 2: clip right sprite boundary */
        L+=OH-257;
        if(L>=0) {
          L=(IH>8? 0x0002:0x0200)<<(OH>IH? (L>>1):L);
          K&=~(L-1);
        }

        /* Get and clip the sprite data */
        K&=((int)PT[0]<<8)|(IH>8? PT[16]:0x00);

        if(OH>IH) {
          /* Big (zoomed) sprite */
          /* Draw left 16 pixels of the sprite */
          if(K&0xFF00) {
            if(K&0x8000) { P[0] = sColor; P[1] = sColor; }
            if(K&0x4000) { P[2] = sColor; P[3] = sColor; }
            if(K&0x2000) { P[4] = sColor; P[5] = sColor; }
            if(K&0x1000) { P[6] = sColor; P[7] = sColor; }
            if(K&0x0800) { P[8] = sColor; P[9] = sColor; }
            if(K&0x0400) { P[10] = sColor; P[11] = sColor; }
            if(K&0x0200) { P[12] = sColor; P[13] = sColor; }
            if(K&0x0100) { P[14] = sColor; P[15] = sColor; }
          }

          /* Draw right 16 pixels of the sprite */
          if(K&0x00FF) {
            if(K&0x0080) { P[16] = sColor; P[17] = sColor; }
            if(K&0x0040) { P[18] = sColor; P[19] = sColor; }
            if(K&0x0020) { P[20] = sColor; P[21] = sColor; }
            if(K&0x0010) { P[22] = sColor; P[23] = sColor; }
            if(K&0x0008) { P[24] = sColor; P[25] = sColor; }
            if(K&0x0004) { P[26] = sColor; P[27] = sColor; }
            if(K&0x0002) { P[28] = sColor; P[29] = sColor; }
            if(K&0x0001) { P[30] = sColor; P[31] = sColor; }
          }
        }
        else {
          /* Normal (unzoomed) sprite */

          /* Draw left 8 pixels of the sprite */
          if(K&0xFF00) {
            if(K&0x8000) P[0]=sColor;
            if(K&0x4000) P[1]=sColor;
            if(K&0x2000) P[2]=sColor;
            if(K&0x1000) P[3]=sColor;
            if(K&0x0800) P[4]=sColor;
            if(K&0x0400) P[5]=sColor;
            if(K&0x0200) P[6]=sColor;
            if(K&0x0100) P[7]=sColor;
          }

          /* Draw right 8 pixels of the sprite */
          if(K&0x00FF)
          {
            if(K&0x0080) P[8]=sColor;
            if(K&0x0040) P[9]=sColor;
            if(K&0x0020) P[10]=sColor;
            if(K&0x0010) P[11]=sColor;
            if(K&0x0008) P[12]=sColor;
            if(K&0x0004) P[13]=sColor;
            if(K&0x0002) P[14]=sColor;
            if(K&0x0001) P[15]=sColor;
          }
        }
      }
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

    P=cv_display+(uY<<8);

  if(!TMS9918_ScreenON)
    memset(P,tms.BGColor,256);
  else {
		//fprintf(stderr,"_TMS9928A_mode0 %d\n",uY); fflush(stderr);
    BC=tms.BGColor;
    FC=tms.FGColor;
    Offset=(uY & 0x07);

    PGT = tms.ChrGen+Offset;

    // 0x80 before 0x40 to avoid pb
    if (tms.VR[0]&2) {
      if (uY >= 0x80) {
        switch (tms.VR[4]&3) {
         case 0: break;
         case 1: PGT+=0x1000; break;
         case 2: break; //PGT-=0x800; break;
         case 3: PGT+=0x1000; break; //PGT+=0x800; break;
        }
      }
      else if (uY >= 0x40) {
        if (tms.VR[4]&2) PGT+=0x800;
      } 
    }
    T=tms.ChrTab+(uY>>3)*40;
    for(X=0;X<40;X++) {
      I = ((int)*T<<3);
      K=PGT[I];
      P[0]=K&0x80? FC:BC;P[1]=K&0x40? FC:BC;
      P[2]=K&0x20? FC:BC;P[3]=K&0x10? FC:BC;
      P[4]=K&0x08? FC:BC;P[5]=K&0x04? FC:BC;
      P+=6;T++;
    }
  }
}

//---------------------------------------------------------------------------
// Refresh line Y (0..191) of SCREEN1, including sprites in this line
void _TMS9928A_mode1(unsigned char uY) {
	unsigned char X,K,Offset,FC,BC;
	unsigned char *T;
	unsigned char *P;

	//P=cv_display+(uY<<8);
    P  = cv_display + TVW*(uY+(TVH-192)/2) + TVW/2-128;

	if(!TMS9918_ScreenON)  {
		memset(P,tms.BGColor,256);
	}
	else {
		//T=tms.ChrTab+(uY>>3)*32;
		T=tms.ChrTab+((uY & 0xF8)<<2);
		Offset=uY & 0x07;

		for(X=0;X<32;X++) {
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
	unsigned int X,K,FC,BC,Offset;
	unsigned char *T;

	unsigned char *PGT,*CLT;
	unsigned int I;
	unsigned char *P;

	//P=cv_display+(uY<<8);
    P  = cv_display + TVW*(uY+(TVH-192)/2) + TVW/2-128;

	if (!TMS9918_ScreenON) {
		memset(P,tms.BGColor,256);
	}
	else {
		//fprintf(stderr,"_TMS9928A_mode2 %d\n",uY); fflush(stderr);

    Offset=uY&0x07;
    PGT = tms.ChrGen+Offset;
    CLT = tms.ColTab+Offset;

    // 0x80 before 0x40 to avoid pb
    if (uY >= 0x80) {
      switch (tms.VR[4]&3) {
       case 0: break;
       case 1: PGT+=0x1000; break;
       case 2: break; //PGT-=0x800; break;
       case 3: PGT+=0x1000; break; //PGT+=0x800; break;
      }
      switch (tms.VR[3]&0x60) {
       case 0: break;
       case 0x20: CLT+=0x1000; break;
       case 0x40: break; //CLT-=0x800; break;
       case 0x60: CLT+=0x1000; break; //CLT+=0x800; break;
      }
    }
    else if (uY >= 0x40) {
      if (tms.VR[4]&2) PGT+=0x800;
      if (tms.VR[3]&0x40) CLT+=0x800;
    } 
    T = tms.ChrTab + ((uY & 0xF8) << 2);

    for(X=0;X<32;X++) {
      I=((int)*T<<3);
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

}

/** RefreshLine3() *******************************************/
/** Refresh line Y (0..191) of SCREEN3, including sprites   **/
/** in this line.                                           **/
/*************************************************************/
void _TMS9928A_mode3(unsigned char uY) {
  unsigned char X,K,Offset;
  unsigned char *P,*T;
  unsigned char *PGT;
  unsigned int I;

	//P=cv_display+(uY<<8);
    P  = cv_display + TVW*(uY+(TVH-192)/2) + TVW/2-128;

  if(!TMS9918_ScreenON) {
    memset(P,tms.BGColor,256);
  }
  else {
		//fprintf(stderr,"_TMS9928A_mode3 %d\n",uY); fflush(stderr);
    //T=tms.ChrTab+(uY>>3)*32;
    Offset=(uY&0x1C)>>2;
    PGT = tms.ChrGen+Offset;

    // 0x80 before 0x40 to avoid pb
    if (tms.VR[0]&2) {
      if (uY >= 0x80) {
        switch (tms.VR[4]&3) {
         case 0: break;
         case 1: PGT+=0x1000; break;
         case 2: break; //PGT-=0x800; break;
         case 3: PGT+=0x1000; break; //PGT+=0x800; break;
        }
      }
      else if (uY >= 0x40) {
        if (tms.VR[4]&2) PGT+=0x800;
      }
    }

    T=tms.ChrTab+((int)(uY&0xF8)<<2);
    for(X=0;X<32;X++) {
      I = ((int)*T<<3);
      K=PGT[I];
      P[0]=P[1]=P[2]=P[3]=tms.IdxPal[K>>4];
      P[4]=P[5]=P[6]=P[7]=tms.IdxPal[K&0x0F];

      P+=8;T++;
    }
    RefreshSprites(uY);
  }
}


// Graphics paint function
void coleco_paint(void) {
    int ix,iy;
    unsigned char *pcv_display=cv_display;
    unsigned int *LinePtr;

    // Fill screen
    for (iy=0;iy<TVH;iy++) {
        LinePtr=cv_screen[iy];
        for (ix=0;ix<TVW;ix++) {
            unsigned char colour = *(pcv_display+TVW*iy+ix);
            *(LinePtr++)=cv_pal32[colour];
        }
    }
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
	if((tms.CurLine>=TMS9918_START_LINE)&&(tms.CurLine<TMS9918_END_LINE)) {
		if(tms.UCount>=100)
                {
			(SCR[tms.Mode].Refresh)(tms.CurLine-TMS9918_START_LINE);
		}
	}

	// Check if VBlank
	if(tms.CurLine==TMS9918_END_LINE) {
		// Check if we are currently drawing screen
		if(tms.UCount>=100)
                {
			// Refresh all screen
			coleco_paint();
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
