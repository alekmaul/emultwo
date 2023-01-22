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
 * accsound_.cpp
 *
 * Lots of parts inspired from EightyOne Sinclair Emulator
 *   https://sourceforge.net/projects/eightyone-sinclair-emulator/
 * Lots of parts inspired from BlueMSX Emulator
 *   https://www.bluemsx.com
 *
 */

//---------------------------------------------------------------------------
#include <windows.h>
#include <mmsystem.h>
//#define DIRECTSOUND_VERSION 0x0500
#include <dsound.h>

#include "tms9928a.h"
#include "ay8910.h"
#include "sn76489.h"

// Global sound variables
#define SND_WHITE   0 // 32768 // 128

int sound_enabled=1;
int sound_freq=44100;
int tickframe=60;

static int sound_framesiz;
static int sound_fillpos;
static short *sound_buf, *sound_buf1, *sound_stream;

unsigned int *smptab;
int smptab_len;
int done_so_far;

// DirectX sound variables
#ifndef DSBLOCK_ENTIREBUFFER
    #define DSBLOCK_ENTIREBUFFER        0x00000002
#endif

DWORD buffer_offset;                    // next position is circular buffer

typedef enum {
    DX_SOUND_DISABLED,
    DX_SOUND_ENABLED,
    DX_SOUND_RUNNING
} dxstate;

typedef struct {
    dxstate state;
    unsigned int bufferOffset;
    unsigned int bufferSize;
    unsigned int fragmentCount;
    unsigned int fragmentSize;
    short bytesPerSample;
    int skipCount;
    LPDIRECTSOUNDBUFFER pbuffer;
    LPDIRECTSOUNDBUFFER buffer;
    LPDIRECTSOUND directSound;
} dxsound;

dxsound dxs;

// -----------------------------------------------------------------------------
// DirectSound Functions
// -----------------------------------------------------------------------------
/*int DSError(int result, char *Message)
{
    if (result)
        MessageBoxA(NULL,
          "Count not initialise DirectSound.\nPlease ensure DirectX 7 or greater is installed",
           Message.c_str(),
           MB_OK);

    return(result);
}
*/
#define DSError(x,y)

//---------------------------------------------------------------------------
static void DSclear(void)
{
        void*   audioBuffer1;
        DWORD  audioSize1;
        void*   audioBuffer2;
        DWORD  audioSize2;
        HRESULT result;

        result = IDirectSoundBuffer_Lock(dxs.buffer, 0, dxs.bufferSize,
                                     &audioBuffer1, &audioSize1, &audioBuffer2, &audioSize2, 0);
        if (result == DSERR_BUFFERLOST)
        {
                IDirectSoundBuffer_Restore(dxs.buffer);
        }
        else
        {
                if (dxs.bytesPerSample == 2)
                {
                        memset(audioBuffer1, 0, audioSize1);
                        if (audioBuffer2)
                                memset(audioBuffer2, 0, audioSize2);
                }
                else
                {
                        memset(audioBuffer1, 0x80, audioSize1);
                        if (audioBuffer2)
                                memset(audioBuffer2, 0x80, audioSize2);
                }
                result = IDirectSoundBuffer_Unlock(dxs.buffer, audioBuffer1, audioSize1,
                                          audioBuffer2, audioSize2);
        }
}

