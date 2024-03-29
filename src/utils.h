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
 * utils.h
 *
 */

//---------------------------------------------------------------------------

#ifndef utilsH
#define utilsH
//---------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

AnsiString FileNameGetPath(AnsiString Fname);
AnsiString FileNameGetExt(AnsiString Fname);
AnsiString GetExt(AnsiString Fname);
unsigned int CRC32Block (const unsigned char *buf, unsigned int len);
//bool ImageToPNG(AnsiString name,TImage* image);
String NameAndDateTimePng( String name );
bool ImageToPNG(AnsiString name,Graphics::TBitmap *bitmap);
void Logger( String logMsg );

#ifdef __cplusplus
}
#endif

#endif


 