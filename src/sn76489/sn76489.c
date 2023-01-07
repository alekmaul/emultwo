/* EmulTwo  - A Windows Colecovision emulator.
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
 *
 * sn76489.c
 *
 * Based on   SN76489 emulation by Maxim in 2001 and 2002
 *  converted from hisoriginal Delphi implementation
*/

#include <limits.h>

#include "sn76489.h"

sn76496 sn;

#if 1
void sn76489_update(short *buffer, unsigned int length)
{
        int i;
        unsigned int j;
        int vol[4];
        int left, nextstep;
        unsigned int out;
        int val;

        short * out_l;
        out_l = buffer;

        for(i=0; i<4; i++)
        {
                if(sn.Volume[i]==0)
                {
                        if(sn.Count[i] <= (length*SNSTEP)) sn.Count[i] += length*SNSTEP;
                }
        }

        for(j=0; j<length; j++)
        {
                vol[0] = vol[1] = vol[2] = vol[3] = 0;
                out = 0;
                left = 0;

                for(i=0; i<3; i++)
                {
                        if(sn.Output[i]!=0) vol[i] += sn.Count[i];
                        sn.Count[i] -= SNSTEP;
                        while(sn.Count[i] <= 0)
                        {
                                sn.Count[i] += sn.Period[i];
                                if(sn.Count[i] > 0)
                                {
                                        sn.Output[i] ^= 1;
                                        if(sn.Output[i] != 0) vol[i] += sn.Period[i];
                                        break;
                                }
                                sn.Count[i] += sn.Period[i];
                                vol[i] += sn.Period[i];
                        }
                        if(sn.Output[i] != 0) vol[i] -= sn.Count[i];
                }

                left = SNSTEP;

                do {
                        nextstep = (sn.Count[3] < left)?sn.Count[3]:left;

                        if(sn.Output[3] != 0) vol[3] += sn.Count[3];
                        sn.Count[3] -= nextstep;

                        if (sn.Count[3] <= 0)
                        {
                                // white noise mode ?
                                if (sn.Register[6] & 4)
                                {
			                if (((sn.RNG & 0x06) != 0x06) && ((sn.RNG & 0x06) != 0))
                                        {
                                                sn.RNG >>= 1;
                                                sn.RNG |= 0x8000;
					}
					else
                                        {
                                                sn.RNG >>= 1;
					}
				}
                                // Periodic noise mode
				else
                                {
			                if (sn.RNG & 1)
                                        {
                                                sn.RNG >>= 1;
                                                sn.RNG |= 0x8000;
					}
					else
                                        {
                                                sn.RNG >>= 1;
					}
                                }
                                sn.Output[3] = sn.RNG&1;
                                sn.Count[3] += sn.Period[3];
                                if(sn.Output[3]) vol[3] += sn.Period[3];
                        }
                        if(sn.Output[3]) vol[3] -= sn.Count[3];

                        left -= nextstep;
                } while( left > 0 );

                out = (vol[0]*sn.Volume[0]) + (vol[1]*sn.Volume[1]) + (vol[2]*sn.Volume[2]) + (vol[3]*sn.Volume[3]);
                if (out > MAX_OUTPUT*SNSTEP) out = MAX_OUTPUT*SNSTEP;
                val = (out>>16);
                out_l[j] = val;
        }
}