int DSinit( int clock_rate , int ticks_per_frame )
{
        PCMWAVEFORMAT pcmwf;    // waveformat struct
        DSBUFFERDESC dsbd;      // buffer description
        DSCAPS  capabilities;   // device capabilities
        WAVEFORMATEX    wfex;   // wave sound format
        int bufferSize;
        HRESULT  result;

        // Initialize COM
        CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

        // create DirectSound object
        //if ( DSError( CoCreateInstance( &CLSID_DirectSound, NULL,CLSCTX_INPROC_SERVER,&IID_IDirectSound, (void **)&dxs.directSound) != DS_OK )
//                        , "Couldn't create DirectSound object.") ) {
        result = CoCreateInstance(&CLSID_DirectSound, NULL, CLSCTX_INPROC_SERVER, &IID_IDirectSound, (PVOID*)&dxs.directSound);
        if ( result != DS_OK )
        {
                return 0;
        }

        // initialize it
        result = IDirectSound_Initialize(dxs.directSound, NULL);
        //if ( DSError( (IDirectSound_Initialize( lpDS, NULL ) != DS_OK ),
         //               "Couldn't initialize DirectSound." ) ) {
        if ( result != DS_OK )
        {
                return 0;
        }

        // set normal cooperative level
        result = IDirectSound_SetCooperativeLevel(dxs.directSound, GetDesktopWindow(), DSSCL_EXCLUSIVE); // NORMAL or DSSCL_EXCLUSIVE
        if ( result != DS_OK )
        {
                return 0;
        }

        // create wave format description
        memset(&capabilities, 0, sizeof(DSCAPS));
        capabilities.dwSize = sizeof(DSCAPS);
        IDirectSound_GetCaps(dxs.directSound, &capabilities);
/*        if ((capabilities.dwFlags & DSCAPS_PRIMARY16BIT) || (capabilities.dwFlags & DSCAPS_SECONDARY16BIT)) {
                dxs.bytesPerSample = 2;
        }
        else {
        */
                dxs.bytesPerSample = 1;
//        }

        memset(&pcmwf, 0, sizeof(PCMWAVEFORMAT));
        pcmwf.wf.wFormatTag = WAVE_FORMAT_PCM;
        pcmwf.wf.nChannels = 1;
        pcmwf.wf.nSamplesPerSec = clock_rate;
        pcmwf.wBitsPerSample = 8 * dxs.bytesPerSample;
        pcmwf.wf.nBlockAlign = pcmwf.wf.nChannels * dxs.bytesPerSample;
        pcmwf.wf.nAvgBytesPerSec = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;

        memset(&dsbd, 0, sizeof(DSBUFFERDESC));
        dsbd.dwSize = sizeof(DSBUFFERDESC);
        dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;

        bufferSize = /*350*/ (clock_rate*180) / 1000 * pcmwf.wf.nChannels * dxs.bytesPerSample;
        dxs.fragmentSize = 1;
        while (bufferSize / dxs.fragmentSize >= 32 || dxs.fragmentSize < 512) {
                dxs.fragmentSize <<= 1;
        }
        dxs.fragmentCount = 1 + bufferSize / dxs.fragmentSize;
        while (dxs.fragmentCount < 8) {
                dxs.fragmentCount *= 2;
                dxs.fragmentSize /= 2;
        }
        dxs.bufferSize = dxs.fragmentCount * dxs.fragmentSize;

        buffer_offset = 0; /* bytes */
        result = IDirectSound_CreateSoundBuffer(dxs.directSound, &dsbd, &dxs.pbuffer, NULL);
        if (result != DS_OK)
        {
                return 0;
        }

        // Do the second buffer
        memset(&dsbd, 0, sizeof(DSBUFFERDESC));
        dsbd.dwSize = sizeof(DSBUFFERDESC);
        dsbd.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2
                   | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN
                   | DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS ;

        dsbd.dwBufferBytes = dxs.bufferSize;
        dsbd.lpwfxFormat = (LPWAVEFORMATEX)&pcmwf;

        result = IDirectSound_CreateSoundBuffer(dxs.directSound, &dsbd, &dxs.buffer, NULL);
        if (result != DS_OK)
        {
                return 0;
        }

        memset(&wfex, 0, sizeof(WAVEFORMATEX));
        wfex.wFormatTag = WAVE_FORMAT_PCM;
        wfex.nChannels = 1;
        wfex.nSamplesPerSec = clock_rate;
        wfex.wBitsPerSample = 8 * dxs.bytesPerSample;
        wfex.nBlockAlign = wfex.nChannels * dxs.bytesPerSample;
        wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

        result = IDirectSoundBuffer_SetFormat(dxs.pbuffer, &wfex);
        if (result != DS_OK)
        {
                return 0;
        }
        dxs.bufferOffset = dxs.bufferSize;
        DSclear();

        dxs.skipCount = 0;
        dxs.state = DX_SOUND_ENABLED;

        // Let's go...
        result = IDirectSoundBuffer_Play(dxs.buffer, 0, 0, DSBPLAY_LOOPING);
        if (result == DSERR_BUFFERLOST)
        {
//        ui_error("Restoring DirectSound buffer.");
                if ((result = IDirectSoundBuffer_Restore(dxs.buffer)) != DS_OK)
                {
//            ui_error("Cannot restore buffer:\n%s", ds_error(result));
                }
                result = IDirectSoundBuffer_Play(dxs.buffer, 0, 0, DSBPLAY_LOOPING);
        }
        if (result != DS_OK) {
        //ui_error("Cannot play DirectSound buffer:\n%s", ds_error(result));
                return 0;
        }

        return 1;
}

