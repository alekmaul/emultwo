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
 * kbstatus_.cpp
 *
 */

/*
    EXTRA CONTROLLERS INFO:
    -Driving Controller (Expansion Module #2). It consist of a steering wheel and
    a gas pedal. Only one can be used on a real ColecoVision. The gas pedal is not
    analog, internally it is just a switch.
    On a real ColecoVision, when the Driving Controller is enabled, the controller 1
    do not work because have been replaced by the Driving Controller, and controller 2
    have to be used to start game, gear shift, etc.
    Driving Controller is just a spinner on controller 1 socket similar to the one
    on Roller Controller and Super Action Controllers so you can use Roller Controller
    or Super Action Controllers to play games requiring Driving Controller.

    -Roller Controller. Basically a trackball with four buttons (the two fire buttons
    from player 1 and the two fire buttons from player 2). Only one Roller Controller
    can be used on a real ColecoVision.
    Roller Controller is connected to both controller sockets and both controllers
    are connected to the Roller Controller, it uses the spinner pins of both sockets
    to generate the X and Y signals (X from controller 1 and the Y from controller 2)

    -Super Action Controllers. It is a hand controller with a keypad, four buttons
    (the two from the player pad and two more), and a spinner. This was made primarily
    for two player sport games, but will work for every other ColecoVision game.
*/

//---------------------------------------------------------------------------
#include <dinput.h>
#include <mmsystem.h>

#include <vcl.h>
#pragma hdrstop

#include "kbstatus_.h"
#include "main_.h"

#include "adamnet.h"
#include "coleco.h"
#include "z80.h"

#define NR_KEYS 0x143

static const byte scan2ascii[NR_KEYS] =
{
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,	/* 00 */
	0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,	/* 10 */
	0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,	/* 20 */
	0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,	/* 30 */
	0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,	/* 40 */
	0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,	/* 50 */
	0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
	0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,	/* 60 */
	0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
	0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,	/* 70 */
	0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E, 151,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* 80 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* 90 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* A0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* B0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* C0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* D0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* E0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* F0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	 '0', '1', '2', '3', '4', '5', '6', '7',	/* 100 */
	 '8', '9', '.', '/', '*', '-', '+',0x0D,
	0x00, 160, 162, 161, 163, 148, 146, 147,	/* 110 */
	 150, 149, 129, 130, 131, 132, 133, 134,
	 144, 145,0x00,0x00,0x00,0x00,0x00,0x00,	/* 120 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* 130 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,    							/* 140 */
};

static const byte scan2ascii_shift[NR_KEYS] =
{
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,	/* 00 */
	0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,	/* 10 */
	0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,  34,	/* 20 */
	0x28,0x29,0x2A,0x2B, '<', '_', '>', '?',
	 ')', '!', '@', '#', '$', '%', '^', '&',	/* 30 */
	 '*', '(',0x3A, ':',0x3C, '+',0x3E,0x3F,
	0x40, 'a', 'b', 'c', 'd', 'e', 'f', 'g',	/* 40 */
	 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
	 'p', 'q', 'r', 's', 't', 'u', 'v', 'w',	/* 50 */
	 'x', 'y', 'z', '{', '|', '}',0x5E,0x5F,
	 '~', 'A', 'B', 'C', 'D', 'E', 'F', 'G',	/* 60 */
	 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
	 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',	/* 70 */
	 'X', 'Y', 'Z',0x7B,0x7C,0x7D,0x7E, 159,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* 80 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* 90 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* A0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* B0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* C0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* D0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* E0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* F0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* FF00 */
	 184, 185, '.', '/', '*', '-', '+',0x0D,
	0x00,0x00,0x00,0x00,0x00, 156, 154, 155,	/* FF10 */
	158, 157, 137, 138, 139, 140, 141, 142,
	152, 153,0x00,0x00,0x00,0x00,0x00,0x00,	/* FF20 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* FF30 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00     						    /* FF40 */
};