void sn76489_write(int data)
{
  int r=0, c=0, n=0;

  if( (data&0x80)) {
    r = (data&0x70)>>4;
    sn.LastRegister = r;
    sn.Register[r] = (sn.Register[r]&0x3F0)|(data&0x0F);
  }
  else {
    r = sn.LastRegister;
  }
  c = r>>1;

  switch( r ) {
    // tone 0,1,2 frequency
    case 0: case 2: case 4:
      if ((data & 0x80) == 0) sn.Register[r] = (sn.Register[r] & 0x0f) | ((data & 0x3f) << 4);
      sn.Period[c] = sn.UpdateStep * sn.Register[r];
      if(sn.Period[c] == 0) sn.Period[c] = sn.UpdateStep;
      if(r == 4){
        // update noise shift frequency
        if((sn.Register[6]&0x03) == 0x03) sn.Period[3] = sn.Period[2]<<1;
      }
      break;
    // tone 0,1,2 volume ; noise volume
    case 1: case 3: case 5: case 7:
      sn.Volume[c] = sn.VolTable[data&0x0F];
      if ((data & 0x80) == 0) sn.Register[r] = (sn.Register[r] & 0x3f0) | (data & 0x0f);
      break;
    // noise: frequency, mode
    case 6:
      if ((data & 0x80) == 0) sn.Register[r] = (sn.Register[r] & 0x3f0) | (data & 0x0f);
      n = sn.Register[6];

      //JGD sn.NoiseFB = ( (n&4)!=0 )?FB_WNOISE:FB_PNOISE;

      // N/512,N/1024,N/2048,Tone #3 output
      n &= 3;
      sn.Period[3] = (n==3)?(sn.Period[2]<<1):(sn.UpdateStep<<(5+n));
      // reset noise shifter
      sn.RNG = NG_PRESET;
      sn.Output[3] = sn.RNG&1;
      break;
  }
}

void sn76489_setclock(int clock)
{
  sn.UpdateStep = (int)(((double)SNSTEP*sn.SampleRate*16)/clock);
}

void sn76489_setgain(void)
{
    int i;
    double out;
    int gain=0;

    // increase max output basing on gain (0.2 dB per step)
  out = MAX_OUTPUT/3;
  while(gain-- > 0) out *= 1.023292992;	// = (10 ^ (0.2/20))

  // build volume table (2dB per step)
  for(i=0; i<15; i++){
    // limit volume to avoid clipping
    if(out > MAX_OUTPUT/3)  sn.VolTable[i] = MAX_OUTPUT/3;
    else                    sn.VolTable[i] = (int)out;

    out /= 1.258925412;	// = 10 ^ (2/20) = 2dB
  }
  sn.VolTable[15] = 0;
}

void sn76489_reset(int clock, int sample_rate)
{
    int i;
    sn.SampleRate = sample_rate;
    sn76489_setclock(clock);

    for(i=0; i<4; i++) sn.Volume[i] = 0;

    sn.LastRegister = 0;

    for(i=0; i<8; i+=2){
        sn.Register[i] = 0;
        sn.Register[i+1] = 0x0F;	// volume = 0
    }

    for(i=0; i<4; i++){
        sn.Output[i] = 0;
        sn.Period[i] = sn.Count[i] = SNSTEP;
    }

    sn.RNG = NG_PRESET;
    sn.Output[3] = sn.RNG&1;
}

void sn76489_init(int clock, int sample_rate)
{
    sn76489_reset(clock, sample_rate);
    sn76489_setgain();
}

#else
#define NoiseInitialState   0x8000  // Initial state of shift register
#define PSG_CUTOFF          0x6     // Value below which PSG does not output

static const int PSGVolumeValues[2][16] = {
    // These values are taken from a real SMS2's output
    {892,892,892,760,623,497,404,323,257,198,159,123,96,75,60,0},
    // these values are true volumes for 2dB drops at each step (multiply previous by 10^-0.1), normalised at 760
    {1516,1205,957,760,603,479,381,303,240,191,152,120,96,76,60,0}
};

void sn76489_reset(int clock, int sample_rate)
{
  sn76496 *p = &sn;
  int i;

  p->PSGStereo = 0xFF;

  for(i = 0; i <= 3; i++)
  {
    /* Initialise PSG state */
    p->Registers[2*i] = 1;     /* tone freq=1 */
    p->Registers[2*i+1] = 0xf;   /* vol=off */
    p->NoiseFreq = 0x10;

    /* Set counters to 0 */
    p->ToneFreqVals[i] = 0;

    /* Set flip-flops to 1 */
    p->ToneFreqPos[i] = 1;

    /* Set intermediate positions to do-not-use value */
    p->IntermediatePos[i] = LONG_MIN;
  }

  p->LatchedRegister=0;

  /* Initialise noise generator */
  p->NoiseShiftRegister=NoiseInitialState;

  /* Zero clock */
  p->Clock=0;
}

