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
 * From from EightyOne Sinclair Emulator
 *   https://sourceforge.net/projects/eightyone-sinclair-emulator/
 *
 */

//---------------------------------------------------------------------------
#include <dsound.h>

#include "accsound_.h"

#include "tms9928a.h"
#include "ay8910.h"
#include "sn76489.h"


CSound Sound;

CDSnd::CDSnd(void)
{
    m_ThreadHandle=NULL;
    m_hWnd=NULL;
}

int CDSnd::Initialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels)
{

    m_BitsPerSample=BitsPerSample;
    m_SampleRate=SampleRate;
    m_Channels=Channels;
    m_FPS=FPS;
    m_hWnd=hWnd;


    // If any essentials haven't been initialised, or the audio thread is already running
    if (!m_hWnd || !m_FPS || !m_BitsPerSample || !m_SampleRate || !m_Channels) return(0);
    if (m_ThreadHandle) return(0);

    // Initialise Variables
	ZeroMemory(&m_WFE, sizeof(m_WFE));
	m_lpDS = NULL;
	m_lpDSB = NULL;
	m_pHEvent[0] = CreateEvent(NULL, FALSE, FALSE, "DSound_Buf_Notify_0");
	m_pHEvent[1] = CreateEvent(NULL, FALSE, FALSE, "DSound_Buf_Notify_1");
        m_lpAudioQueue = NULL;

        LPVOID lpvAudio1 = NULL, lpvAudio2 = NULL;
	DWORD dwBytesAudio1, dwBytesAudio2;
	DWORD dwRetSamples, dwRetBytes;

        // Set Sound Output Format - Stereo/22kHz/8Bit

        m_WFE.wFormatTag =  WAVE_FORMAT_PCM;
        m_WFE.nChannels = m_Channels;
        m_WFE.nSamplesPerSec = m_SampleRate;
        m_WFE.wBitsPerSample = m_BitsPerSample;
        m_WFE.nBlockAlign = m_WFE.nChannels;
        m_WFE.nAvgBytesPerSec = m_WFE.nChannels *
                        m_WFE.nSamplesPerSec * m_WFE.wBitsPerSample/8;

        // Calculate Bufferlengths
        // AudioQueue is 2 Seconds long
        // DXbuffer is 2 frames long

        m_DXBufLen = (m_WFE.nBlockAlign * m_WFE.nSamplesPerSec) / FPS;
        m_DXBufLen = (m_DXBufLen & ~1) * 2;
        m_QueueLen = m_DXBufLen * 40;

	//Create DirectSound
        if (DirectSoundCreate(NULL, &m_lpDS, NULL))
                return(-1); // Create DirectSound Failed

	//Set Cooperative Level
        if (m_lpDS->SetCooperativeLevel(m_hWnd, DSSCL_PRIORITY))
                return(-1); // SetCooperativeLevel Failed

	//Create Primary Buffer
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	LPDIRECTSOUNDBUFFER lpDSB = NULL;
        if (m_lpDS->CreateSoundBuffer(&dsbd, &lpDSB, NULL))
                return -1; //Create Primary Sound Buffer Failed

	//Set Primary Buffer Format
        if (lpDSB->SetFormat(&m_WFE))
               return -1; // Set Primary Format Failed!"));

	//Create Secondary Buffer
	dsbd.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS;
	dsbd.dwBufferBytes = m_DXBufLen;
	dsbd.lpwfxFormat = &m_WFE;

        if (m_lpDS->CreateSoundBuffer(&dsbd, &m_lpDSB, NULL))
                return(-1);// Create Second Sound Buffer Failed

	//Query DirectSoundNotify
	LPDIRECTSOUNDNOTIFY lpDSBNotify;

        if (m_lpDSB->QueryInterface(IID_IDirectSoundNotify, (LPVOID *)&lpDSBNotify))
                return -1; // QueryInterface DirectSoundNotify Failed

	//Set Direct Sound Buffer Notify Position
	DSBPOSITIONNOTIFY pPosNotify[2];
	pPosNotify[0].dwOffset = m_DXBufLen / 2;
	pPosNotify[0].hEventNotify = m_pHEvent[0];
	pPosNotify[1].dwOffset = m_DXBufLen - 1;
	pPosNotify[1].hEventNotify = m_pHEvent[1];

        if (lpDSBNotify->SetNotificationPositions(2, pPosNotify))
                return -1; //Set NotificationPosition Failed

	//New audio Queue
	if (m_lpAudioQueue !=NULL)
        {
	       	delete []m_lpAudioQueue; // free (m_lpAudioQueue); //
		m_lpAudioQueue = NULL;
	}

	m_lpAudioQueue = new BYTE[m_QueueLen]; // (char *) malloc (m_QueueLen); //

	//Initialise Audio Queue
        m_QueueStart = 0;
        m_QueueSize = 0;

	memset(m_lpAudioQueue, 0x80, m_QueueLen);

    m_state = DX_SOUND_ENABLED;

    if (m_lpDSB->Lock(0, m_DXBufLen, &lpvAudio1, &dwBytesAudio1, &lpvAudio2, &dwBytesAudio2, 0))
        return -1; // Lock DirectSoundBuffer Failed!

    if (!lpvAudio2)
    {
        memcpy(lpvAudio1, m_lpAudioQueue, dwBytesAudio1);
    }
    else
    {
	    memcpy(lpvAudio1, m_lpAudioQueue, dwBytesAudio1);
	    memcpy(lpvAudio2, m_lpAudioQueue + dwBytesAudio1, dwBytesAudio2);
    }

    // Unlock DirectSoundBuffer
    m_lpDSB->Unlock(lpvAudio1, dwBytesAudio1, lpvAudio2, dwBytesAudio2);

    // Create the Audio Thread
    m_ThreadHandle = CreateThread(0, 0, &CallThread, this, 0, &m_ThreadID);
    if(!m_ThreadHandle) return -1; // Cannot create Audio thread.

    ResumeThread(m_ThreadHandle);    // Start/Resume the Audio Thread
	m_lpDSB->Play(0, 0, DSBPLAY_LOOPING);

    return 0;
}