static const byte scan2ascii_ctrl[NR_KEYS] =
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* 00 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* 10 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* 20 */
	0x00,0x00,0x00,0x00,0x00,  31,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,  30,0x00,	/* 30 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,   1,   2,   3,   4,   5,   6,   7,	/* 40 */
	   8,   9,  10,  11,  12,  13,  14,  15,
	  16,  17,  18,  19,  20,  21,  22,  23,	/* 50 */
	  24,  25,  26,  27,  28,  29,  30,  31,
	0x00,   1,   2,   3,   4,   5,   6,   7,	/* 60 */
	   8,   9,  10,  11,  12,  13,  14,  15,
	  16,  17,  18,  19,  20,  21,  22,  23,	/* 70 */
	  24,  25,  26,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* 80 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* 90 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* A0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* B0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* C0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* D0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* E0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* F0 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* FF00 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00, 164, 166, 165, 167,0x00,0x00,0x00,	/* FF10 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* FF20 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	/* FF30 */
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00     							/* FF40 */
};

//---------------------------------------------------------------------------
JOYINFO JoyInfo;
JOYCAPS JoyCaps;
unsigned short JoystickState;

int mouse_xpos=0, mouse_ypos=0;

unsigned short JoyP1[NBKEYCV],JoyP2[NBKEYCV];
unsigned int key_shift=0;

const unsigned short keyCoresp[NBKEYCV] = {
      JST_UP,
      JST_DOWN,
      JST_LEFT,
      JST_RIGHT,

      JST_FIREL,
      JST_FIRER,

      JST_HASH,JST_STAR,
      JST_0, JST_1, JST_2, JST_3, JST_4 , JST_5, JST_6, JST_7, JST_8, JST_9,
      JST_PURPLE,JST_BLUE, // Purple , Blue (red and yellow = butr and butl)
};

//---------------------------------------------------------------------------

#pragma package(smart_init)

void KeysRead(TIniFile *ini)
{
    // Get player one config
    JoyP1[0] = ini->ReadInteger("JOYCFG","J1UP",VK_UP);
    JoyP1[1] = ini->ReadInteger("JOYCFG","J1DOWN",VK_DOWN);
    JoyP1[2] = ini->ReadInteger("JOYCFG","J1LEFT",VK_LEFT);
    JoyP1[3] = ini->ReadInteger("JOYCFG","J1RIGHT",VK_RIGHT);

    JoyP1[4] = ini->ReadInteger("JOYCFG","J1BUTL",0x41);      // Default is A
    JoyP1[5] = ini->ReadInteger("JOYCFG","J1BUTR",0x5A);      // Default is Z

    JoyP1[6] = ini->ReadInteger("JOYCFG","J1BUTSHA",VK_DIVIDE);    // #
    JoyP1[7] = ini->ReadInteger("JOYCFG","J1BUTSTA",VK_MULTIPLY);  // *

    JoyP1[8] = ini->ReadInteger("JOYCFG","J1BUT0",VK_NUMPAD0);     // 0
    JoyP1[9] = ini->ReadInteger("JOYCFG","J1BUT1",VK_NUMPAD1);     // 1
    JoyP1[10] = ini->ReadInteger("JOYCFG","J1BUT2",VK_NUMPAD2);    // 2
    JoyP1[11] = ini->ReadInteger("JOYCFG","J1BUT3",VK_NUMPAD3);    // 3
    JoyP1[12] = ini->ReadInteger("JOYCFG","J1BUT4",VK_NUMPAD4);    // 4
    JoyP1[13] = ini->ReadInteger("JOYCFG","J1BUT5",VK_NUMPAD5);    // 5
    JoyP1[14] = ini->ReadInteger("JOYCFG","J1BUT6",VK_NUMPAD6);    // 6
    JoyP1[15] = ini->ReadInteger("JOYCFG","J1BUT7",VK_NUMPAD7);    // 7
    JoyP1[16] = ini->ReadInteger("JOYCFG","J1BUT8",VK_NUMPAD8);    // 8
    JoyP1[17] = ini->ReadInteger("JOYCFG","J1BUT9",VK_NUMPAD9);    // 9

    JoyP1[18] = ini->ReadInteger("JOYCFG","J1BUTPUR",0x45);        // E (for Purple , Blue (red and yellow = butr and butl) )
    JoyP1[19] = ini->ReadInteger("JOYCFG","J1BUTBLU",0x52);        // R

    // Get player two config
    JoyP2[0] = ini->ReadInteger("JOYCFG","J2UP",VK_UP);
    JoyP2[1] = ini->ReadInteger("JOYCFG","J2DOWN",VK_DOWN);
    JoyP2[2] = ini->ReadInteger("JOYCFG","J2LEFT",VK_LEFT);
    JoyP2[3] = ini->ReadInteger("JOYCFG","J2RIGHT",VK_RIGHT);

    JoyP2[4] = ini->ReadInteger("JOYCFG","J2BUTL",0x41);      // Default is A
    JoyP2[5] = ini->ReadInteger("JOYCFG","J2BUTR",0x5A);      // Default is Z

    JoyP2[6] = ini->ReadInteger("JOYCFG","J2BUTSHA",VK_DIVIDE);    // #
    JoyP2[7] = ini->ReadInteger("JOYCFG","J2BUTSTA",VK_MULTIPLY);  // *

    JoyP2[8] = ini->ReadInteger("JOYCFG","J2BUT0",VK_NUMPAD0);     // 0
    JoyP2[9] = ini->ReadInteger("JOYCFG","J2BUT1",VK_NUMPAD1);     // 1
    JoyP2[10] = ini->ReadInteger("JOYCFG","J2BUT2",VK_NUMPAD2);    // 2
    JoyP2[11] = ini->ReadInteger("JOYCFG","J2BUT3",VK_NUMPAD3);    // 3
    JoyP2[12] = ini->ReadInteger("JOYCFG","J2BUT4",VK_NUMPAD4);    // 4
    JoyP2[13] = ini->ReadInteger("JOYCFG","J2BUT5",VK_NUMPAD5);    // 5
    JoyP2[14] = ini->ReadInteger("JOYCFG","J2BUT6",VK_NUMPAD6);    // 6
    JoyP2[15] = ini->ReadInteger("JOYCFG","J2BUT7",VK_NUMPAD7);    // 7
    JoyP2[16] = ini->ReadInteger("JOYCFG","J2BUT8",VK_NUMPAD8);    // 8
    JoyP2[17] = ini->ReadInteger("JOYCFG","J2BUT9",VK_NUMPAD9);    // 9

    JoyP2[18] = ini->ReadInteger("JOYCFG","J2BUTPUR",0x45);        // E (for Purple , Blue (red and yellow = butr and butl) )
    JoyP2[19] = ini->ReadInteger("JOYCFG","J2BUTBLU",0x52);        // R
}


