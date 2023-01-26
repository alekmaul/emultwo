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
 * accdraw_.h
 *
 */

//---------------------------------------------------------------------------

#ifndef accdraw_H
#define accdraw_H

extern Graphics::TBitmap *GDIFrame;

//---------------------------------------------------------------------------
extern int RenderInit(void);
extern void RenderEnd(void);
extern void RecalcWinSize(void);
extern void AccurateInit(int resize);
extern void AccurateUpdateDisplay(bool singlestep);
extern void AccurateBGDisplay(HWND hWnd, int w, int h);
extern void AccurateDraw(unsigned int Line);

extern void RenderCalcPalette(unsigned char *palette);

extern void RenderSaveScreenBMP(AnsiString filename);
extern void RenderSaveScreenPNG(AnsiString filename);

#endif