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

#define NoWinT  32
#define NoWinB  (NoWinT+192/*240*/)
#define NoWinL  42
#define NoWinR  (NoWinL+256/*320*/)

int WinR=NoWinR;
int WinL=NoWinL;
int WinT=NoWinT;
int WinB=NoWinB;

Graphics::TBitmap *GDIFrame;

TRect rcsource, rcdest;

/* NOT WORKING ON WINDOWS 11

#if 0
LPDIRECTDRAW        m_pDD=NULL;              // DirectDraw object
LPDIRECTDRAWSURFACE m_pddsFrontBuffer=NULL;  // DirectDraw primary surface
LPDIRECTDRAWSURFACE m_pddsFrame=NULL;        // DirectDraw back surface
LPDIRECTDRAWSURFACE DDFrame;
DDSURFACEDESC       DDFrameSurface;
#else
LPDIRECTDRAW7        m_pDD=NULL;              // DirectDraw object
LPDIRECTDRAWSURFACE7 m_pddsFrontBuffer=NULL;  // DirectDraw primary surface
LPDIRECTDRAWSURFACE7 m_pddsFrame=NULL;        // DirectDraw back surface
LPDIRECTDRAWSURFACE7 DDFrame;
DDSURFACEDESC2       DDFrameSurface;
#endif

LPDIRECTDRAWCLIPPER pcClipper=NULL;          // Clipper for windowed mode
HWND                hWnd;                     // Handle of window
int BPP,Paletteised;

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
    if (m_pDD)
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
#if 0
    hRet = DirectDrawCreate(NULL,&m_pDD,NULL);
    if(DDError(hRet != DD_OK,"DDCreate")) return(false);
#else
    hRet = DirectDrawCreateEx(NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL);
    if(DDError(hRet != DD_OK,"DDCreateEX")) return(false);
#endif

    hRet = m_pDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
    if(DDError(hRet != DD_OK,"DDSetCoop")) return(false);

    HRESULT hr;

#if 0
    DDSURFACEDESC ddsd;
#else
    DDSURFACEDESC2 ddsd;
#endif
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
#if 0
    DDSURFACEDESC ddsd;
#else
    DDSURFACEDESC2 ddsd;
#endif

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
    Paletteised = (BPP==1) ? true:false;

    WinL=NoWinL; WinR=NoWinR; WinT=NoWinT; WinB=NoWinB;
    if (coleco.NTSC) { WinT-=24; WinB-=24; }

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
#if 0
    DDFrame->Lock(NULL, &DDFrameSurface, DDLOCK_WAIT, NULL);
#else
    DDFrame->Lock(NULL, &DDFrameSurface, DDLOCK_WAIT |  DDLOCK_NOSYSLOCK, NULL);
#endif

    for (int i=0;i<TVH;i++)
        cv_screen[i]=(unsigned int *) DDFrameSurface.lpSurface + (DDFrameSurface.lPitch * i)/4;

    RecalcWinSize();
}

void DDAccurateUpdateDisplay(bool singlestep)
{
    static int framecounter=0;
    HRESULT hRet;
    RECT rDest;

    if (++framecounter > coleco.frameskip || singlestep)
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
#if 0
        DDFrame->Lock(NULL, &DDFrameSurface, DDLOCK_WAIT, NULL);
#else
        DDFrame->Lock(NULL, &DDFrameSurface, DDLOCK_WAIT |  DDLOCK_NOSYSLOCK, NULL);
#endif
}
*/

// -----------------------------------------------------------------------------
// GDI Functions
// -----------------------------------------------------------------------------
void GDIAccurateInit(int resize)
{
    float OrigW, OrigH, ScaleW, ScaleH;

    if (GDIFrame)
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
    if (coleco.NTSC) { WinT-=24; WinB-=24; }

    GDIFrame=new Graphics::TBitmap;

    GDIFrame->Width=TVW;
    GDIFrame->Height=TVH;
    GDIFrame->PixelFormat=pf32bit; // pf16bit;

    // Init Coleco screen with GDI Handle
    for (int i=0;i<TVH;i++)
      cv_screen[i]=(unsigned int *)GDIFrame->ScanLine[i];

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
    RecalcWinSize();
}