void sn76489_init(int clock, int sample_rate)
{
  sn76496 *p = &sn;
  p->dClock=(float)clock/16/sample_rate;
  sn76489_reset(clock,sample_rate);
  sn76489_config(MUTE_ALLON, BOOST_ON, VOL_TRUNC, FB_SC3000); //(sms.console < CONSOLE_SMS) ? FB_SC3000 : FB_SEGAVDP);
}

void sn76489_config(int mute, int boost, int volume, int feedback)
{
  sn76496 *p = &sn;

  p->Mute = mute;
  p->BoostNoise = boost;
  p->VolumeArray = volume;
  p->WhiteNoiseFeedback = feedback;
}

void sn76489_write(int data)
{
  sn76496 *p = &sn;

  if (data&0x80) {
    /* Latch/data byte  %1 cc t dddd */
    p->LatchedRegister=((data>>4)&0x07);
    p->Registers[p->LatchedRegister]=
      (p->Registers[p->LatchedRegister] & 0x3f0)  /* zero low 4 bits */
      | (data&0xf);               /* and replace with data */
  } else {
    /* Data byte    %0 - dddddd */
    if (!(p->LatchedRegister%2)&&(p->LatchedRegister<5))
      /* Tone register */
      p->Registers[p->LatchedRegister]=
        (p->Registers[p->LatchedRegister] & 0x00f)  /* zero high 6 bits */
        | ((data&0x3f)<<4);           /* and replace with data */
  else
      /* Other register */
      p->Registers[p->LatchedRegister]=data&0x0f;     /* Replace with data */
  }
  switch (p->LatchedRegister) {
  case 0:
  case 2:
  case 4: /* Tone channels */
    if (p->Registers[p->LatchedRegister]==0) p->Registers[p->LatchedRegister]=1;  /* Zero frequency changed to 1 to avoid div/0 */
  break;
  case 6: /* Noise */
    p->NoiseShiftRegister=NoiseInitialState;   /* reset shift register */
    p->NoiseFreq=0x10<<(p->Registers[6]&0x3);   /* set noise signal generator frequency */
  break;
  }
}