int CDSnd::Play()
{
    // Ensure Sound is initialised before attempting the play sould
    if (!m_hWnd || !m_FPS || !m_BitsPerSample || !m_SampleRate || !m_Channels)
        return(0);

    ResumeThread(m_ThreadHandle);    // Start/Resume the Audio Thread
	m_lpDSB->Play(0, 0, DSBPLAY_LOOPING);
    return 0;
}

int CDSnd::End()
{
	if (m_lpDSB != NULL)
    {
	    m_lpDSB->Stop();  //Stop Playing Sound

        //Kill the audio thread
        if (m_ThreadHandle) TerminateThread(m_ThreadHandle,0);
        m_ThreadHandle=NULL;

		//Empty the buffer
		LPVOID lpvAudio1 = NULL;
		DWORD dwBytesAudio1 = 0;

		if (m_lpDSB->Lock(0, 0, &lpvAudio1, &dwBytesAudio1, NULL, NULL, DSBLOCK_ENTIREBUFFER))
            return -1; // Lock entirebuffer failed! Stop Failed!

		memset(lpvAudio1, 0, dwBytesAudio1);
		m_lpDSB->Unlock(lpvAudio1, dwBytesAudio1, NULL, NULL);

		//Move the current play position to begin
		m_lpDSB->SetCurrentPosition(0);

        m_QueueStart=0;
        m_QueueSize=0;
	}

	if (m_lpAudioQueue != NULL)
    {
		delete []m_lpAudioQueue;// free(m_lpAudioQueue);//
		m_lpAudioQueue = NULL;
	}

    return 0;
}

// Helper function to get around not being able to
// use class functions as windows callback directly
DWORD WINAPI CDSnd::CallThread(LPVOID Param)
{
    CDSnd *instance = (CDSnd *)Param;

    if(!instance) return -1;

    instance->ThreadFN();
    return(0);
}

