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
 * kbstatus.h
 *
 */

 //---------------------------------------------------------------------------

#ifndef kbstatusH
#define kbstatusH

#include <IniFiles.hpp>

#define NBKEYCV 20

// DirectInput joystick
#define   JOY_AXIS_UP       0x00000001
#define   JOY_AXIS_DOWN     0x00000002
#define   JOY_AXIS_LEFT     0x00000004
#define   JOY_AXIS_RIGHT    0x00000008
#define   JOY_PAD_UP        0x00000010
#define   JOY_PAD_DOWN      0x00000020
#define   JOY_PAD_LEFT      0x00000040
#define   JOY_PAD_RIGHT     0x00000080
#define   JOY_BUTTON_1      0x00000100
#define   JOY_BUTTON_2      0x00000200
#define   JOY_BUTTON_3      0x00000400
#define   JOY_BUTTON_4      0x00000800
#define   JOY_BUTTON_5      0x00001000
#define   JOY_BUTTON_6      0x00002000
#define   JOY_BUTTON_7      0x00004000
#define   JOY_BUTTON_8      0x00008000
#define   JOY_BUTTON_9      0x00010000
#define   JOY_BUTTON_10     0x00020000
#define   JOY_BUTTON_11     0x00040000
#define   JOY_BUTTON_12     0x00080000

// Joystick() Result Bits
#define JST_NONE      0x0000
#define JST_KEYPAD    0x000F
#define JST_UP        0x0100
#define JST_RIGHT     0x0200
#define JST_DOWN      0x0400
#define JST_LEFT      0x0800
#define JST_FIRER     0x0040
#define JST_FIREL     0x4000
#define JST_0         0x0005
#define JST_1         0x0002
#define JST_2         0x0008
#define JST_3         0x0003
#define JST_4         0x000D
#define JST_5         0x000C
#define JST_6         0x0001
#define JST_7         0x000A
#define JST_8         0x000E
#define JST_9         0x0004
#define JST_STAR      0x0006
#define JST_HASH      0x0009
#define JST_PURPLE    0x0007
#define JST_BLUE      0x000B
#define JST_RED       JST_FIRER
#define JST_YELLOW    JST_FIREL

extern unsigned short  JoyP1[NBKEYCV],JoyP2[NBKEYCV];

//---------------------------------------------------------------------------
extern void KeysRead(TIniFile *ini);
extern void KeysWrite(TIniFile *ini);
extern void CheckKeyDown(WORD key);
extern void CheckKeyUp(WORD key);

extern void CheckJoyMove(TMessage &msg);
extern void CheckJoyDown(TMessage &msg);
extern void CheckJoyUp(TMessage &msg);

extern void JoystickInit(HWND hWnd, HINSTANCE hInst);
extern void JoystickEnd(void);

extern void CheckMouseMove(int X, int Y);
extern void KeybJoyUpdate(void);
extern void RCUpdate(void);

#endif