void KeysWrite(TIniFile *ini)
{
    // Write player one config
    ini->WriteInteger("JOYCFG","J1UP",JoyP1[0]);
    ini->WriteInteger("JOYCFG","J1DOWN",JoyP1[1]);
    ini->WriteInteger("JOYCFG","J1LEFT",JoyP1[2]);
    ini->WriteInteger("JOYCFG","J1RIGHT",JoyP1[3]);

    ini->WriteInteger("JOYCFG","J1BUTL",JoyP1[4]);      // Default is A
    ini->WriteInteger("JOYCFG","J1BUTR",JoyP1[5]);      // Default is Z

    ini->WriteInteger("JOYCFG","J1BUTSHA",JoyP1[6]);    // #
    ini->WriteInteger("JOYCFG","J1BUTSTA",JoyP1[7]);  // *

    ini->WriteInteger("JOYCFG","J1BUT0",JoyP1[8]);     // 0
    ini->WriteInteger("JOYCFG","J1BUT1",JoyP1[9]);     // 1
    ini->WriteInteger("JOYCFG","J1BUT2",JoyP1[10]);    // 2
    ini->WriteInteger("JOYCFG","J1BUT3",JoyP1[11]);    // 3
    ini->WriteInteger("JOYCFG","J1BUT4",JoyP1[12]);    // 4
    ini->WriteInteger("JOYCFG","J1BUT5",JoyP1[13]);    // 5
    ini->WriteInteger("JOYCFG","J1BUT6",JoyP1[14]);    // 6
    ini->WriteInteger("JOYCFG","J1BUT7",JoyP1[15]);    // 7
    ini->WriteInteger("JOYCFG","J1BUT8",JoyP1[16]);    // 8
    ini->WriteInteger("JOYCFG","J1BUT9",JoyP1[17]);    // 9

    ini->WriteInteger("JOYCFG","J1BUTPUR",JoyP1[18]);        // E (for Purple , Blue (red and yellow = butr and butl) )
    ini->WriteInteger("JOYCFG","J1BUTBLU",JoyP1[19]);        // R

    // Writeplayer two config
    ini->WriteInteger("JOYCFG","J2UP",JoyP2[0]);
    ini->WriteInteger("JOYCFG","J2DOWN",JoyP2[1]);
    ini->WriteInteger("JOYCFG","J2LEFT",JoyP2[2]);
    ini->WriteInteger("JOYCFG","J2RIGHT",JoyP2[3]);

    ini->WriteInteger("JOYCFG","J2BUTL",JoyP2[4]);      // Default is A
    ini->WriteInteger("JOYCFG","J2BUTR",JoyP2[5]);      // Default is Z

    ini->WriteInteger("JOYCFG","J2BUTSHA",JoyP2[6]);    // #
    ini->WriteInteger("JOYCFG","J2BUTSTA",JoyP2[7]);  // *

    ini->WriteInteger("JOYCFG","J2BUT0",JoyP2[8]);     // 0
    ini->WriteInteger("JOYCFG","J2BUT1",JoyP2[9]);     // 1
    ini->WriteInteger("JOYCFG","J2BUT2",JoyP2[10]);    // 2
    ini->WriteInteger("JOYCFG","J2BUT3",JoyP2[11]);    // 3
    ini->WriteInteger("JOYCFG","J2BUT4",JoyP2[12]);    // 4
    ini->WriteInteger("JOYCFG","J2BUT5",JoyP2[13]);    // 5
    ini->WriteInteger("JOYCFG","J2BUT6",JoyP2[14]);    // 6
    ini->WriteInteger("JOYCFG","J2BUT7",JoyP2[15]);    // 7
    ini->WriteInteger("JOYCFG","J2BUT8",JoyP2[16]);    // 8
    ini->WriteInteger("JOYCFG","J2BUT9",JoyP2[17]);    // 9

    ini->WriteInteger("JOYCFG","J2BUTPUR",JoyP2[18]);        // E (for Purple , Blue (red and yellow = butr and butl) )
    ini->WriteInteger("JOYCFG","J2BUTBLU",JoyP2[19]);        // R
}
//---------------------------------------------------------------------------