// Audio thread function - called every frame
// copy a frame size chunk of audio data from the audio
// queue into the dx sound portion of buffer that isn't
// being used
void CDSnd::ThreadFN()
{
	LPVOID lpvAudio1 = NULL, lpvAudio2 = NULL;
	DWORD dwBytesAudio1, dwBytesAudio2;
	DWORD dwRetSamples, dwRetBytes;

    while(1)
    {
                int i=WaitForMultipleObjects(2, m_pHEvent, FALSE, -1) - WAIT_OBJECT_0;

                switch(i)
                {
                case 0:
                // Lock DirectSoundBuffer Second Part
                if (m_lpDSB->Lock(m_DXBufLen/2, m_DXBufLen/2,
                                &lpvAudio1, &dwBytesAudio1, &lpvAudio2, &dwBytesAudio2, 0))
                        return;  // Lock Buffer Failed

                        break;
                case 1:
                        // Lock DirectSoundBuffer First Part
		        if (m_lpDSB->Lock(0, m_DXBufLen/2, &lpvAudio1, &dwBytesAudio1, &lpvAudio2, &dwBytesAudio2, 0))
                                return; // Lock Buffer Failed

                        break;

                default:  // Timed out so just run the frame without sound
                        break;
                }

                if (i < 2)
                {
	                // Copy AudioQueue to DirectSoundBuffer
                        // Should check for Queue wrap-around, but can't be bothered for now
	                if (lpvAudio2 == NULL)
                        {
                                if (m_QueueStart+dwBytesAudio1 > m_QueueLen)
                                {
                                        // Queue wraps around end queueso copy both parts
                                        int P1Len = m_QueueLen-m_QueueStart;
                                        int P2Len = m_QueueSize-P1Len;
                                        char *P1 = (char *) lpvAudio1;
                                        char *P2 = (char *) lpvAudio1 + P1Len;

                                        memcpy(P1, m_lpAudioQueue + m_QueueStart, P1Len);
                                        memcpy(P2, m_lpAudioQueue, P2Len);

                                }
                                else
                                        memcpy(lpvAudio1, m_lpAudioQueue + m_QueueStart, dwBytesAudio1);
	                }
	                else
                        {       // BUG - doesn't chach for queue wrap around.  However, the
                                // chances of the queue wrapping around at the same time as
                                // receiving a split DX Sound frame seem unlikely, so we'll
                                // live with it for the time being.

		                memcpy(lpvAudio1, m_lpAudioQueue + m_QueueStart, dwBytesAudio1);
		                memcpy(lpvAudio2, m_lpAudioQueue + dwBytesAudio1 + m_QueueStart, dwBytesAudio2);
	                }


                        int delta;

                        delta= m_QueueSize;
                        if (m_QueueSize < dwBytesAudio1) delta = m_QueueSize;
                        else

                        m_QueueSize -= delta;
                        m_QueueStart += delta;
                        if (m_QueueStart >= m_QueueLen)
                                m_QueueStart -= m_QueueLen;    // BUG //

	                // Unlock DirectSoundBuffer
	                m_lpDSB->Unlock(lpvAudio1, dwBytesAudio1, lpvAudio2, dwBytesAudio2);

                }

                ResetEvent(m_pHEvent[0]);
                ResetEvent(m_pHEvent[1]);

                SetLastError(0);
                SendMessage( m_hWnd, WM_USER, NULL, NULL);
                if (GetLastError())
                {
                        char buf[256];
                        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                buf, sizeof(buf), NULL);

                       MessageBox(NULL, buf,"Message Sending Error",2);
                }
        }
}

void CDSnd::Frame( unsigned char *data, unsigned int len )
{
    if (m_state == DX_SOUND_DISABLED)
    {
        return;
    }

    // Copy a sound frame from the emulator into the Sound Queue
    DWORD Pos = m_QueueStart + m_QueueSize;
    if (Pos > m_QueueLen) Pos -= m_QueueLen;
    m_QueueSize += len;
    if (m_QueueSize > m_QueueLen) m_QueueSize = m_QueueLen;

    while(len--)
    {
        m_lpAudioQueue[Pos++] = *data++;
        if( Pos >= m_QueueLen ) Pos -= m_QueueLen;
    }
}

void CDSnd::Suspend(void)
{
    m_state=DX_SOUND_DISABLED;
}

void CDSnd::Resume(void)
{
    m_state = DX_SOUND_ENABLED;
}

// -----------------------------------------------------------------------------
// Common Functions
// -----------------------------------------------------------------------------

// Initialise - msy br called several time with different data from different
// sections of the progrm.  eg, first time it's called we provide the window
// handle, next time we're called we get the fps of the emulated machine
// third time we get the sample rate requested.
// Each time we're called we store the provided information, and only when everything
// needed has been provided do we actually do anything.

