//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "z80.h"

#include "memory_.h"

#include "editvalue_.h"
#include "searchsequence_.h"

#include "coleco.h"
#include "dumpmem_.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tmemory *memory;

std::set<int> dirtyBird;
std::set<int>::iterator changeCursor;

//---------------------------------------------------------------------------
__fastcall Tmemory::Tmemory(TComponent* Owner)
    : TForm(Owner)
{
    maxMemory = 0xFFFF+1;
    cboMemory->ItemIndex=0;
    BaseAddress = Z80.pc.d;

    // Memory part
    mHWND = this->panMem->Handle;
    mOffscreenBitmap = NULL;
    mRowRenderer = NULL;
    mSelectedAddress = -1;
    ignoreScrollChange = false;
    mRowRenderer = new TraditionalRowRenderer; // SetViewMode(MWVM_TRADITIONAL);
    mCharSize = Canvas->TextExtent(AnsiString("0"));
    mHeadingHeight = mCharSize.cy + (mCharSize.cy / 2);
}
//---------------------------------------------------------------------------

int get_memoryvalue(int adrmemory)
{
    int by=0;
    switch (memory->cboMemory->ItemIndex)
    {
    case 0 : by=getbyte(adrmemory); break; // ROM
    case 1 : by=coleco_gettmsval(VRAM,adrmemory,0,0); break; // VRAM
    case 2 : by=coleco_gettmsval(RAM,adrmemory,0,0); break; // RAM
    case 3 : by=coleco_gettmsval(SGMRAM,adrmemory,0,0); break; // SGM RAM
    case 4 : by=coleco_gettmsval(EEPROM,adrmemory,0,0); break; // EEPROM
    case 5 : by=coleco_gettmsval(SRAM,adrmemory,0,0); break; // SRAM
    }

    return by;
}
//---------------------------------------------------------------------------

void set_memoryvalue(int adr, int val)
{
    switch (memory->cboMemory->ItemIndex)
    {
    case 0 : coleco_setval(ROM, adr,val); break; // ROM
    case 1 : coleco_setval(VRAM, adr,val); break; // VRAM
    case 2 : coleco_setval(RAM, adr,val); break; // RAM
    case 3 : coleco_setval(SGMRAM, adr,val); break; // SGM RAM
    case 4 : coleco_setval(EEPROM, adr,val); break; // EEPROM
    case 5 : coleco_setval(SRAM, adr,val); break; // SRAM
    }
}
//---------------------------------------------------------------------------

void RowRenderer::ChooseTextColour()
{
    if (mDirty != mLast && mAddress == *mDirty)
    {
        SetTextColor(mCHDC, RGB(200,0,0));
        ++mDirty;
    }
    else
    {
        SetTextColor(mCHDC, GetSysColor(COLOR_WINDOWTEXT));
    }
}
//---------------------------------------------------------------------------

void RowRenderer::AddressOut(void)
{
    SetTextColor(mCHDC, GetSysColor(COLOR_WINDOWTEXT));
    if (mAddress >= 0)
    {
        TextOut(mCHDC, 0, mY, AnsiString::IntToHex(mAddress,4).c_str(), 4);
    }
    else
    {
        TextOut(mCHDC, 0, mY, AnsiString::IntToHex(65536 + mAddress,4).c_str(), 4);
    }
}
//---------------------------------------------------------------------------

bool RowRenderer::ByteAtX(const int x, int& byte)
{
    if (x <= mLMargin || x >= mLMargin + (mDisplayCellsPerRow * mCellWidth))
    {
        return false;
    }

    byte = ((x - mLMargin) / mCellWidth) * mBytesPerCell;
    return true;
}
//---------------------------------------------------------------------------

void RowRenderer::SetGeometry(int width, const TSize& charSize, int nCharsPerCell)
{
    mKern = charSize.cx / 2;
    mCellWidth = charSize.cx * (nCharsPerCell + 1);
    mLMargin = (charSize.cx * 6);

    int availWidth = width - mLMargin - SBARWIDTH;

    mDisplayCellsPerRow = availWidth / mCellWidth;

    if (mDisplayCellsPerRow * mCellWidth + (charSize.cx * nCharsPerCell) < availWidth)
    {
        ++mDisplayCellsPerRow;
    }
}
//---------------------------------------------------------------------------

void RowRenderer::RenderColumnHeadings(const TSize& charSize)
{
    int columnInset = ((mCellWidth - charSize.cy) / 2) + mLMargin;

    for (int x = 0; x < mDisplayCellsPerRow; ++x)
    {
		AnsiString heading = AnsiString::IntToHex(x, 2);
        TextOut(mCHDC, (x * mCellWidth) + columnInset, 0, heading.c_str(), 2);
    }
}
//---------------------------------------------------------------------------