void DSend( void )
{
//        if (DSError( ( IDirectSoundBuffer_Stop( lpDSBuffer ) != DS_OK ), "Couldn't stop sound." ) )
        if (dxs.directSound == NULL)
        {
                return;
        }
        IDirectSoundBuffer_Stop(dxs.buffer);
        IDirectSoundBuffer_Release(dxs.buffer);
        IDirectSound_Release(dxs.directSound);
        dxs.buffer = NULL;
        dxs.directSound = NULL;

        CoUninitialize();
}


static int DScanwrite(dxsound *dxSound, DWORD start, DWORD size)
{
        DWORD readPos;
        DWORD writePos;
        DWORD end = start + size;

        IDirectSoundBuffer_GetCurrentPosition(dxSound->buffer, &readPos, &writePos);
        if (writePos < readPos) writePos += dxSound->bufferSize;
        if (start    < readPos) start    += dxSound->bufferSize;
        if (end      < readPos) end      += dxSound->bufferSize;

        if (start < writePos || end < writePos)
        {
                return (dxSound->bufferSize - (writePos - readPos)) / 2 - dxSound->fragmentSize;
        }

        return 0;
}

static int DSwriteone(dxsound* dxSound, short *buffer, DWORD lockSize)
{
        void*   audioBuffer1;
        DWORD  audioSize1;
        void*   audioBuffer2;
        DWORD  audioSize2;
        HRESULT result;
        DWORD  i;

        do {
                result = IDirectSoundBuffer_Lock(dxSound->buffer, dxSound->bufferOffset,
                                         lockSize,
                                         &audioBuffer1, &audioSize1, &audioBuffer2,
                                         &audioSize2, 0);

                if (result == DSERR_BUFFERLOST)
                {
                        IDirectSoundBuffer_Restore(dxSound->buffer);
                        result = IDirectSoundBuffer_Lock(dxSound->buffer, dxSound->bufferOffset,
                                             lockSize, &audioBuffer1,
                                             &audioSize1,
                                             &audioBuffer2, &audioSize2, 0);
                }
        } while ((audioSize1 + audioSize2) < lockSize);

        if (dxSound->bytesPerSample == 2)
        {
                memcpy(audioBuffer1,buffer,audioSize1);
                if (audioBuffer2)
                        memcpy(audioBuffer2,(BYTE *)buffer + audioSize1, audioSize2);
        }
        else
        {
                short *copyptr = buffer;
                for (i = 0; i < audioSize1; i++)
                {
                        ((BYTE *)audioBuffer1)[i]=(*(copyptr++) >> 8) + 0x80;
                }
                if (audioBuffer2 != NULL)
                {
                        for (i = 0; i < audioSize2; i++)
                        {
                                ((BYTE *)audioBuffer2)[i]=(*(copyptr++) >> 8) + 0x80;
                        }
                }
        }

        result = IDirectSoundBuffer_Unlock(dxSound->buffer, audioBuffer1, audioSize1,
                                       audioBuffer2, audioSize2);
        dxSound->bufferOffset += lockSize;
        dxSound->bufferOffset %= dxSound->bufferSize;

        return 0;
}

// Generate sound for 1 part of frame
void DSframe( short *pbuf, unsigned int nr )
{
        if (dxs.state == DX_SOUND_ENABLED)
        {
                HRESULT result;
                DWORD readPos;
                DWORD writePos;

                IDirectSoundBuffer_GetCurrentPosition(dxs.buffer, &readPos, &writePos);

                dxs.bufferOffset  = (readPos + dxs.bufferSize / 2) % dxs.bufferSize;

                result = IDirectSoundBuffer_Play(dxs.buffer, 0, 0, DSBPLAY_LOOPING);
                if (result == DSERR_BUFFERLOST)
                {
                        result = IDirectSoundBuffer_Play(dxs.buffer, 0, 0, DSBPLAY_LOOPING);
                }
                dxs.state = DX_SOUND_RUNNING;
        }

        if (dxs.state == DX_SOUND_DISABLED)
        {
                return;
        }

        nr *= dxs.bytesPerSample;
        if (dxs.skipCount > 0) {
                dxs.skipCount -= nr;
                return;
        }

        dxs.skipCount = DScanwrite(&dxs, dxs.bufferOffset, nr);
        if (dxs.skipCount > 0) {
                return;
        }

        DSwriteone(&dxs, pbuf, nr);
}

