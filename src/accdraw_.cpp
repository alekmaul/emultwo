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
 * accdraw_.cpp
 *
 * Lots of parts inspired from EightyOne Sinclair Emulator
 *   https://sourceforge.net/projects/eightyone-sinclair-emulator/
 *
 */

//---------------------------------------------------------------------------

#include <vcl.h>
#include <ddraw.h>
#include <stdio.h>
#pragma hdrstop

#include "accdraw_.h"
#include "main_.h"
#include "colecoconfig.h"
#include "coleco.h"
#include "utils.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#define NoWinT  0  /*32*/
#define NoWinB  (NoWinT+TVH/*240*/)
#define NoWinL  0 /*42*/
#define NoWinR  (NoWinL+TVW/*320*/)

int WinR,WinL,WinT,WinB;

Graphics::TBitmap *GDIFrame;

TRect rcsource, rcdest;
BYTE *dest=NULL,*buffer=NULL;
int TVP;

#define Plot(x,c) { *(DWORD *)(dest+(x)) = cv_pal32[(c)]; }

/* DirectDraw NOT WORKING ON WINDOWS 11    */

LPDIRECTDRAW7        m_pDD=NULL;              // DirectDraw object
LPDIRECTDRAWSURFACE7 m_pddsFrontBuffer=NULL;  // DirectDraw primary surface
LPDIRECTDRAWSURFACE7 m_pddsFrame=NULL;        // DirectDraw back surface
LPDIRECTDRAWSURFACE7 DDFrame;
DDSURFACEDESC2       DDFrameSurface;

LPDIRECTDRAWCLIPPER pcClipper=NULL;          // Clipper for windowed mode
HWND                hWnd;                     // Handle of window

int BPP;

// -----------------------------------------------------------------------------
// DirectDraw Functions
// -----------------------------------------------------------------------------
bool DDError(bool result, AnsiString Message)
{
    if (result)
        MessageBoxA(NULL,
          "Count not initialise DirectDraw.\nPlease ensure DirectX 7 or greater is installed",
           Message.c_str(),
           MB_OK);

    return(result);
}

void DDEnd(void)
{
    if (m_pDD!= NULL)
    {
        if (m_pddsFrontBuffer != NULL)
        {
            if (m_pddsFrame) m_pddsFrame->Release();
            m_pddsFrame = NULL;

            if (pcClipper) pcClipper->Release();
            pcClipper = NULL;

            if (m_pddsFrontBuffer) m_pddsFrontBuffer->Release();
            m_pddsFrontBuffer = NULL;
        }
        m_pDD->Release();
        m_pDD = NULL;
    }
}

int DDInit(void)
{
    HRESULT hRet;

    hWnd = Form1->Handle;

    DDEnd();
    hRet = DirectDrawCreateEx(NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL);
    if(DDError(hRet != DD_OK,"DDCreateEX")) return(false);

    hRet = m_pDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
    if(DDError(hRet != DD_OK,"DDSetCoop")) return(false);

    HRESULT hr;

    DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    if(DDError(hr = m_pDD->CreateSurface(&ddsd, &m_pddsFrontBuffer, NULL)
      ,"CreateFrontSurface")) return(false);

    // Create the backbuffer surface
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
    ddsd.dwWidth = 1024;
    ddsd.dwHeight = 768;

    hr = m_pDD->CreateSurface(&ddsd, &m_pddsFrame, NULL);
    if (hr)
    {
        ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
        ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
        ddsd.dwWidth = 460;
        ddsd.dwHeight = 400;
        hr = m_pDD->CreateSurface(&ddsd, &m_pddsFrame, NULL);
        if(DDError(hr,"CreateBackSurface")) return(false);
    }

    if(DDError(hr = m_pDD->CreateClipper(0, &pcClipper, NULL)
        ,"CreateClipper")) return(false);

    if(DDError(hr = pcClipper->SetHWnd(0, hWnd),"SetClipperHwnd"))
    {
        pcClipper->Release();
        return(false);
    }

    if(DDError(hr = m_pddsFrontBuffer->SetClipper(pcClipper),"SetClipperSurface"))
    {
        pcClipper->Release();
        return(false);
    }

    return(true);
}