bool TraditionalRowRenderer::ByteAtX(const int x, int& byte)
{
    int basex = mLMargin + (mDisplayCellsPerRow * mCellWidth) + mCellWidth;

    if (x >= basex && x < basex + (mDisplayCellsPerRow * 8))
    {
        byte = (x - basex) / 8;
    }
    else if (x <= mLMargin || x >= mLMargin + (mDisplayCellsPerRow * mCellWidth))
    {
        return false;
    }
    else
    {
        byte = (x - mLMargin) / mCellWidth;
    }

    return true;
}
//---------------------------------------------------------------------------

void TraditionalRowRenderer::SetGeometry(int width, TSize& charSize)
{
    RowRenderer::SetGeometry(width, charSize, 2);

    double availableWidth = width - mLMargin - mCellWidth - SBARWIDTH;

    float cells = 8 + mCellWidth;
    int Rw = int(availableWidth * (8 / cells));
    int Lw = int(availableWidth) - Rw;

    mDisplayCellsPerRow = Lw / mCellWidth;
}
//---------------------------------------------------------------------------

void TraditionalRowRenderer::RenderRow(void)
{
    HDC another = CreateCompatibleDC(mCHDC);
    HGDIOBJ oldBM = NULL;

    COLORREF prevBackgroundColour = GetBkColor(mCHDC);
    COLORREF paper = RGB(255,255,0);
    COLORREF ink = RGB(0,0,255);

    AddressOut();
    int basex = mLMargin + (mDisplayCellsPerRow * mCellWidth) + mCellWidth;
    for (int x = 0; x < mDisplayCellsPerRow; ++x)
    {
        ChooseTextColour();

        int by=get_memoryvalue(mAddress);
        if ((mAddress >= 0x0000) && (mAddress <= memory->maxMemory-1)) // 0xFFFF))
        {
            if (mSelectedAddress == mAddress)
            {
                SetBkColor(mCHDC, paper);
                SetTextColor(mCHDC, ink);
            }

            TextOut(mCHDC, x * mCellWidth + mLMargin + mKern, mY,
                AnsiString::IntToHex(by, 2).c_str(), 2);

            if ( (by>=32) && (by<=127) )
            {
                char W=static_cast<char>(by);
                TextOut(mCHDC, x * 8 + basex, mY, &W, 1);
            }
            else
                TextOut(mCHDC, x * 8 + basex, mY, ".", 1);

            if (mSelectedAddress == mAddress)
            {
                SetBkColor(mCHDC, prevBackgroundColour);
            }
        }
        ++mAddress;
    }

    SelectObject(another, oldBM);
    DeleteDC(another);

    SetBkColor(mCHDC, prevBackgroundColour);
}
//---------------------------------------------------------------------------

void __fastcall Tmemory::cboMemoryChange(TObject *Sender)
{
    // change max for memory
    switch (cboMemory->ItemIndex)
    {
    case 0 : // ROM
        maxMemory = 0xFFFF+1;
        break;
    case 1 : // VRAM
        maxMemory = 0x3FFF+1;
        break;
    case 2 : // RAM
        maxMemory = 0x3FF+1;
        break;
    case 3 : // SGM RAM
        maxMemory = 0x7FFF+1;
        break;
    case 4 : // EEPROM
        maxMemory = 0x7FFF+1;
        break;
    case 5 : // SRAM
        maxMemory = 0x7FF+1;
        break;
    }
    UpdateChanges();
}
//---------------------------------------------------------------------------