void CheckKeyDown(WORD key)
{
        int i,  adamkey=0;
        unsigned int valtche;

        // Check joystick for player one & two (16 bits shifted)
        for (i=0;i<NBKEYCV;i++)
        {
                if (key == JoyP1[i]) { coleco_joystat |= keyCoresp[i]; }
                if (key == JoyP2[i]) { coleco_joystat |= (keyCoresp[i]<<16); }
        }

        // Lock out opposite direction keys (Grog's Revenge)
        if(coleco_joystat&JST_RIGHT)       coleco_joystat&=~JST_LEFT;
        if(coleco_joystat&JST_DOWN)        coleco_joystat&=~JST_UP;
        if(coleco_joystat&(JST_RIGHT<<16)) coleco_joystat&=~(JST_LEFT<<16);
        if(coleco_joystat&(JST_DOWN<<16))  coleco_joystat&=~(JST_UP<<16);

        // Check now keyboard if we are in ADAM mode
        if (emul2.machine==MACHINEADAM)
        {
                if (key==VK_SHIFT)
                {
                        key_shift=1;
                }
                else
                switch (key)
                {
                        case VK_F1 : adamkey=KEY_F1; break;
                        case VK_F2 : adamkey=KEY_F2; break;
                        case VK_F3 : adamkey=KEY_F3; break;
                        case VK_F4 : adamkey=KEY_F4; break;
                        case VK_F5 : adamkey=KEY_F5; break;
                        case VK_F6 : adamkey=KEY_F6; break;
                        case VK_UP: adamkey=KEY_UP; break;
                        case VK_DOWN: adamkey=KEY_DOWN; break;
                        case VK_LEFT: adamkey=KEY_LEFT; break;
                        case VK_RIGHT: adamkey=KEY_RIGHT; break;
                        case VK_TAB: adamkey=KEY_TAB; break;
                        case VK_BACK: adamkey=KEY_BS; break;
                        case VK_INSERT: adamkey=KEY_INS; break;
                        case VK_DELETE: adamkey=KEY_DEL; break;
                        case VK_PRINT : adamkey=KEY_PRINT; break;
                        case VK_ESCAPE : adamkey=KEY_ESC; break;
                        default:
                                // Assume any key in 0x00..0x7E range an ASCII code
                                if(key<='~') adamkey=key;
                                break;
                }
                if (adamkey)
                        PutKBD(adamkey | ( (key_shift) ? CON_SHIFT:0) );
        }
}
//---------------------------------------------------------------------------

