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

//---------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" int SoundPrepSmpTab(int linesperframe);
extern "C" int SoundInit(int freq, int speed);
extern "C" void SoundEnd(void);
extern "C" void SoundUpdate(unsigned int lineupdate );
extern "C" void SoundSuspend(void);
extern "C" void SoundResume(void);
#else
extern int SoundPrepSmpTab(int linesperframe);
extern int SoundInit(int freq, int speed);
extern void SoundEnd(void);
extern void SoundUpdate(unsigned int lineupdate );
extern void SoundSuspend(void);
extern void SoundResume(void);

#endif

#endif