void Tmemory::CreateBitmap(void)
{
    if (mOffscreenBitmap)
    {
        ::DeleteObject(mOffscreenBitmap);
    }

    RECT rect;
    ::GetClientRect(mHWND, (LPRECT) &rect);

    mBMWidth = rect.right - rect.left;
    mBMHeight = rect.bottom - rect.top/* - StatusBar1->Height*/;
    if (mBMWidth < 1)
        mBMWidth  = 1;
    if (mBMHeight < 1)
        mBMHeight  = 1;

    HDC hdc = (HDC) Canvas->Handle;
    HDC chdc = CreateCompatibleDC(hdc);

    mOffscreenBitmap = ::CreateCompatibleBitmap(hdc, mBMWidth, mBMHeight);
    HGDIOBJ oldbm = SelectObject(chdc, mOffscreenBitmap);

    HGDIOBJ oldfont = SelectObject(chdc, ((TPanelCanvasHack*)panMem)->Canvas->Font->Handle);


    FillRect(chdc, &rect, (HBRUSH)(COLOR_BTNFACE+1));

    SetBkColor(chdc, GetSysColor(COLOR_BTNFACE));

    mRowRenderer->SetGeometry(mBMWidth, mCharSize);

    mRows = (mBMHeight - mHeadingHeight) / mCharSize.cy;

    int bytesPerRow = mRowRenderer->BytesPerCell() * mRowRenderer->mDisplayCellsPerRow;

    sbMemory->SmallChange = bytesPerRow;
    sbMemory->LargeChange = 8 * sbMemory->SmallChange;

    sbMemory->Min = 0;
    sbMemory->Max = maxMemory - (mRows * bytesPerRow) + bytesPerRow - 1;

    mRowRenderer->mAddress = mBaseAddress;
    mRowRenderer->mSelectedAddress = mSelectedAddress;
    mRowRenderer->mDirty = dirtyBird.lower_bound(mRowRenderer->mAddress);
    mRowRenderer->mLast = dirtyBird.end();
    mRowRenderer->mCHDC = chdc;
    mRowRenderer->mY = mHeadingHeight;

    mRowRenderer->RenderColumnHeadings(mCharSize);

    for (int i = 0; i < mRows; ++i)
    {
        mRowRenderer->RenderRow();
        mRowRenderer->mY += mCharSize.cy;
    }

    SelectObject(chdc, oldfont);
    SelectObject(chdc, oldbm);
    DeleteDC(chdc);
}
//---------------------------------------------------------------------------

void __fastcall Tmemory::FormPaint(TObject *Sender)
{
    if (!mOffscreenBitmap)
    {
        CreateBitmap();
    }

    HDC hdc = (HDC)  ((TPanelCanvasHack*)panMem)->Canvas->Handle;
    HDC chdc = CreateCompatibleDC(hdc);
    HGDIOBJ oldbm = SelectObject(chdc, mOffscreenBitmap);
    ::BitBlt(hdc, 0, 0, mBMWidth, mBMHeight, chdc, 0, 0, SRCCOPY);
    SelectObject(chdc, oldbm);
    DeleteDC(chdc);
}
//---------------------------------------------------------------------------

void __fastcall Tmemory::SetBaseAddress(int value)
{
    mSelectedAddress = -1;
    ignoreScrollChange = true;
    sbMemory->Position = value;
    ignoreScrollChange = false;
}

//---------------------------------------------------------------------------

void __fastcall Tmemory::ClearChanges()
{
    dirtyBird.clear();
    UpdateChanges();
}

//---------------------------------------------------------------------------

void __fastcall Tmemory::UpdateChanges()
{
    if (mOffscreenBitmap)
    {
        ::DeleteObject(mOffscreenBitmap);
        mOffscreenBitmap = NULL;
    }

    Invalidate();

    changeCursor = dirtyBird.lower_bound(mBaseAddress);
}

//---------------------------------------------------------------------------

void __fastcall Tmemory::sbMemoryChange(TObject *Sender)
{
    if ((sbMemory->Position > 0) && !ignoreScrollChange)
    {
        int delta = sbMemory->Position - mBaseAddress;
        int bytesPerRow = mRowRenderer->BytesPerCell() * mRowRenderer->mDisplayCellsPerRow;
        int offset = delta % bytesPerRow;

        sbMemory->Position -= offset;
    }

    mBaseAddress = sbMemory->Position;

    UpdateChanges();
}

//---------------------------------------------------------------------------
bool  __fastcall Tmemory::xyToAddress(int xIn, int yIn, int& address)
{
    int x;
    int y = (yIn - mHeadingHeight) / mCharSize.cy;

    if (!mRowRenderer->ByteAtX(xIn, x) || yIn < mHeadingHeight || yIn >= mHeadingHeight + (mRows * mCharSize.cy))
    {
        return false;
    }

    address = mBaseAddress + x + (y * mRowRenderer->mDisplayCellsPerRow * mRowRenderer->BytesPerCell());

    return (address <= 0xFFFF);
}
//---------------------------------------------------------------------------

void __fastcall Tmemory::panMemMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    int address;
    AnsiString t;
    mSelectedAddress = -1;

    if (xyToAddress(X,Y,address))
    {
        t = AnsiString::IntToHex(address,4);
        mSelectedAddress = address;
    }

    Panel4->Caption = t;

    UpdateChanges();
}
//---------------------------------------------------------------------------