void DDAccurateInit(int resize)
{
    DDPIXELFORMAT DDpf;
    DDSURFACEDESC2 ddsd;

    float OrigW, OrigH, ScaleW, ScaleH;

    OrigW=Form1->ClientWidth;
    OrigH=Form1->ClientHeight;
    OrigH -= Form1->StatusBar1->Height;

    if (Form1->BaseWidth==0)
        Form1->BaseWidth= NoWinR-NoWinL;
    if (Form1->BaseHeight==0)
        Form1->BaseHeight= NoWinB-NoWinT;

    ScaleW = OrigW / Form1->BaseWidth;
    ScaleH = OrigH / Form1->BaseHeight;

    //fill the DDpf structure and get the BytesPerPixel
    ZeroMemory (&DDpf, sizeof(DDpf));
    DDpf.dwSize = sizeof(DDpf);
    m_pddsFrontBuffer->GetPixelFormat(&DDpf);
    BPP = DDpf.dwRGBBitCount/8;

    WinL=NoWinL; WinR=NoWinR; WinT=NoWinT; WinB=NoWinB;
    if (emul2.NTSC) { WinT-=24; WinB-=24; }

    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize = sizeof( ddsd );

    // Create the backbuffer surface
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
    ddsd.dwWidth = TVW;
    ddsd.dwHeight = TVH;

    m_pddsFrame->Release(); m_pddsFrame = NULL;
    m_pDD->CreateSurface(&ddsd, &m_pddsFrame, NULL);

    if (resize)
    {
        Form1->BaseWidth=WinR-WinL;
        Form1->BaseHeight=WinB-WinT;

        OrigW = Form1->BaseWidth * ScaleW;
        OrigH = Form1->BaseHeight * ScaleH;
        OrigH += Form1->StatusBar1->Height;

        Form1->ClientWidth = OrigW;
        Form1->ClientHeight = OrigH;
    }

    DDFrame=m_pddsFrame;

    ZeroMemory(&DDFrameSurface, sizeof(DDFrameSurface));
    DDFrameSurface.dwSize = sizeof(DDFrameSurface);
    DDFrame->Lock(NULL, &DDFrameSurface, DDLOCK_WAIT |  DDLOCK_NOSYSLOCK, NULL);

    dest=buffer=(BYTE*)DDFrameSurface.lpSurface;
    TVP=DDFrameSurface.lPitch;

    RenderCalcPalette(cv_palette,16*4);
    RecalcWinSize();
}

void DDAccurateUpdateDisplay(bool singlestep)
{
    static int framecounter=0;
    HRESULT hRet;
    RECT rDest;

    if (++framecounter > emul2.frameskip || singlestep)
        framecounter=0;
    else
        return;

    DDFrame->Unlock(NULL);

    POINT p = {0, 0};
    p=Form1->ClientToScreen(p);

    rDest=rcdest;
    rDest.left += p.x;
    rDest.top += p.y;
    rDest.right += p.x;
    rDest.bottom += p.y;

    while(1)
    {
        hRet = m_pddsFrontBuffer->Blt(&rDest, DDFrame, &rcsource, DDBLT_WAIT, NULL);

        if (hRet == DD_OK) break;
        else
        if(hRet == DDERR_SURFACELOST)
        {
            m_pddsFrontBuffer->Restore();
            m_pddsFrame->Restore();
        }
        else if(hRet != DDERR_WASSTILLDRAWING) return;
    }
    DDFrame->Lock(NULL, &DDFrameSurface, DDLOCK_WAIT |  DDLOCK_NOSYSLOCK, NULL);
    dest=buffer=(BYTE*)DDFrameSurface.lpSurface;
}

int DDGetMaskInfo (DWORD Bitmask, int* lpShift)
{
    int Precision, Shift;

    Precision = Shift = 0;
    //count the zeros on right hand side
    while (!(Bitmask & 0x01L))
    {
        Bitmask >>= 1;
        Shift++;
    }

    //count the ones on right hand side
    while (Bitmask & 0x01L)
    {
        Bitmask >>= 1;
        Precision++;
    }
    *lpShift = Shift;
    return Precision;
}