int CSound::Initialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels)
{
    // If a parameter is not NULL, store the data
    if (hWnd) m_hWnd=hWnd;
    if (FPS) m_FPS=FPS;
    if (BitsPerSample) m_BitsPerSample=BitsPerSample;
    if (SampleRate) m_SampleRate=SampleRate;
    if (Channels) m_Channels=Channels;

    // If anything is still missing, return
    if (!m_hWnd || !m_FPS || !m_BitsPerSample || !m_SampleRate || !m_Channels) return(0);

    // Start by initialsing DirectSound
    DXSound.Initialise(m_hWnd, m_FPS, m_BitsPerSample, m_SampleRate, m_Channels);

    // get a 1 frame size buffer
    FrameSize=m_SampleRate/m_FPS;

    Buffer = new BYTE[FrameSize*m_Channels];  // (char *) malloc(FrameSize*m_Channels); //
    if(Buffer==NULL)
    {
        DXSound.End();
        return(1); // Oh dear, malloc failed
    }

    Buffersn = new SHORT[FrameSize*m_Channels]; // (short *) malloc(FrameSize*m_Channels); //
    if(Buffersn==NULL)
    {
        DXSound.End();
        return(1); // Oh dear, malloc failed
    }

    Bufferay = new SHORT[FrameSize*m_Channels]; // (short *) malloc(FrameSize*m_Channels); //
    if(Bufferay==NULL)
    {
        DXSound.End();
        return(1); // Oh dear, malloc failed
    }

    // Prepare sample buffer position table with NTSC values
    if (!SoundPrepSmpTab(TMS9918_LINES))
    {
        DXSound.End();
        return(1); // Oh dear, malloc failed
    }

    OldVal=OldValOrig=128;
    OldPos=-1;
    FillPos=0;

    DXSound.Play();
    return(0);
}

// Reinitialise gets called if something changes... we lose the window handle,
// The user changes the sample rats - that kind of thing.
void CSound::ReInitialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels)
{
    if (!hWnd) hWnd = m_hWnd;
    if (!FPS) FPS = m_FPS;
    if (!BitsPerSample) BitsPerSample = m_BitsPerSample;
    if (!SampleRate) SampleRate = m_SampleRate;
    if (!Channels) Channels = m_Channels;

    End();
    Initialise(hWnd,FPS,BitsPerSample,SampleRate,Channels);
}

// End() - free the sound buffer and tell DirectSould it's all over.
void CSound::End(void)
{
    DXSound.End();
    if(Buffer != NULL)
    {
        delete []Buffer; // free(Buffer); //
        Buffer= NULL;
    }
    if(Buffersn != NULL)
    {
        delete []Buffersn; // free(Buffersn); //
        Buffersn=NULL;
    }
    if(Bufferay != NULL)
    {
        delete []Bufferay; // free(Bufferay); //
        Bufferay=NULL;
    }
    if(smptab != NULL)
    {
        delete []smptab; // free(smptab); //
        smptab=NULL;
    }
}

void CSound::SoundSuspend(void)
{
    memset(Buffer,0x00,FrameSize);
    DXSound.Frame(Buffer, FrameSize);
    DXSound.Suspend();
}

void CSound::SoundResume(void)
{
    DXSound.Resume();
}

void CSound::Frame(unsigned int lineupdate )
{
    short *ptr, *ptr1;

    ptr=(short *) Buffersn+FillPos;
    ptr1=(short *) Bufferay+FillPos;

    // Finish buffers at end of frame
    if(lineupdate == (smptab_len - 1) )
    {
        // Generate SN76489 sample data
        sn76489_update(ptr, FrameSize - FillPos);

        // Generate AY sample data
        if (emul2.SGM)
            ay8910_update(ptr1, FrameSize - FillPos);

        // Mix all voices
        for (int i=0;i<FrameSize;i++)
        {
            Buffer[i]=(Buffersn[i]+Bufferay[i])>>8;
        }
        DXSound.Frame(Buffer, FrameSize);

        // Reset
        FillPos = 0;
    }
    else
    {
        unsigned int tinybit;
        if (lineupdate==0) FillPos=0;
        tinybit = smptab[lineupdate] - FillPos;

        // Generate SN76489 sample data
        sn76489_update(ptr, tinybit);

        // Generate AY sample data
        if (emul2.SGM)
            ay8910_update(ptr1,tinybit);

        // Sum total
        FillPos += tinybit;
    }
}

int CSound::SoundPrepSmpTab(int linesperframe)
{
    int i;

    // Free sample buffer position table if previously allocated
    if(smptab!=NULL)
    {
        delete []smptab;
        smptab=NULL;
    }

    // Redo it regarding number of lines per frame
    smptab_len = linesperframe;
    smptab = new INT[smptab_len]; // (int *) malloc(smptab_len); //
    if (smptab==NULL)
    {
        DXSound.End();
        return(0);
    }
    for (i = 0; i < smptab_len; i++)
    {
        double calc = (FrameSize * i);
        calc = calc / (double)smptab_len;
        smptab[i] = (int)calc;
    }

    return (1);
}