void CheckKeyUp(WORD key)
{
        int i;

        // Check for player one & two (16 bits shifted)
        for (i=0;i<NBKEYCV;i++)
        {
                if (key == JoyP1[i]) { coleco_joystat &= ~keyCoresp[i]; }
                if (key == JoyP2[i]) { coleco_joystat &= ~(keyCoresp[i]<<16); }
        }

        // Check now keyboard if we are in ADAM mode
        if (emul2.machine==MACHINEADAM)
        {
                if (key==VK_SHIFT)
                {
                        key_shift=0;
                }
        }
}
//---------------------------------------------------------------------------

void CheckJoyMove(TMessage &msg)
{
        int PosX,PosY;

        PosX=msg.LParamLo;
        PosY=msg.LParamHi;
        JoystickState =0;
        if (PosX<10000) JoystickState |= JOY_PAD_LEFT;
        else if (PosX>55000) JoystickState |= JOY_PAD_RIGHT;
        if (PosY<10000) JoystickState |= JOY_PAD_UP;
        else if (PosY>55000) JoystickState |= JOY_PAD_DOWN;
}

//---------------------------------------------------------------------------

void CheckJoyDown(TMessage &msg)
{
        // Acquire buttons
        if (msg.WParam & JOY_BUTTON1) JoystickState |= JOY_BUTTON_1;
        if (msg.WParam & JOY_BUTTON2) JoystickState |= JOY_BUTTON_2;
        if (msg.WParam & JOY_BUTTON3) JoystickState |= JOY_BUTTON_3;
        if (msg.WParam & JOY_BUTTON4) JoystickState |= JOY_BUTTON_4;
}
//---------------------------------------------------------------------------

void CheckJoyUp(TMessage &msg)
{
    // Acquire buttons
     if (msg.WParam & JOY_BUTTON1) JoystickState &= ~JOY_BUTTON_1;
     if (msg.WParam & JOY_BUTTON2) JoystickState &= ~JOY_BUTTON_2;
     if (msg.WParam & JOY_BUTTON3) JoystickState &= ~JOY_BUTTON_3;
     if (msg.WParam & JOY_BUTTON4) JoystickState &= ~JOY_BUTTON_4;
}
//---------------------------------------------------------------------------

void JoystickInit(HWND hWnd, HINSTANCE hInst)
{
    memset((void *) &JoyCaps,0x00,sizeof(JOYCAPS));
    JoystickState=0;

    // Do we support joysticks ?
    if (joyGetNumDevs()==0){
        return;
    }

    // OK, we have support, do we have joystick connected ?
    if (joyGetPos(JOYSTICKID1,&JoyInfo)!=JOYERR_NOERROR){
        return;
    }

    // Get joystick capabilities
    joyGetDevCaps(JOYSTICKID1,&JoyCaps,sizeof(JOYCAPS));
    joySetCapture(hWnd,JOYSTICKID1,2*JoyCaps.wPeriodMin,false);
}
//---------------------------------------------------------------------------

void JoystickEnd(void)
{
        // Disconnect joystick
        if (JoyCaps.wMid != 0) {
                joyReleaseCapture(JOYSTICKID1);
        }
}
//---------------------------------------------------------------------------