void sn76489_update(short *buffer, unsigned int length)
{
  sn76496 *p = &sn;
  unsigned int i, j;

    for(j = 0; j < length; j++)
    {
        for (i=0;i<=2;++i)
        {
            if (sn.IntermediatePos[i]!=LONG_MIN)
                sn.Channels[i]=(sn.Mute >> i & 0x1)*PSGVolumeValues[sn.VolumeArray][sn.Registers[2*i+1]]*sn.IntermediatePos[i]/65536;
            else
                sn.Channels[i]=(sn.Mute >> i & 0x1)*PSGVolumeValues[sn.VolumeArray][sn.Registers[2*i+1]]*sn.ToneFreqPos[i];
        }
    p->Channels[3]=(short)((p->Mute >> 3 & 0x1)*PSGVolumeValues[p->VolumeArray][p->Registers[7]]*(p->NoiseShiftRegister & 0x1));

    if (p->BoostNoise) p->Channels[3]<<=1; /* Double noise volume to make some people happy */


    buffer[j] =0;
    for (i=0;i<=3;++i)
    {
        buffer[j] +=p->Channels[i];
    }

    p->Clock+=p->dClock;
    p->NumClocksForSample=(int)p->Clock;  /* truncates */
    p->Clock-=p->NumClocksForSample;  /* remove integer part */

    /* Decrement tone channel counters */
    for (i=0;i<=2;++i)
      p->ToneFreqVals[i]-=p->NumClocksForSample;

    /* Noise channel: match to tone2 or decrement its counter */
    if (p->NoiseFreq==0x80) p->ToneFreqVals[3]=p->ToneFreqVals[2];
    else p->ToneFreqVals[3]-=p->NumClocksForSample;


    /* Tone channels: */
    for (i=0;i<=2;++i) {
      if (p->ToneFreqVals[i]<=0) {   /* If it gets below 0... */
        if (p->Registers[i*2]>PSG_CUTOFF) {
          /* Calculate how much of the sample is + and how much is - */
          /* Go to floating point and include the clock fraction for extreme accuracy :D */
          /* Store as long int, maybe it's faster? I'm not very good at this */
          p->IntermediatePos[i]=(long)((p->NumClocksForSample-p->Clock+2*p->ToneFreqVals[i])*p->ToneFreqPos[i]/(p->NumClocksForSample+p->Clock)*65536);
          p->ToneFreqPos[i]=-p->ToneFreqPos[i]; /* Flip the flip-flop */
        } else {
          p->ToneFreqPos[i]=1;   /* stuck value */
          p->IntermediatePos[i]=LONG_MIN;
        }
        p->ToneFreqVals[i]+=p->Registers[i*2]*(p->NumClocksForSample/p->Registers[i*2]+1);
      } else p->IntermediatePos[i]=LONG_MIN;
    }

    /* Noise channel */
    if (p->ToneFreqVals[3]<=0) {   /* If it gets below 0... */
      p->ToneFreqPos[3]=-p->ToneFreqPos[3]; /* Flip the flip-flop */
      if (p->NoiseFreq!=0x80)      /* If not matching tone2, decrement counter */
        p->ToneFreqVals[3]+=p->NoiseFreq*(p->NumClocksForSample/p->NoiseFreq+1);
      if (p->ToneFreqPos[3]==1) {  /* Only once per cycle... */
        int Feedback;
        if (p->Registers[6]&0x4) { /* White noise */
          /* Calculate parity of fed-back bits for feedback */
          switch (p->WhiteNoiseFeedback) {
            /* Do some optimised calculations for common (known) feedback values */
          case 0x0006:  /* SC-3000    %00000110 */
          case 0x0009:  /* SMS, GG, MD  %00001001 */
            /* If two bits fed back, I can do Feedback=(nsr & fb) && (nsr & fb ^ fb) */
            /* since that's (one or more bits set) && (not all bits set) */
  /* which one?     Feedback=((p->NoiseShiftRegister&p->WhiteNoiseFeedback) && (p->NoiseShiftRegister&p->WhiteNoiseFeedback^p->WhiteNoiseFeedback)); */
            Feedback=((p->NoiseShiftRegister&p->WhiteNoiseFeedback) && ((p->NoiseShiftRegister&p->WhiteNoiseFeedback)^p->WhiteNoiseFeedback));
            break;
          case 0x8005:  /* BBC Micro */
            /* fall through :P can't be bothered to think too much */
          default:    /* Default handler for all other feedback values */
            Feedback=p->NoiseShiftRegister&p->WhiteNoiseFeedback;
            Feedback^=Feedback>>8;
            Feedback^=Feedback>>4;
            Feedback^=Feedback>>2;
            Feedback^=Feedback>>1;
            Feedback&=1;
            break;
          }
        } else    /* Periodic noise */
          Feedback=p->NoiseShiftRegister&1;

        p->NoiseShiftRegister=(p->NoiseShiftRegister>>1) | (Feedback<<15);

  /* Original code: */
  /*      p->NoiseShiftRegister=(p->NoiseShiftRegister>>1) | ((p->Registers[6]&0x4?((p->NoiseShiftRegister&0x9) && (p->NoiseShiftRegister&0x9^0x9)):p->NoiseShiftRegister&1)<<15); */
      }
    }
  }
}
#endif