// -----------------------------------------------------------------------------
// GDI Functions
// -----------------------------------------------------------------------------
void GDIAccurateInit(int resize)
{
    float OrigW, OrigH, ScaleW, ScaleH;

    if (GDIFrame!= NULL)
    {
        delete GDIFrame;
        GDIFrame=NULL;
    }

    OrigW=Form1->ClientWidth;
    OrigH=Form1->ClientHeight;
    OrigH -= Form1->StatusBar1->Height;

    if (Form1->BaseWidth==0)
        Form1->BaseWidth= NoWinR-NoWinL;
    if (Form1->BaseHeight==0)
        Form1->BaseHeight= NoWinB-NoWinT;

    ScaleW = OrigW / Form1->BaseWidth;
    ScaleH = OrigH / Form1->BaseHeight;

    WinL=NoWinL; WinR=NoWinR; WinT=NoWinT; WinB=NoWinB;
    if (emul2.NTSC) { WinT-=24; WinB-=24; }

    GDIFrame=new Graphics::TBitmap;

    GDIFrame->Width=TVW;
    GDIFrame->Height=TVH;
    GDIFrame->PixelFormat=pf32bit; //pf16bit;//

    BPP = 4; //2; //

    if (resize)
    {
        Form1->BaseWidth=WinR-WinL;
        Form1->BaseHeight=WinB-WinT;

        OrigW = Form1->BaseWidth * ScaleW;
        OrigH = Form1->BaseHeight * ScaleH;
        OrigH += Form1->StatusBar1->Height;

        Form1->ClientWidth = OrigW;
        Form1->ClientHeight = OrigH;
    }

    dest=buffer=(unsigned char *) GDIFrame->ScanLine[0];
    TVP = ((char *)GDIFrame->ScanLine[1]) - ((char *)GDIFrame->ScanLine[0]);

    RenderCalcPalette(cv_palette,16*4);
    RecalcWinSize();
}

void GDIAccurateUpdateDisplay(bool singlestep)
{
//    int ret;
    static int framecounter=0;

    if (++framecounter > emul2.frameskip || singlestep)
        framecounter=0;
    else
        return;

    //ret =
    StretchBlt(Form1->Canvas->Handle,
                        rcdest.Left, rcdest.Top,
                        (rcdest.Right-rcdest.Left),
                        (rcdest.Bottom-rcdest.Top),
                        GDIFrame->Canvas->Handle, 0, 0, TVW, TVH,SRCCOPY);

    //if (!ret) ShowMessage(SysErrorMessage(GetLastError()));
    dest=buffer=(unsigned char *) GDIFrame->ScanLine[0];
}

// -----------------------------------------------------------------------------
// Common Functions
// -----------------------------------------------------------------------------
void RecalcWinSize(void)
{
    rcdest.Top=0; rcdest.Bottom=Form1->ClientHeight;
    rcdest.Left=0; rcdest.Right=Form1->ClientWidth;
    rcdest.Bottom -= Form1->StatusBar1->Height;

    rcsource.right=TVW;
    rcsource.bottom=TVH;
    rcsource.left=0;
    rcsource.top=0;
}

int RenderInit(void)
{
    if (Form1->RenderMode==RENDERDDRAW)
        return(DDInit());
    return(1);
}

void RenderEnd(void)
{
    if (GDIFrame)
    {
        delete GDIFrame;
        GDIFrame=NULL;
    }
    DDEnd();
}

void RenderCalcPalette(unsigned char *palette, int palsize)
{
    int rsz, gsz, bsz; 	//bitsize of field
    int rsh, gsh, bsh;	//0�s on left (the shift value)
    DWORD CompiledPixel;
    int i,r,g,b;

    if (Form1->RenderMode==RENDERDDRAW)
    {
        DDPIXELFORMAT DDpf;
        ZeroMemory (&DDpf, sizeof(DDpf));
        DDpf.dwSize = sizeof(DDpf);
        m_pddsFrontBuffer->GetPixelFormat(&DDpf);

        rsz = DDGetMaskInfo (DDpf.dwRBitMask, &rsh);
        gsz = DDGetMaskInfo (DDpf.dwGBitMask, &gsh);
        bsz = DDGetMaskInfo (DDpf.dwBBitMask, &bsh);
    }
    else
    {
        rsz=8;
        gsz=8;
        bsz=8;
        rsh=16;
        gsh=8;
        bsh=0;
    }

    for(i=0;i<palsize*3;i+=3)
    {
        r=palette[i];
        g=palette[i+1];
        b=palette[i+2];

        r >>= (8-rsz);	//keep only the MSB bits of component
        g >>= (8-gsz);
        b >>= (8-bsz);
        r <<= rsh;	//SHIFT THEM INTO PLACE
        g <<= gsh;
        b <<= bsh;

        CompiledPixel = (DWORD)(r | g | b);
        cv_pal32[i/3]=CompiledPixel;
    }
}
// -----------------------------------------------------------------------------

