//---------------------------------------------------------------------------

#ifndef memory_H
#define memory_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <set>
#include <vector>

//---------------------------------------------------------------------------
static const int SBARWIDTH = 24;

class TPanelCanvasHack : public TPanel
    {
    public:
        __property Canvas;
    };

class RowRenderer
{
public:
        RowRenderer() :
                mLMargin(0),
                mKern(0),
                mCellWidth(1),
                mBytesPerCell(1),
                mDisplayCellsPerRow(1)
        {};

        virtual ~RowRenderer(){};

        virtual void RenderRow(void) = 0;
        virtual void SetGeometry(int, TSize&) = 0;

        virtual bool ByteAtX(const int, int&);
        void RenderColumnHeadings(const TSize& charSize);
        int BytesPerCell() { return mBytesPerCell; };

        // in
        std::set<int>::iterator mDirty;
        std::set<int>::iterator mLast;
        int mAddress, mY, mSelectedAddress;
        HDC mCHDC;

        // out
        int mDisplayCellsPerRow;

protected:
        void SetGeometry(int, const TSize&, int);
        void ChooseTextColour();
        void AddressOut();
        void SetCharacterBackgroundColour(int xpos, int ypos, COLORREF paper, COLORREF ink);

        int mLMargin, mKern;
        int mCellWidth, mBytesPerCell;
};

class TraditionalRowRenderer : public RowRenderer
{
public:
        virtual void RenderRow(void);
        virtual void SetGeometry(int, TSize&);
        virtual bool ByteAtX(const int, int&);
};

//---------------------------------------------------------------------------
class Tmemory : public TForm
{
__published:	// IDE-managed Components
    TPopupMenu *PopupMenu1;
    TMenuItem *RunStop2;
    TMenuItem *SetAddress1;
    TMenuItem *Search1;
    TMenuItem *DumpMemory1;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Memory1;
    TMenuItem *Goto1;
    TMenuItem *Search2;
    TMenuItem *DumpMemory2;
    TPanel *panMem;
    TScrollBar *sbMemory;
    TPanel *pMemory;
    TLabel *Panel4;
    TLabel *Label1;
    TComboBox *cboMemory;
    TUpDown *udMemory;
    TEdit *eMemAdr;
    void __fastcall SetAddress1Click(TObject *Sender);
    void __fastcall cboMemoryChange(TObject *Sender);
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall sbMemoryChange(TObject *Sender);
    void __fastcall panMemMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall panMemClick(TObject *Sender);
    void __fastcall Search1Click(TObject *Sender);
    void __fastcall udMemoryChangingEx(TObject *Sender,
          bool &AllowChange, short NewValue, TUpDownDirection Direction);
    void __fastcall eMemAdrKeyPress(TObject *Sender, char &Key);
    void __fastcall DumpMemory1Click(TObject *Sender);
private:	// User declarations
    RowRenderer* mRowRenderer;
    int mSelectedAddress;
    AnsiString mSearchText;

    void CreateBitmap(void);
    bool FindSequence(std::vector<int>& bytes, int& addr);
    bool FindMatch(int& addr, std::vector<int>& bytes);
    void DoSearch();
    bool PerformSearch(std::vector<int>& bytes);


    // for memory
    void __fastcall SetBaseAddress(int value);
    bool __fastcall xyToAddress(int xIn, int yIn, int& address);
    int mRows;
    int mBaseAddress;
    bool ignoreScrollChange;
    HWND mHWND;
    HBITMAP mOffscreenBitmap;
    int mBMWidth;
    int mBMHeight;
    TSize mCharSize;
    int mHeadingHeight;
    void __fastcall UpdateChanges();
    void __fastcall ClearChanges();

public:		// User declarations
    __fastcall Tmemory(TComponent* Owner);
    __property int BaseAddress  = { write=SetBaseAddress };
    int maxMemory;
};
//---------------------------------------------------------------------------
extern PACKAGE Tmemory *memory;
//---------------------------------------------------------------------------
#endif
