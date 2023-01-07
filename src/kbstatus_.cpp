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
    -Driving Controller (Expansion Module #2). It consist of a steering wheel and a gas pedal. Only one
    can be used on a real ColecoVision. The gas pedal is not analog, internally it is just a switch.
    On a real ColecoVision, when the Driving Controller is enabled, the controller 1 do not work because
    have been replaced by the Driving Controller, and controller 2 have to be used to start game, gear
    shift, etc.
    Driving Controller is just a spinner on controller 1 socket similar to the one on Roller Controller
    and Super Action Controllers so you can use Roller Controller or Super Action Controllers to play
    games requiring Driving Controller.
    -Roller Controller. Basically a trackball with four buttons (the two fire buttons from player 1 and
    the two fire buttons from player 2). Only one Roller Controller can be used on a real ColecoVision.
    Roller Controller is connected to both controller sockets and both controllers are conected to the Roller
    Controller, it uses the spinner pins of both sockets to generate the X and Y signals (X from controller 1
    and the Y from controller 2)
    -Super Action Controllers. It is a hand controller with a keypad, four buttons (the two from
    the player pad and two more), and a spinner. This was made primarily for two player sport games, but
    will work for every other ColecoVision game.
*/

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "kbstatus_.h"
#include "main_.h"

#include "adamnet.h"
#include "coleco.h"

//---------------------------------------------------------------------------
struct kb
{
        WORD WinKey;
};

JOYINFO JoyInfo;
JOYCAPS JoyCaps;

unsigned int JoystickState;

BYTE KeyState[256];

unsigned short JoyP1[NBKEYCV],JoyP2[NBKEYCV];
unsigned int key_shift=0;

/*

*/

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

/*
/** GetMouse() ***********************************************/
/** Get mouse position and button states in the following   **/
/** format: RMB.LMB.Y[29-16].X[15-0].                       

GetMouse
    /* Scale mouse position 
    X = VideoW*(X<0? 0:X>=XSize? XSize-1:X)/XSize;
    Y = VideoH*(Y<0? 0:Y>=YSize? YSize-1:Y)/YSize;


/** Mouse() **************************************************/
/** This function is called to poll mouse. It should return **/
/** the X coordinate (-512..+512) in the lower 16 bits, the **/
/** Y coordinate (-512..+512) in the next 14 bits, and the  **/
/** mouse buttons in the upper 2 bits. All values should be **/
/** counted from the screen center!                         **/
/************************************************************

  unsigned int X,Y;

  X = GetMouse();
  Y = ((((X>>16)&0x0FFF)<<10)/200-512)&0x3FFF;
  Y = (Y<<16)|((((X&0x0FFF)<<10)/320-512)&0xFFFF);
  return(Y|(X&MSE_BUTTONS));

  YYYYYYYYYYYYYYYYXXXXXXXXXXXXXXXX

#define MSE_RIGHT    0x80000000
#define MSE_LEFT     0x40000000
#define MSE_BUTTONS  (MSE_RIGHT|MSE_LEFT)
#define MSE_YPOS     0x3FFF0000
#define MSE_XPOS     0x0000FFFF



  /* If emulating spinners...
  if(Mode&CV_SPINNERS)
  {
    int I,K;

    /* Get mouse position relative to the window center,
    /* normalized to -512..+512 range
    I = Mouse();
    /* First spinner 
    K = (Mode&CV_SPINNER1Y? (I<<2):Mode&CV_SPINNER1X? (I<<16):0)>>16;
    K = K<-512? -512:K>512? 512:K;
    SpinStep  = K>=0? (K>32? K:0):(K<-32? -K:0);
    SpinState = K>0? 0x00003000:K<0? 0x00001000:0;
    /* Second spinner
    K = (Mode&CV_SPINNER2Y? (I<<2):Mode&CV_SPINNER2X? (I<<16):0)>>16;
    K = K<-512? -512:K>512? 512:K;
    SpinStep |= (K>=0? (K>32? K:0):(K<-32? -K:0))<<16;
    SpinState|= K>0? 0x10000000:K<0? 0x30000000:0;
    /* Fire buttons
    if(I&0x80000000)
      JoyState |= (Mode&CV_SPINNER2? (JST_FIRER<<16):0)
               |  (Mode&CV_SPINNER1? JST_FIRER:0);
    if(I&0x40000000)
      JoyState |= (Mode&CV_SPINNER2? (JST_FIREL<<16):0)
               |  (Mode&CV_SPINNER1? JST_FIREL:0);
  }

*/

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

        // Get joystick values if possible
        if ((JoyCaps.wMid != 0) && JoystickState)
        {
                if (JoystickState & JOY_BUTTON_1) coleco_joystat |=  keyCoresp[4]; // but #1
                if (JoystickState & JOY_BUTTON_2) coleco_joystat |=  keyCoresp[5]; // but #2

/*        if (JoystickState & JOY_BUTTON_3) valTche |=  keyCoresp[6]; // but ##
        if (JoystickState & JOY_BUTTON_4) valTche |=  keyCoresp[7]; // but #*

        if (JoystickState & JOY_BUTTON_5) valTche |=  keyCoresp[8]; // 0
        if (JoystickState & JOY_BUTTON_6) valTche |=  keyCoresp[9]; // 1
        if (JoystickState & JOY_BUTTON_7) valTche |=  keyCoresp[10]; // 2
        if (JoystickState & JOY_BUTTON_8) valTche |=  keyCoresp[11]; // 3
        if (JoystickState & JOY_BUTTON_9) valTche |=  keyCoresp[12]; // 4
        if (JoystickState & JOY_BUTTON_10) valTche |=  keyCoresp[13]; // 5
        if (JoystickState & JOY_BUTTON_11) valTche |=  keyCoresp[14]; // 6
        if (JoystickState & JOY_BUTTON_12) valTche |=  keyCoresp[15]; // 7*/
        }

        // Check now keyboard if we are in ADAM mode
        if (coleco.machine==MACHINEADAM)
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
                        case VK_RETURN: adamkey=KEY_ENTER; break;
                        case VK_BACK: adamkey=KEY_BS; break;
                        case VK_PRIOR : adamkey=KEY_MOVE; break; // PageUp
                        case VK_ESCAPE : adamkey=KEY_MOVE; break;
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
        if (coleco.machine==MACHINEADAM)
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
        JoystickState &= 0xFFFFFF00;
        if (PosX<10000) JoystickState |= JOY_PAD_LEFT;
        else if (PosX>55000) JoystickState |= JOY_PAD_RIGHT;
        if (PosY<10000) JoystickState |= JOY_PAD_UP;
        else if (PosY>55000) JoystickState |= JOY_PAD_DOWN;
/*
        if (JoystickState & JOY_PAD_UP) coleco_joystat |=  keyCoresp[0]; // UP
        else coleco_joystat &= ~keyCoresp[0];
        if (JoystickState & JOY_PAD_DOWN) coleco_joystat |=  keyCoresp[1]; // DOWN
        else coleco_joystat &= ~keyCoresp[1];
        if (JoystickState & JOY_PAD_LEFT) coleco_joystat |=  keyCoresp[2]; // LEFT
        else coleco_joystat &= ~keyCoresp[2];
        if (JoystickState & JOY_PAD_RIGHT) coleco_joystat |=  keyCoresp[3]; // RIGHT
        else coleco_joystat &= ~keyCoresp[3];
*/
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
    if (JoyCaps.wMid != 0) {
        joyReleaseCapture(JOYSTICKID1);
    }
}

