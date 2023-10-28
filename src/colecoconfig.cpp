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

EMUL2 emul2;
MACHINE machine;

int lastMemoryReadAddrLo, lastMemoryWriteAddrLo;
int lastMemoryReadValueLo, lastMemoryWriteValueLo;
int lastMemoryReadAddrHi, lastMemoryWriteAddrHi;
int lastMemoryReadValueHi, lastMemoryWriteValueHi;

int TVW, TVH, TVBL, TVBT;        // Screen width & height & border left & border top

extern bool GetVersionNumber(int& versionNumberMajor, int& versionNumberMinor, int& versionNumberPart3, int& versionNumberPart4);
//---------------------------------------------------------------------------

void load_config(void)
{
    int versionNumberMajor,versionNumberMinor, versionNumberPart3,versionNumberPart4;

    // Retrieve version number of exe
    GetVersionNumber(versionNumberMajor, versionNumberMinor, versionNumberPart3, versionNumberPart4);

    // Init default values
    emul2.emuid=EMUID;
    emul2.major=versionNumberMajor;
    emul2.minor=versionNumberMinor;
    emul2.testver=versionNumberPart3;
    emul2.machine=MACHINECOLECO;
    emul2.NTSC=1;
    emul2.SGM=0;
    emul2.F18A=0;
    emul2.steerwheel=0;
    emul2.rollercontrol=0;
    emul2.superaction=0;

    emul2.frameskip=0;

    emul2.cardcrc=-1;
    emul2.cardsize=0;
    emul2.startdebug=0;
    emul2.romCartridge = ROMCARTRIDGENONE;
    emul2.typebackup=NOBACKUP;
    emul2.palette=0;
    emul2.hackbiospal;
    emul2.biosnodelay;
    emul2.singlestep=0;
    emul2.stop=0;

    strcpy(emul2.machinename, "E.mul T.wo");

    strcpy(emul2.colecobios,"Internal");
    strcpy(emul2.adameos,"Internal");
    strcpy(emul2.adamwriter,"Internal");

    strcpy(emul2.currentrom,"");

    machine.clockspeed=CLOCK_NTSC;
    machine.tperscanline=207;
    machine.tperframe=312*207;
    machine.fps=50;
    machine.intposition=0;

    TVW=TVW_TMS;
    TVH=TVH_TMS;
}