void GDIAccurateUpdateDisplay(bool singlestep)
{
    int ret;
    static int framecounter=0;

    if (++framecounter > coleco.frameskip || singlestep)
        framecounter=0;
    else
        return;

    ret = StretchBlt(Form1->Canvas->Handle,
                        rcdest.Left, rcdest.Top,
                        (rcdest.Right-rcdest.Left),
                        (rcdest.Bottom-rcdest.Top),
                        GDIFrame->Canvas->Handle, 0, 0, TVW, TVH,SRCCOPY);

    if (!ret) ShowMessage(SysErrorMessage(GetLastError()));
}

// -----------------------------------------------------------------------------
// Common Functions
// -----------------------------------------------------------------------------
void RecalcWinSize(void)
{
    int sw,sh, dw,dh, scale;
    int bw,bh;

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
#if 0
    if (Form1->RenderMode==RENDERDDRAW)
        return(DDInit());
#endif
    return(1);
}

void RenderEnd(void)
{
    if (GDIFrame)
    {
        delete GDIFrame;
        GDIFrame=NULL;
    }
#if 0
    DDEnd();
#endif
}

void AccurateInit(int resize)
{
#if 0
        if (Form1->RenderMode==RENDERDDRAW)
            DDAccurateInit(resize);
        else
#endif
            GDIAccurateInit(resize);
}

void AccurateUpdateDisplay(bool singlestep)
{
#if 0
    if (Form1->RenderMode==RENDERDDRAW)
        DDAccurateUpdateDisplay(singlestep);
    else
#endif
        GDIAccurateUpdateDisplay(singlestep);
}

/*
int AccurateDraw(SCANLINE *Line)
{
        static int FrameNo=0;
        static int LastVSyncLen=0, Shade=0;
        register int i,c;

        for(i=0; i<Line->scanline_len; i++)
        {
                c=Line->scanline[i];

                Plot(FrameNo*TVP, c); -> #define Plot(x,c) { *(DWORD *)(dest+RasterX+(x)) = Colours[(c)]; }

        LinePtr=cv_screen[iy];
                            unsigned char colour = *(pcv_display+TVW*iy+ix);
            *(LinePtr++)=cv_pal32[colour];


                RasterX += BPP;

                if (RasterX > ScanLen)
                {
                        RasterX=0;
                        RasterY += 1;
                        if (RasterY>=TVH)
                        {
                                i=Line->scanline_len+1;
                                Line->sync_valid=true;
                        }
                }
        }
        if (Line->sync_len<HSYNC_MINLEN) Line->sync_valid=0;
        if (Line->sync_valid)
        {
                if (RasterX>(HSYNC_TOLLERANCE*BPP))
                {
                        RasterX=0;
                        RasterY+= 1;
                        dest += TVP;
                }
                if (RasterY>=TVH ||  RasterY>=VSYNC_TOLLERANCEMAX
                                      ||  (Line->sync_len>VSYNC_MINLEN
                                          && RasterY>VSYNC_TOLLERANCEMIN))
                {
                        RasterX=RasterY=0;
                        FrameNo=0;
                        AccurateUpdateDisplay(false);
                }
        }

        if (zx81.single_step)
        {
                int i;

                for(i=0;i<8;i++) *(DWORD *)(dest+RasterX+i*BPP) = Colours[15];
                AccurateUpdateDisplay(true);
        }
        return(0);
}
*/
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
#if 0
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
#endif
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
#if 0
        }
#endif
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
#if 0
                if (Form1->RenderMode==RENDERDDRAW)
                {
                        StretchBlt(bmp->Canvas->Handle, 0,0, TVW, TVH,
                                DDFrameSurface.lpSurface, 0, 0, TVW,TVH,SRCCOPY);
                }
                else
                {
#endif
                        StretchBlt(bmp->Canvas->Handle, 0,0, TVW, TVH,
                                GDIFrame->Canvas->Handle, 0, 0, TVW,TVH,SRCCOPY);
#if 0
                }
#endif
                ImageToPNG( filename, bmp);
        }
        __finally
        {
                delete bmp; 
        }

}


