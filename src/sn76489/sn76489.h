/* EmulTwo  - A Windows Colecovision emulator.
 * Copyright (C) 2018-2019 Alekmaul
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
 * sn76489.h
 *
 */
#ifndef _SN76489_H
#define _SN76489_H

#if 1
// --------------------------------------------------------------------------------------------
// The SN76489 has 8 "registers":
// 4 x 4 bit volume registers,
// 3 x 10 bit tone registers and
// 1 x 3 bit noise register.
// --------------------------------------------------------------------------------------------
#define MAX_OUTPUT     	0x7fff
#define SNSTEP          0x8000 //0x10000

#define	FB_WNOISE      	0x12000
#define FB_PNOISE      	0x08000

#define NG_PRESET      	0x8000

typedef struct  {
    int SampleRate;
    int UpdateStep;
    int VolTable[16];
    int Register[8];
    int Volume[4];
    int Period[4];
    int Count[4];
    int Output[4];
    int LastRegister;
    int RNG;
} sn76496;

#else
/*
    More testing is needed to find and confirm feedback patterns for
    SN76489 variants and compatible chips.
*/
enum feedback_patterns {
    FB_BBCMICRO =   0x8005, /* Texas Instruments TMS SN76489N (original) from BBC Micro computer */
    FB_SC3000   =   0x0006, /* Texas Instruments TMS SN76489AN (rev. A) from SC-3000H computer */
    FB_SEGAVDP  =   0x0009, /* SN76489 clone in Sega's VDP chips (315-5124, 315-5246, 315-5313, Game Gear) */
};

enum volume_modes {
    VOL_TRUNC   =   0,      /* Volume levels 13-15 are identical */
    VOL_FULL    =   1,      /* Volume levels 13-15 are unique */
};

enum boost_modes {
    BOOST_OFF   =   0,      /* Regular noise channel volume */
    BOOST_ON    =   1,      /* Doubled noise channel volume */
};

enum mute_values {
    MUTE_ALLOFF =   0,      /* All channels muted */
    MUTE_TONE1  =   1,      /* Tone 1 mute control */
    MUTE_TONE2  =   2,      /* Tone 2 mute control */
    MUTE_TONE3  =   4,      /* Tone 3 mute control */
    MUTE_NOISE  =   8,      /* Noise mute control */
    MUTE_ALLON  =   15,     /* All channels enabled */
};

typedef struct
{
    // expose this for inspection/modification for channel muting 
    int Mute;
    int BoostNoise;
    int VolumeArray;
    
    /* Variables */
    float Clock;
    float dClock;
    int PSGStereo;
    int NumClocksForSample;
    int WhiteNoiseFeedback;
    
    /* PSG registers: */
    unsigned short Registers[8];        /* Tone, vol x4 */
    int LatchedRegister;
    unsigned short NoiseShiftRegister;
    short NoiseFreq;            /* Noise channel signal generator frequency */

    /* Output calculation variables */
    short ToneFreqVals[4];      /* Frequency register values (counters) */
    char ToneFreqPos[4];        /* Frequency channel flip-flops */
    short Channels[4];          /* Value of each channel, before stereo is applied */
    int IntermediatePos[4];   /* intermediate values used at boundaries between + and - */

} sn76496;
#endif

#ifdef __cplusplus
extern "C" sn76496 sn;
extern "C" void sn76489_update(short *buffer, unsigned int length);
extern "C" void sn76489_write(int data);
extern "C" void sn76489_reset(int clock, int sample_rate);
extern "C" void sn76489_init(int clock, int sample_rate);
extern "C" void sn76489_config(int mute, int boost, int volume, int feedback);
#else
extern sn76496 sn;
extern void sn76489_update(short *buffer, unsigned int length);
extern void sn76489_write(int data);
extern void sn76489_reset(int clock, int sample_rate);
extern void sn76489_init(int clock, int sample_rate);
extern void sn76489_config(int mute, int boost, int volume, int feedback);
#endif

#endif

