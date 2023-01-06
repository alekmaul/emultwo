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
 * about.cpp
 *
 */


//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "about_.h"
#include "colecoconfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tabout *about;

#define MaxWidth(label,width) if((label->Width + label->Left) > width) width=label->Width + label->Left

//---------------------------------------------------------------------------

static AnsiString ExtractPart(AnsiString versionNumber, int& s, int&e)
{
        AnsiString part;

        while (e <= versionNumber.Length())
        {
                if (versionNumber[e] == '.')
                {
                        part = versionNumber.SubString(s, e - s);
                        s = e + 1;
                        e += 2;
                        break;
                }

                e++;
        }

        return part;
}

bool GetVersionNumber(int& versionNumberMajor, int& versionNumberMinor, int& versionNumberPart3, int& versionNumberPart4)
{
        versionNumberMajor = 0;
        versionNumberMinor = 0;
        versionNumberPart3 = 0;
        versionNumberPart4 = 0;
        
        AnsiString versionNumber = "";

        AnsiString fileName = Application->ExeName;

	// Get the size of the version information buffer
	DWORD dwHandle = 0;
	DWORD dwLength = GetFileVersionInfoSize(fileName.c_str(), &dwHandle);
	if (dwLength < 1)
        {
		return false;
	}

	// Allocate space for the version information buffer
	LPVOID lpvMem;
	HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE, dwLength);
	if (hMem == NULL)
        {
		return false;
	}
        
	lpvMem = GlobalLock(hMem);
	if (lpvMem == NULL)
        {
		GlobalUnlock(hMem);
		GlobalFree(hMem);
		return false;
	}

	// Get the version information block to the buffer
	BOOL fRet = GetFileVersionInfo(	fileName.c_str(), dwHandle, dwLength, (LPVOID)lpvMem);
	if (!fRet)
        {
		GlobalUnlock(hMem);
		GlobalFree(hMem);
		return false;
	}

        // Get the root block
        VS_FIXEDFILEINFO fileInfo;
	LPVOID lpInfo;
	UINT cch;
	if (VerQueryValue(lpvMem, "\\", &lpInfo, &cch))
        {
                ZeroMemory((void*)(&fileInfo), sizeof(fileInfo));
                CopyMemory((void*)&fileInfo, (const void*)lpInfo, sizeof(fileInfo));
        }
	else
        {
		GlobalUnlock(hMem);
		GlobalFree(hMem);
		return false;
	}

	// Get translation information
        AnsiString langID;
        AnsiString charset;
        if (VerQueryValue(lpvMem, TEXT("\\VarFileInfo\\Translation"), &lpInfo, &cch))
        {
    	        WORD wLangID = ((WORD*)lpInfo)[0];
                WORD wCharsetID = ((WORD*)lpInfo)[1];
                langID = langID.IntToHex((int)wLangID, 4);
                charset = charset.IntToHex((int)wCharsetID, 4);
   	}
  	else
        {
  		GlobalUnlock(hMem);
  		GlobalFree(hMem);
  		return false;
  	}

	// Fetch the version information
	TCHAR key[80];

	lstrcpy(key, TEXT("\\StringFileInfo\\"));
   	AnsiString versionInfoLangID(langID + charset);
        lstrcat(key, versionInfoLangID.c_str());
	lstrcat(key, "\\");
	lstrcat(key, TEXT("FileVersion"));

	if (VerQueryValue(lpvMem, key, &lpInfo, &cch))
        {
                versionNumber = (char*)lpInfo;
                versionNumber += ".";
        }
        else
        {
  		GlobalUnlock(hMem);
  		GlobalFree(hMem);
  		return false;
  	}
        
	GlobalUnlock(hMem);
	GlobalFree(hMem);

        // Extract the version number parts

        int s = 1;
        int e = 2;
        versionNumberMajor = StrToInt(ExtractPart(versionNumber, s, e));
        versionNumberMinor = StrToInt(ExtractPart(versionNumber, s, e));
        versionNumberPart3 = StrToInt(ExtractPart(versionNumber, s, e));
        versionNumberPart4 = StrToInt(ExtractPart(versionNumber, s, e));

        return true;
}
//---------------------------------------------------------------------------
__fastcall Tabout::Tabout(TComponent* Owner)
        : TForm(Owner)
{
        int w=0;

        Left = (Screen->Width - Width) /2;
        Top = (Screen->Height - Height) /2;

        int versionNumberMajor;
        int versionNumberMinor;
        int versionNumberPart3;
        int versionNumberPart4;
        bool versionFound = GetVersionNumber(versionNumberMajor, versionNumberMinor, versionNumberPart3, versionNumberPart4);
        if (versionFound)
        {
                Version->Caption="Version ";
                Version->Caption = Version->Caption + versionNumberMajor;
                Version->Caption = Version->Caption + ".";
                Version->Caption = Version->Caption + versionNumberMinor;
                if (versionNumberPart3 != 0)
                {
                        Version->Caption = Version->Caption + ".";
                        Version->Caption = Version->Caption + versionNumberPart3;
                        if (versionNumberPart4 != 0)
                        {
                                Version->Caption = Version->Caption + ".";
                                Version->Caption = Version->Caption + versionNumberPart4;
                        }
                }
        }
        else
        {
                Version->Caption = "Unknown";
        }

        MaxWidth(Label1,w);
        MaxWidth(Label2,w); MaxWidth(Label10,w);
        MaxWidth(Label3,w); MaxWidth(Label11,w);
        MaxWidth(Label4,w);
        MaxWidth(Label5,w);
        MaxWidth(Label15,w);
        MaxWidth(Label8,w);

        ClientWidth=w+8;
}

//---------------------------------------------------------------------------
void __fastcall Tabout::Button1Click(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------

void __fastcall Tabout::Label15Click(TObject *Sender)
{
        //  Original = http://www.chuntey.com/eightyone/
        
        ShellExecute(0,NULL, "https://github.com/alekmaul/emultwo/", NULL, NULL, SW_SHOW);
}
//---------------------------------------------------------------------------

void __fastcall Tabout::Label15MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        Label15->Font->Color=clRed;
}
//---------------------------------------------------------------------------

void __fastcall Tabout::Label15MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        Label15->Font->Color=clHighlight;
}
//---------------------------------------------------------------------------