void CheckMouseMove(int X, int Y)
{
    int x,y;
    float ScaleW, ScaleH;
    //AnsiString text="";

    // Get mouse positions and scale them
    ScaleW=Form1->ClientWidth/Form1->BaseWidth; ScaleH=((Form1->ClientHeight-Form1->StatusBar1->Height)/Form1->BaseHeight);
    x=(X/ScaleW); y=(Y/ScaleH);
    x = ((x*1024)/Form1->BaseWidth-512);
    y = ((y*1024)/Form1->BaseHeight-512);

    // IF driving module, no Y value
    if (emul2.steerwheel)
        y=0;

    // emulate Super Action & Driving module values
    coleco_spinpos[0] = x<-512? -512:x>512? 512:x;
    coleco_spinparam[0]  = coleco_spinpos[0]>=0? (coleco_spinpos[0]>32? coleco_spinpos[0]:0):(coleco_spinpos[0]<-32? -coleco_spinpos[0]:0);
    coleco_spinstate[0] = coleco_spinpos[0]>0? 0x00003000:coleco_spinpos[0]<0? 0x00001000:0;
    coleco_spinpos[1] = x<-512? -512:x>512? 512:x;
    coleco_spinparam[1]  = coleco_spinpos[1]>=0? (coleco_spinpos[1]>32? coleco_spinpos[1]:0):(coleco_spinpos[1]<-32? -coleco_spinpos[1]:0);
    coleco_spinstate[1] = coleco_spinpos[1]>0? 0x00003000:coleco_spinpos[1]<0? 0x00001000:0;

    //Form1->StatusBar1->Panels->Items[3]->Text = IntToStr((int) coleco_spinpos[0])+" " +IntToStr((int) coleco_spinparam[0]);
}

//---------------------------------------------------------------------------

void KeybJoyUpdate(void)
{
    // Get joystick values if possible
    if ((JoyCaps.wMid != 0) && JoystickState)
    {
        if (JoystickState & JOY_BUTTON_1) coleco_joystat |=  keyCoresp[4]; // but #1
        if (JoystickState & JOY_BUTTON_2) coleco_joystat |=  keyCoresp[5]; // but #2
        if (JoystickState & JOY_BUTTON_3) coleco_joystat |=  keyCoresp[6]; // but ##
        if (JoystickState & JOY_BUTTON_4) coleco_joystat |=  keyCoresp[7]; // but #*

        if (JoystickState & JOY_BUTTON_5) coleco_joystat |=  keyCoresp[8]; // 0
        if (JoystickState & JOY_BUTTON_6) coleco_joystat |=  keyCoresp[9]; // 1
        if (JoystickState & JOY_BUTTON_7) coleco_joystat |=  keyCoresp[10]; // 2
        if (JoystickState & JOY_BUTTON_8) coleco_joystat |=  keyCoresp[11]; // 3
        if (JoystickState & JOY_BUTTON_9) coleco_joystat |=  keyCoresp[12]; // 4
        if (JoystickState & JOY_BUTTON_10) coleco_joystat |=  keyCoresp[13]; // 5
        if (JoystickState & JOY_BUTTON_11) coleco_joystat |=  keyCoresp[14]; // 6
        if (JoystickState & JOY_BUTTON_12) coleco_joystat |=  keyCoresp[15]; // 7*/

        if (JoystickState & JOY_PAD_UP) coleco_joystat |=  keyCoresp[0]; // UP
        else coleco_joystat &= ~keyCoresp[0];
        if (JoystickState & JOY_PAD_DOWN) coleco_joystat |=  keyCoresp[1]; // DOWN
        else coleco_joystat &= ~keyCoresp[1];
        if (JoystickState & JOY_PAD_LEFT) coleco_joystat |=  keyCoresp[2]; // LEFT
        else coleco_joystat &= ~keyCoresp[2];
        if (JoystickState & JOY_PAD_RIGHT) coleco_joystat |=  keyCoresp[3]; // RIGHT
        else coleco_joystat &= ~keyCoresp[3];
    }
}

//---------------------------------------------------------------------------

// DOne during each line end
void RCUpdate(void)
{
    // No interrupt for  the moment
    z80_set_irq_line(INPUT_LINE_IRQ0, CLEAR_LINE);

    // Reset spinner bits
    coleco_joystat&=~0x30003000;

    // Count ticks for spinners
    coleco_spinrecur[0]+=coleco_spinparam[0];
    coleco_spinrecur[1]+=coleco_spinparam[1];

    // Process spinner 1
    if (coleco_spinrecur[0]&0x00008000)
    {
        coleco_spinrecur[0]&=~0x00008000;
        coleco_joystat|=coleco_spinstate[0];
        z80_set_irq_line(INPUT_LINE_IRQ0, ASSERT_LINE);
    }

    // Process spinner 2
    if (coleco_spinrecur[1]&0x00008000)
    {
        coleco_spinrecur[1]&=~0x00008000;
        coleco_joystat|=(coleco_spinstate[1]<<16);
        z80_set_irq_line(INPUT_LINE_IRQ0, ASSERT_LINE);
    }
}

