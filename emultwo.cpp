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
 * emultwo.cpp
 *
 * Lots of parts inspired from EightyOne Sinclair Emulator
 *   https://sourceforge.net/projects/eightyone-sinclair-emulator/
 */

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("emultwo.res");
USEUNIT("src\z80\z80.c");
USEUNIT("src\z80\z80_ops.c");
USEUNIT("src\colecoconfig.cpp");
USEUNIT("src\coleco\coleco.cpp");
USEFORM("src\main_.cpp", Form1);
USEFORM("src\about_.cpp", about);
USEUNIT("src\utils.cpp");
USEUNIT("src\accdraw_.cpp");
USELIB("libs\directdraw\ddraw.lib");
USEFORM("src\hardware_.cpp", hardware);
USEFORM("src\debug_.cpp", debug);
USEUNIT("src\sn76489\sn76489.c");
USEUNIT("src\tms9928a\tms9928a.c");
USEUNIT("src\accsound_.c");
USEUNIT("src\ay8910\ay8910.c");
USEUNIT("src\kbstatus_.cpp");
USEUNIT("src\eeprom\c24xx.c");
USEUNIT("src\adam\adamnet.cpp");
USEFORM("src\patternviewer_.cpp", patternviewer);
USEUNIT("src\adam\fdidisk.c");
USEFORM("src\cartprofile_.cpp", cartprofile);
USEFORM("src\nametabviewer_.cpp", nametabviewer);
USEFORM("src\spriteviewer_.cpp", spriteviewer);
USELIB("libs\libpngstat\libpngstat.lib");
USELIB("libs\libpngstat\zlib.lib");
USEUNIT("src\symbolstore.cpp");
USEFORM("src\editvalue_.cpp", EditValue);
USEFORM("src\symbbrows_.cpp", SymbBrows);
USEFORM("src\setbreakpoint_.cpp", SetBreakpoint);
USEFORM("src\searchsequence_.cpp", SearchSequence);
USEFORM("src\dumpmem_.cpp", DumpMem);
USEFORM("src\printviewer_.cpp", printviewer);
//---------------------------------------------------------------------------
#include "main_.h"

//---------------------------------------------------------------------------
char **CommandLine;
HINSTANCE g_hwndMain;

WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdline, int)
{
        int i;
        char *p, *CmdLineRaw;
        bool quote;

        CmdLineRaw=(char *)malloc(strlen(cmdline)+2);
        strcpy(CmdLineRaw,cmdline);

        p=CmdLineRaw;
        quote=false;

        while(*p)
        {
                if (*p=='\"') quote=!quote;
                if ((*p==' ') && (quote==false)) *p=0;
                p++;
        }
        *++p=0;

        i=0;
        p=CmdLineRaw;
        while(strlen(p))
        {
                i++;
                p+=strlen(p)+1;
        }
        i++;

        CommandLine=(char **)malloc(i*sizeof(char *));

        p=CmdLineRaw;
        i=0;
        while(strlen(p))
        {
                CommandLine[i++]=p;
                p+=strlen(p)+1;
        }
        CommandLine[i]=NULL;

        i=0;
        while(CommandLine[i])
        {
                p=CommandLine[i];

                if ((p[0]=='\"') && (p[strlen(p)-1]=='\"'))
                {
                        p[strlen(p)-1]=0;
                        CommandLine[i]++;
                }
                i++;
        }

        try
        {
                g_hwndMain=hInstance;

                Application->Initialize();
                Application->Title = "Emul Two";
                #include "allformsImpl.cpp"

                Application->ProcessMessages();

                Form1->Enabled = true;
                Form1->GatherWindowsIfRequired();
                Form1->AnimTimer1->Enabled=true;
                Form1->Timer2->Enabled=true;
                Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }

        return 0;
}
//---------------------------------------------------------------------------