void AccurateInit(int resize)
{
    dest=buffer=NULL;
        if (Form1->RenderMode==RENDERDDRAW)
            DDAccurateInit(resize);
        else
            GDIAccurateInit(resize);
}

void AccurateUpdateDisplay(bool singlestep)
{
    if (Form1->RenderMode==RENDERDDRAW)
        DDAccurateUpdateDisplay(singlestep);
    else
        GDIAccurateUpdateDisplay(singlestep);
}
// -----------------------------------------------------------------------------

void AccurateDraw(unsigned int Line)
{
    unsigned char c;
    unsigned char *pcv_display;
    int i;

    if (Line<TVH)
    {
        pcv_display=cv_display+TVW*Line;
        dest=buffer+Line*TVP;
        for(i=0; i<TVW; i++)
        {
            c = *(pcv_display+i);
            Plot(i*BPP,c);
        }
    }
    if (Line==TVH)
    {
        dest=buffer;
        AccurateUpdateDisplay(false);
    }
}
// -----------------------------------------------------------------------------

void RenderSaveScreenBMP(AnsiString filename)
{
    FILE *f;
    int w,h,Pw;
    int pad, size;
    int i,j,k;
    unsigned char r,g,b;
    short int b16;
    int b32;

    w=TVW;
    h=TVH;

    Pw=w*3;
    pad=4-(Pw%4); if (pad==4) pad=0;

    size = ((Pw+pad)*h)+54;

    f=fopen(filename.c_str(),"wb");

    fwrite("BM", 1, 2, f);
    b32=size; fwrite(&b32, 1, 4, f);
    b16=0; fwrite(&b16, 1, 2, f);
    b16=0; fwrite(&b16, 1, 2, f);
    b32=54; fwrite(&b32, 1, 4, f);
    b32=40; fwrite(&b32, 1, 4, f);
    b32=w; fwrite(&b32, 1, 4, f);
    b32=h; fwrite(&b32, 1, 4, f);
    b16=1; fwrite(&b16, 1, 2, f);
    b16=24; fwrite(&b16, 1, 2, f);
    b32=0; fwrite(&b32, 1, 4, f);
    b32=0; fwrite(&b32, 1, 4, f);
    b32=0; fwrite(&b32, 1, 4, f);
    b32=0; fwrite(&b32, 1, 4, f);
    b32=0; fwrite(&b32, 1, 4, f);
    b32=0; fwrite(&b32, 1, 4, f);

    for(i=h-1;i>=0;i--)
    {
        if (Form1->RenderMode==RENDERDDRAW) {
          unsigned int *pixel = (unsigned int *) DDFrameSurface.lpSurface + (DDFrameSurface.lPitch * i)/4;
          for(j=0;j<w;j++)
          {
              int val = *pixel++;
              b = val & 0xFF;
              g = (val>>8) & 0xFF;
              r = (val>>16) & 0xFF;
              fwrite(&b, 1,1, f);
              fwrite(&g, 1,1, f);
              fwrite(&r, 1,1, f);
          }
        }
        else {
            unsigned int *pixel = (unsigned int *) GDIFrame->ScanLine[i];
            for(j=0;j<w;j++) {
              int val = *pixel++;
              b = val & 0xFF;
              g = (val>>8) & 0xFF;
              r = (val>>16) & 0xFF;
              fwrite(&b, 1,1, f);
              fwrite(&g, 1,1, f);
              fwrite(&r, 1,1, f);
            }
        }
        for(k=0;k<pad;k++) fwrite("\0", 1,1, f);
    }
    fclose(f);
}
// -----------------------------------------------------------------------------

void RenderSaveScreenPNG(AnsiString filename)
{
        Graphics::TBitmap *bmp;

        bmp = new Graphics::TBitmap;
        bmp->Width=TVW;
        bmp->Height=TVH;
        bmp->PixelFormat=pf24bit;

        try
        {
                if (Form1->RenderMode==RENDERDDRAW)
                {
                        StretchBlt(bmp->Canvas->Handle, 0,0, TVW, TVH,
                                DDFrameSurface.lpSurface, 0, 0, TVW,TVH,SRCCOPY);
                }
                else
                {
                        StretchBlt(bmp->Canvas->Handle, 0,0, TVW, TVH,
                                GDIFrame->Canvas->Handle, 0, 0, TVW,TVH,SRCCOPY);
                }
                ImageToPNG( filename, bmp);
        }
        __finally
        {
                delete bmp;
        }

}