void __fastcall Tmemory::panMemClick(TObject *Sender)
{
    int address;

    // prise du point de référence de la souris
    TPoint cpm;
    GetCursorPos(&cpm);
    TPoint cp = ScreenToClient(cpm);

    // CR  this is nasty - but it's the quickest way to tell if the debugger
    // is running continuously
    if (emul2.singlestep!=1)  return;

    //if (xyToAddress(cp.x-gBoxMemory->Left-panMem->Left, cp.y-gBoxMemory->Top-panMem->Top, address))
    if (xyToAddress(cp.x-Left-panMem->Left, cp.y-Top-panMem->Top, address))
    {
        editvalue->Top = cpm.y;//Mouse->CursorPos.y;
        editvalue->Left = cpm.x;//Mouse->CursorPos.x;

        int n = get_memoryvalue(address);
        if (editvalue->Edit2(n, 1))
        {
            set_memoryvalue(address,n);
            UpdateChanges();  // CR  refresh after edit
        }
    }
}

//---------------------------------------------------------------------------

void __fastcall Tmemory::SetAddress1Click(TObject *Sender)
{
    editvalue->CentreOn(this);

    int v = SetAddress1->Tag;
    if (editvalue->Edit2(v, 2))
    {
        SetBaseAddress(v);
    }
}

//---------------------------------------------------------------------------

bool Tmemory::FindSequence(std::vector<int>& bytes, int& addr)
{
    bool found;

    do
    {
        found = FindMatch(addr, bytes);
    }
    while (!found && (addr < maxMemory));

    return found;
}
//---------------------------------------------------------------------------

bool Tmemory::FindMatch(int& addr, std::vector<int>& bytes)
{
    int firstByte = bytes[0];

    while ((addr < maxMemory) && (getbyte(addr) != firstByte))
    {
        ++addr;
    }

    if (addr == maxMemory)
    {
        return false;
    }

    for (int b = 1; b < (int)bytes.size(); b++)
    {
        ++addr;
        int byte = getbyte(addr);

        if (byte != bytes[b])
        {
            return false;
        }
    }

    addr -= bytes.size() - 1;

    return true;
}
//---------------------------------------------------------------------------

void Tmemory::DoSearch()
{
    searchsequence->CentreOn(this);

    std::vector<int> bytes;
    bool performSearch = searchsequence->EditSequenceBytes(mSearchText, bytes);
    if (performSearch)
    {
        if (!PerformSearch(bytes))
        {
            AnsiString mErrortxt="Can't find '"+mSearchText+"'";
            Application->MessageBox(mErrortxt.c_str(),"Error", MB_OK | MB_ICONERROR);
        }
    }
    else
        Application->MessageBox("Invalid characters","Error", MB_OK | MB_ICONERROR);
}
//---------------------------------------------------------------------------

bool Tmemory::PerformSearch(std::vector<int>& bytes)
{
    int address = mBaseAddress + 1;

    if ((address < maxMemory) && FindSequence(bytes, address))
    {
        SetBaseAddress(address);
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------

void __fastcall Tmemory::Search1Click(TObject *Sender)
{
    DoSearch();
}

//---------------------------------------------------------------------------

void __fastcall Tmemory::udMemoryChangingEx(TObject *Sender,
      bool &AllowChange, short NewValue, TUpDownDirection Direction)
{
    AllowChange = false;
    SetBaseAddress(mBaseAddress + NewValue);
}
//---------------------------------------------------------------------------

void __fastcall Tmemory::eMemAdrKeyPress(TObject *Sender, char &Key)
{
    if (Key == VK_RETURN)
    {
        char* endPtr;
        int entryValue = int(strtol(eMemAdr->Text.c_str(), &endPtr, 16));
        if ( ((*endPtr == 0) || (*endPtr == 0x14))  && (entryValue >= 0) && (entryValue <= maxMemory))
        {
            SetBaseAddress(entryValue);
        }
        else
          eMemAdr->Text="";
    }
}
//---------------------------------------------------------------------------

void __fastcall Tmemory::DumpMemory1Click(TObject *Sender)
{
    int address;

    // Get mouse pointer reference
    TPoint cpm;
    GetCursorPos(&cpm);
    TPoint cp = ScreenToClient(cpm);

    // if got it, dump memory regarding type of memory
    if (mSelectedAddress!=-1)
    {
        address=mSelectedAddress;
        dumpmem->CentreOn(this);
        switch (cboMemory->ItemIndex)
        {
        case 0 : // ROM
            dumpmem->Dump2(address, 0xFFFF+1,0);
            break;
        case 1 : // VRAM
            dumpmem->Dump2(address, 0x3FFF+1,1);
            break;
        case 2 : // RAM
            dumpmem->Dump2(address, 0x3FF+1,2);
            break;
        case 3 : // SGM RAM
            dumpmem->Dump2(address, 0x7FFF+1,3);
            break;
        }
    }
}
//---------------------------------------------------------------------------

