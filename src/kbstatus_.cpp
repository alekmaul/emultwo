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

//---------------------------------------------------------------------------
JOYINFO JoyInfo;
JOYCAPS JoyCaps;
unsigned short JoystickState;

int mouse_xpos=0, mouse_ypos=0;

unsigned short JoyP1[NBKEYCV],JoyP2[NBKEYCV];
unsigned int key_shift=0,key_control=0;

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
/*
'------------------------------------------------------------------------------
' Complete Virtual Key Listing for PowerBASIC:
' Originally posted at nehe.gamedev.net
   '---------------------------------------------------------------------------
    MACRO VK_LBUTTON              = &h01  'Left mouse button
    MACRO VK_RBUTTON              = &h02  'Right mouse button
    MACRO VK_CANCEL               = &h03  'Control-break processing
    MACRO VK_MBUTTON              = &h04  'Middle mouse button
    MACRO VK_XBUTTON1             = &h05  'Win2K X1 mouse button
    MACRO VK_XBUTTON2             = &h06  'Win2K X2 mouse button
    MACRO VK_BACK                 = &h08  'BACKSPACE key
    MACRO VK_TAB                  = &h09  'TAB key
    MACRO VK_CLEAR                = &h0C  'CLEAR key
    MACRO VK_RETURN               = &h0D  'ENTER key
    MACRO VK_SHIFT                = &h10  'SHIFT key
    MACRO VK_CONTROL              = &h11  'CTRL key
    MACRO VK_MENU                 = &h12  'ALT key
    MACRO VK_PAUSE                = &h13  'PAUSE key
    MACRO VK_CAPITAL              = &h14  'CAPS LOCK key
    MACRO VK_KANA                 = &h15  'IME Kana mode
    MACRO VK_HANGUEL              = &h15  'IME Hanguel mode
    MACRO VK_HANGUL               = &h15  'IME Hangul mode
    MACRO VK_JUNJA                = &h17  'IME Junja mode
    MACRO VK_FINAL                = &h18  'IME final mode
    MACRO VK_HANJA                = &h19  'IME Hanja mode
    MACRO VK_KANJI                = &h19  'IME Kanji mode
    MACRO VK_ESCAPE               = &h1B  'ESC key
    MACRO VK_CONVERT              = &h1C  'IME convert
    MACRO VK_NONCONVERT           = &h1D  'IME nonconvert
    MACRO VK_ACCEPT               = &h1E  'IME accept
    MACRO VK_MODECHANGE           = &h1F  'IME mode change request
    MACRO VK_SPACE                = &h20  'SPACEBAR
    MACRO VK_PRIOR                = &h21  'PAGE UP key
    MACRO VK_NEXT                 = &h22  'PAGE DOWN key
    MACRO VK_END                  = &h23  'END key
    MACRO VK_HOME                 = &h24  'HOME key
    MACRO VK_LEFT                 = &h25  'LEFT ARROW key
    MACRO VK_UP                   = &h26  'UP ARROW key
    MACRO VK_RIGHT                = &h27  'RIGHT ARROW key
    MACRO VK_DOWN                 = &h28  'DOWN ARROW key
    MACRO VK_SELECT               = &h29  'SELECT key
    MACRO VK_PRINT                = &h2A  'PRINT key
    MACRO VK_EXECUTE              = &h2B  'EXECUTE key
    MACRO VK_SNAPSHOT             = &h2C  'PRINT SCREEN key
    MACRO VK_INSERT               = &h2D  'INS key
    MACRO VK_DELETE               = &h2E  'DEL key
    MACRO VK_HELP                 = &h2F  'HELP key
    MACRO VK_0                    = &h30  '0 key
    MACRO VK_1                    = &h31  '1 key
    MACRO VK_2                    = &h32  '2 key
    MACRO VK_3                    = &h33  '3 key
    MACRO VK_4                    = &h34  '4 key
    MACRO VK_5                    = &h35  '5 key
    MACRO VK_6                    = &h36  '6 key
    MACRO VK_7                    = &h37  '7 key
    MACRO VK_8                    = &h38  '8 key
    MACRO VK_9                    = &h39  '9 key
    MACRO VK_A                    = &h41  'A key
    MACRO VK_B                    = &h42  'B key
    MACRO VK_C                    = &h43  'C key
    MACRO VK_D                    = &h44  'D key
    MACRO VK_E                    = &h45  'E key
    MACRO VK_F                    = &h46  'F key
    MACRO VK_G                    = &h47  'G key
    MACRO VK_H                    = &h48  'H key
    MACRO VK_I                    = &h49  'I key
    MACRO VK_J                    = &h4A  'J key
    MACRO VK_K                    = &h4B  'K key
    MACRO VK_L                    = &h4C  'L key
    MACRO VK_M                    = &h4D  'M key
    MACRO VK_N                    = &h4E  'N key
    MACRO VK_O                    = &h4F  'O key
    MACRO VK_P                    = &h50  'P key
    MACRO VK_Q                    = &h51  'Q key
    MACRO VK_R                    = &h52  'R key
    MACRO VK_S                    = &h53  'S key
    MACRO VK_T                    = &h54  'T key
    MACRO VK_U                    = &h55  'U key
    MACRO VK_V                    = &h56  'V key
    MACRO VK_W                    = &h57  'W key
    MACRO VK_X                    = &h58  'X key
    MACRO VK_Y                    = &h59  'Y key
    MACRO VK_Z                    = &h5A  'Z key
    MACRO VK_LWIN                 = &h5B  'Left  Windows key (MS Nat Kbrd)
    MACRO VK_RWIN                 = &h5C  'Right Windows key (MS Nat Kbrd)
    MACRO VK_APPS                 = &h5D  'Applications key  (MS Nat Kbrd)
    MACRO VK_SLEEP                = &h5F  'Computer Sleep key
    MACRO VK_NUMPAD0              = &h60  'Numeric keypad 0 key
    MACRO VK_NUMPAD1              = &h61  'Numeric keypad 1 key
    MACRO VK_NUMPAD2              = &h62  'Numeric keypad 2 key
    MACRO VK_NUMPAD3              = &h63  'Numeric keypad 3 key
    MACRO VK_NUMPAD4              = &h64  'Numeric keypad 4 key
    MACRO VK_NUMPAD5              = &h65  'Numeric keypad 5 key
    MACRO VK_NUMPAD6              = &h66  'Numeric keypad 6 key
    MACRO VK_NUMPAD7              = &h67  'Numeric keypad 7 key
    MACRO VK_NUMPAD8              = &h68  'Numeric keypad 8 key
    MACRO VK_NUMPAD9              = &h69  'Numeric keypad 9 key
    MACRO VK_MULTIPLY             = &h6A  'Multiply key
    MACRO VK_ADD                  = &h6B  'Add key
    MACRO VK_SEPARATOR            = &h6C  'Separator key
    MACRO VK_SUBTRACT             = &h6D  'Subtract key
    MACRO VK_DECIMAL              = &h6E  'Decimal key
    MACRO VK_DIVIDE               = &h6F  'Divide key
    MACRO VK_F1                   = &h70  'F1 key
    MACRO VK_F2                   = &h71  'F2 key
    MACRO VK_F3                   = &h72  'F3 key
    MACRO VK_F4                   = &h73  'F4 key
    MACRO VK_F5                   = &h74  'F5 key
    MACRO VK_F6                   = &h75  'F6 key
    MACRO VK_F7                   = &h76  'F7 key
    MACRO VK_F8                   = &h77  'F8 key
    MACRO VK_F9                   = &h78  'F9 key
    MACRO VK_F10                  = &h79  'F10 key
    MACRO VK_F11                  = &h7A  'F11 key
    MACRO VK_F12                  = &h7B  'F12 key
    MACRO VK_F13                  = &h7C  'F13 key
    MACRO VK_F14                  = &h7D  'F14 key
    MACRO VK_F15                  = &h7E  'F15 key
    MACRO VK_F16                  = &h7F  'F16 key
    MACRO VK_F17                  = &h80  'F17 key
    MACRO VK_F18                  = &h81  'F18 key
    MACRO VK_F19                  = &h82  'F19 key
    MACRO VK_F20                  = &h83  'F20 key
    MACRO VK_F21                  = &h84  'F21 key
    MACRO VK_F22                  = &h85  'F22 key
    MACRO VK_F23                  = &h86  'F23 key
    MACRO VK_F24                  = &h87  'F24 key
    MACRO VK_NUMLOCK              = &h90  'NUM LOCK key
    MACRO VK_SCROLL               = &h91  'SCROLL LOCK key
    MACRO VK_LSHIFT               = &hA0  'Left SHIFT key
    MACRO VK_RSHIFT               = &hA1  'Right SHIFT key
    MACRO VK_LCONTROL             = &hA2  'Left CONTROL key
    MACRO VK_RCONTROL             = &hA3  'Right CONTROL key
    MACRO VK_LMENU                = &hA4  'Left MENU key
    MACRO VK_RMENU                = &hA5  'Right MENU key
    MACRO VK_BROWSER_BACK         = &hA6  'Win2K Browser Back key
    MACRO VK_BROWSER_FORWARD      = &hA7  'Win2K Browser Forward key
    MACRO VK_BROWSER_REFRESH      = &hA8  'Win2K Browser Refresh key
    MACRO VK_BROWSER_STOP         = &hA9  'Win2K Browser Stop key
    MACRO VK_BROWSER_SEARCH       = &hAA  'Win2K Browser Search key
    MACRO VK_BROWSER_FAVORITES    = &hAB  'Win2K Browser Favorites key
    MACRO VK_BROWSER_HOME         = &hAC  'Win2K Browser Launch and Home key
    MACRO VK_VOLUME_MUTE          = &hAD  'Win2K Volume Mute key
    MACRO VK_VOLUME_DOWN          = &hAE  'Win2K Volume Down key
    MACRO VK_VOLUME_UP            = &hAF  'Win2K Volume Up key
    MACRO VK_MEDIA_NEXT_TRACK     = &hB0  'Win2K Next Track key
    MACRO VK_MEDIA_PREV_TRACK     = &hB1  'Win2K Previous Track key
    MACRO VK_MEDIA_STOP           = &hB2  'Win2K Stop Media key
    MACRO VK_MEDIA_PLAY_PAUSE     = &hB3  'Win2K Play/Pause Media key
    MACRO VK_LAUNCH_MAIL          = &hB4  'Win2K Launch Mail key
    MACRO VK_LAUNCH_MEDIA_SELECT  = &hB5  'Win2K Select Media key
    MACRO VK_LAUNCH_APP1          = &hB6  'Win2K Launch Application 1 key
    MACRO VK_LAUNCH_APP2          = &hB7  'Win2K Launch Application 2 key
    MACRO VK_OEM_1                = &hBA  'Win2K US Std. Keyboard ';:' key
    MACRO VK_OEM_PLUS             = &hBB  'Win2K '+' key
    MACRO VK_OEM_COMMA            = &hBC  'Win2K ',' key
    MACRO VK_OEM_MINUS            = &hBD  'Win2K '-' key
    MACRO VK_OEM_PERIOD           = &hBE  'Win2K '.' key
    MACRO VK_OEM_2                = &hBF  'Win2K US Std. Keyboard '/?' key
    MACRO VK_OEM_3                = &hC0  'Win2K US Std. Keyboard '`~' key
    MACRO VK_OEM_4                = &hDB  'Win2K US Std. Keyboard '[{' key
    MACRO VK_OEM_5                = &hDC  'Win2K US Std. Keyboard '\|' key
    MACRO VK_OEM_6                = &hDD  'Win2K US Std. Keyboard ']}' key
    MACRO VK_OEM_7                = &hDE  'Win2K US Std. Keyboard "'   key
    MACRO VK_OEM_8                = &hDF  'Win2K US Std. Keyboard, undocumented
    MACRO VK_OEM_102              = &hE2  'Win2K '<>' or '\|' key
    MACRO VK_PROCESSKEY           = &hE5  'Win95/NT/2K IME PROCESS key
    MACRO VK_PACKET               = &hE7  'Win2K Used to pass Unicode chars
                                          'as if they were keystrokes.
                                          'The VK_PACKET key is the low word
                                          'of a 32-bit Virtual Key value used
                                          'for non-keyboard input methods.
                                          'The Unicode char is the high word.
    MACRO VK_ATTN                 = &hF6  'Attn key
    MACRO VK_CRSEL                = &hF7  'CrSel key
    MACRO VK_EXSEL                = &hF8  'ExSel key
    MACRO VK_EREOF                = &hF9  'Erase EOF key
    MACRO VK_PLAY                 = &hFA  'Play key
    MACRO VK_ZOOM                 = &hFB  'Zoom key
    MACRO VK_NONAME               = &hFC  'Reserved for future use
    MACRO VK_PA1                  = &hFD  'PA1 key
    MACRO VK_OEM_CLEAR            = &hFE  'Clear key
   '---------------------------------------------------------------------------
   'OEM Specific: &h92–&h96, &hE1, &hE3–&hE4, &hE6, &hE9–&hF5
   'Reserved:     &h0A–&h0B, &h5E, &hB8-&hB9, &hC1–&hD7, &hE0
   'Undefined:    &h07, &h0E–&h0F, &h16, &h1A, &h3A–&h40
   'Unassigned:   &h88–&h8F, &h97–&h9F,  &hE8, &hD8–&hDA
'------------------------------------------------------------------------------
'------------------------------------------------------------------------------

*/

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
                if (key==VK_CONTROL)
                {
                        key_control=1;
                }
                else
                switch (key)
                {
                case VK_ESCAPE : adamkey=KEY_ESC; break;   //     27
                case VK_BACK : adamkey=KEY_BS; break;      //   8   // + SHIFT = 184
                case VK_TAB : adamkey=KEY_TAB; break;      //  9   // + SHIFT = 185
                case VK_RETURN : adamkey=KEY_ENTER; break; //      13
                case VK_OEM_7 : adamkey=KEY_QUOTE; break;  //      '\''
                case VK_OEM_3 : adamkey=KEY_BQUOTE; break;  //     '`'
                case VK_OEM_6 : adamkey=KEY_BSLASH; break;  //     '\\'
                case VK_OEM_COMMA : adamkey=KEY_COMMA; break;  //      ','
                case VK_OEM_PERIOD : adamkey=KEY_DOT; break;  //        '.'
                case VK_OEM_2 : adamkey=KEY_SLASH; break;  //     '/'
                case VK_OEM_5 : adamkey=KEY_ASTERISK; break; //   '*'
                case VK_HOME : adamkey=KEY_HOME; break;     //  128
                case VK_F1 : adamkey=KEY_F1; break;         //129 // + SHIFT = 137
                case VK_F2 : adamkey=KEY_F2; break;        // 130 // + SHIFT = 138
                case VK_F3 : adamkey=KEY_F3; break;        // 131 // + SHIFT = 139
                case VK_F4 : adamkey=KEY_F4; break;        // 132 // + SHIFT = 140
                case VK_F5 : adamkey=KEY_F5; break;        // 133 // + SHIFT = 141
                case VK_F6 : adamkey=KEY_F6; break;        // 134 // + SHIFT = 142
                case VK_F7 : adamkey=KEY_WILDCARD; break; //   144 // + SHIFT = 152
                case VK_PAUSE : adamkey=KEY_UNDO; break;       // 145 // + SHIFT = 153
                case VK_PRIOR : adamkey=KEY_MOVE; break; //       146 // + SHIFT = 154 (COPY)
                case VK_NEXT : adamkey=KEY_STORE; break; //147 // + SHIFT = 155 (FETCH)
                case VK_END : adamkey=KEY_CLEAR; break;      //150 // + SHIFT = 158
                case VK_INSERT : adamkey=KEY_INS; break;        //148 // + SHIFT = 156
                case VK_PRINT : adamkey=KEY_PRINT; break;     // 149 // + SHIFT = 157
                case VK_DELETE : adamkey=KEY_DEL; break;      //  151 // + SHIFT = 159, + CTRL = 127
                case VK_UP : adamkey=KEY_UP; break;         //160 // + CTRL = 164, + HOME = 172
                case VK_RIGHT : adamkey=KEY_RIGHT; break;      //161 // + CTRL = 165, + HOME = 173
                case VK_DOWN : adamkey=KEY_DOWN; break;       //162 // + CTRL = 166, + HOME = 174
                case VK_LEFT : adamkey=KEY_LEFT; break;       //163 // + CTRL = 167, + HOME = 175
                        //case VK_HOME : adamkey=KEY_DIAG_NE; break;    //168
                        //case VK_END : adamkey=KEY_DIAG_SE; break;    //169
                        //case VK_NEXT : adamkey=KEY_DIAG_SW; break;    //170
                        //case VK_PRIOR : adamkey=KEY_DIAG_NW; break;   // 171
                default:    // Assume any key in 0x00..0x7E range an ASCII code
//                if(key<='~')
                    adamkey=key;
                    break;
                }
                if (adamkey)
                        PutKBD(adamkey | ( (key_shift) ? CON_SHIFT:0) | ( (key_control) ? CON_CONTROL:0) );
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
                else if (key==VK_CONTROL)
                {
                        key_control=0;
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

