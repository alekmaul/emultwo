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
 * accsound.h
 *
 */

//---------------------------------------------------------------------------

#ifndef accsound_H
#define accsound_H

#include <windows.h>
#include <mmreg.h>
#include <dsound.h>

typedef enum {
    DX_SOUND_DISABLED,
    DX_SOUND_ENABLED,
    DX_SOUND_RUNNING
} CDstate;

class CDSnd
{
public:
    CDSnd();
	int Initialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels);
    int Play();
	int End();

    void Suspend(void);
    void Resume(void);

    void Frame(unsigned char *data, unsigned int len);
private:
    void ThreadFN(void);
    static DWORD WINAPI CallThread(LPVOID Param);

    HWND m_hWnd;
    int m_BitsPerSample;
    int m_SampleRate;
    int m_Channels;
    int m_FPS;

    DWORD m_ThreadID;
    HANDLE m_ThreadHandle;

    CDstate m_state;

    // DirectSound
	WAVEFORMATEX m_WFE;
	LPDIRECTSOUND m_lpDS;
	LPDIRECTSOUNDBUFFER m_lpDSB;
	HANDLE m_pHEvent[2];
    DWORD m_DXBufLen;

	// Audio Buffer
	// LPGETAUDIOSAMPLES_PROGRESS m_lpGETAUDIOSAMPLES;
	LPBYTE m_lpAudioQueue;
    DWORD m_QueueLen;
    DWORD m_QueueSize;
    DWORD m_QueueStart;
};

class CSound
{
public:
    int Initialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels);
    void ReInitialise(HWND hWnd, int FPS, int BitsPerSample, int SampleRate, int Channels);
    int SoundPrepSmpTab(int linesperframe);

	void End(void);
	void Frame(unsigned int lineupdate );

    void SoundSuspend(void);
    void SoundResume(void);

    int VolumeLevel[4];
private:
    CDSnd DXSound;

    HWND m_hWnd;
    int m_BitsPerSample;
    int m_SampleRate;
    int m_Channels;
    int m_FPS;

    int FrameSize;
    int FramesPerSecond;

    int smptab_len;

    unsigned char *Buffer;
    short *Buffersn, *Bufferay;
    int *smptab;
    int OldPos,FillPos,OldVal,OldValOrig;
};

extern CSound Sound;

#endif