// Pause sound
void DSsuspend(void)
{
        DSclear();

        IDirectSoundBuffer_Stop(dxs.buffer);
        dxs.state = DX_SOUND_DISABLED;
}

// Resume sound
void DSresume(void)
{
        dxs.state = DX_SOUND_ENABLED;
}

// -----------------------------------------------------------------------------
// Common Functions
// -----------------------------------------------------------------------------
int SoundPrepSmpTab(int linesperframe)
{
        int i;

        // Free sample buffer position table if previously allocated
        if(smptab)
        {
                free(smptab);
                smptab = NULL;
        }

        // Redo it regarding number of lines per frame
        smptab_len = linesperframe;
        smptab = malloc(smptab_len * sizeof(int));
        if (smptab==NULL)
        {
                DSend();
                return(0);
        }
        for (i = 0; i < smptab_len; i++)
        {
                double calc = (sound_framesiz * i);
                calc = calc / (double)smptab_len;
                smptab[i] = (int)calc;
        }

        return (1);
}

int SoundInit(int freq, int speed)
{
        int i;
        short *ptr,*ptr1;
        int sound_oldval;

        // Init directx sound
        sound_freq=freq;
        tickframe=speed;
        if (!DSinit(sound_freq,tickframe))
        {
                return(0);
        }

        // here engine is ok
        sound_enabled=1;
        sound_framesiz=sound_freq/tickframe;


        // Prepare Windows stream
        if ((sound_stream=malloc(sound_framesiz*2))==NULL)       // 16 bits
        {
                DSend();
                return(0);
        }

        // PSG buffer
        if ((sound_buf=malloc(sound_framesiz*2))==NULL)       // 16 bits
        {
                DSend();
                return(0);
        }

        // AY buffer
        if ((sound_buf1=malloc(sound_framesiz*2))==NULL)  // 16 bits
        {
                DSend();
                return(0);
        }

        done_so_far = 0;

        // Prepare sample buffer position table with NTSC values
        if (!SoundPrepSmpTab(TMS9918_LINES))
        {
                DSend();
                return(0);
        }

        sound_fillpos=0;

        ptr=sound_buf;
        ptr1=sound_buf1;
        sound_oldval=SND_WHITE;
        for(i=sound_fillpos;i<sound_framesiz;i++)
        {
                *ptr++=sound_oldval;
                *ptr1++=sound_oldval;
        }

        return(1);
}

void SoundEnd(void)
{
        if(sound_enabled)
        {
                if(smptab)
                        free(smptab);
                if(sound_buf)
                        free(sound_buf);
                if(sound_buf1)
                        free(sound_buf1);
                if (sound_stream)
                        free(sound_stream);
                DSend();
                sound_enabled=0;
        }
}

// Generic PSG+AY mixer callback
void sound_mixer_callback(void)
{
        int i;

        // Update stream
        for(i = 0; i < sound_framesiz; i++)
        {
                sound_stream[i] = sound_buf1[i]+sound_buf[i];
        }

        // Send stream to directx sound engine
        DSframe(sound_stream,sound_framesiz);

        // Update sound graphic if displayed
        //SoundVI_Update(sound_buf,sound_buf1,sound_framesiz);
}

void SoundUpdate(unsigned int lineupdate )
{
        short *ptr,*ptr1;
        int f;

        if(!sound_enabled) return;

        ptr=sound_buf+done_so_far;
        ptr1=sound_buf1+done_so_far;

        // Finish buffers at end of frame
        if(lineupdate == (smptab_len - 1) )
        {
                // Generate SN76489 sample data
                sn76489_update(ptr, sound_framesiz - done_so_far);

                // Generate AY sample data
                if (emul2.SGM)
                        ay8910_update(ptr1, sound_framesiz - done_so_far);

                // Mix all voices
                sound_mixer_callback();

                // Reset
                done_so_far = 0;
        }
        else
        {
                unsigned int tinybit;
                if (lineupdate==0) done_so_far=0;
                tinybit = smptab[lineupdate] - done_so_far;

                // Generate SN76489 sample data
                sn76489_update(ptr, tinybit);

                // Generate AY sample data
                if (emul2.SGM)
                        ay8910_update(ptr1,tinybit);

                // Sum total
                done_so_far += tinybit;
        }
}

void SoundSuspend(void)
{
        if (!sound_enabled)
                return;
        DSsuspend();
}

void SoundResume(void)
{
        if (!sound_enabled)
                return;
        DSresume();
}

