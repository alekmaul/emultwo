/* EmulTwo  - A Windows ColecoVision emulator.
 * Copyright (C) 2014-2022 Alekmaul
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
 * colecoconfig.c
 *
 */

#include <string.h>
#include <dir.h>

#include "colecoconfig.h"

COLECO coleco;
MACHINE machine;
MOUSE mouse;

int lastMemoryReadAddrLo, lastMemoryWriteAddrLo;
int lastMemoryReadValueLo, lastMemoryWriteValueLo;
int lastMemoryReadAddrHi, lastMemoryWriteAddrHi;
int lastMemoryReadValueHi, lastMemoryWriteValueHi;
int directMemoryAccess;

extern bool GetVersionNumber(int& versionNumberMajor, int& versionNumberMinor, int& versionNumberPart3, int& versionNumberPart4);

void load_config(void)
{
        int versionNumberMajor;
        int versionNumberMinor;
        int versionNumberPart3;
        int versionNumberPart4;
        GetVersionNumber(versionNumberMajor, versionNumberMinor, versionNumberPart3, versionNumberPart4);

        coleco.emuid=EMUID;
        coleco.major=versionNumberMajor;
        coleco.minor=versionNumberMinor;
        coleco.testver=versionNumberPart3;
        coleco.machine=MACHINECOLECO;
        coleco.NTSC=1;
        coleco.SGM=0;
        coleco.inverse=0;
        coleco.frameskip=0;
        coleco.aysound=0;
        coleco.aytype=0;
        coleco.vsyncsound=0;
        coleco.cardcrc=-1;
        coleco.cardsize=0;
        coleco.romCartridge = ROMCARTRIDGENONE;
        coleco.typebackup=NOBACKUP;

        machine.clockspeed=CLOCK_NTSC;
        machine.tperscanline=207;
        machine.tperframe=312*207;
        machine.intposition=0;

        coleco.singlestep=0;
        coleco.stop=0;

        strcpy(coleco.machinename, "E.mul T.wo");

        mouse.x=0;
        mouse.y=0;
        mouse.buttons=0;
        mouse.lastx=0;
        mouse.lasty=0;
}

