//---------------------------------------------------------------------------
#ifndef debug_H
#define debug_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <IniFiles.hpp>
#include <Menus.hpp>
#include "breakpoint.h"
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
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

class Tdebug : public TForm
{
__published:	// Composants gérés par l'EDI
    TGroupBox *GroupBox1;
    TLabel *HLL;
    TLabel *BCL;
    TLabel *DEL;
    TLabel *AL;
    TLabel *FL;
    TLabel *HL;
    TLabel *BC;
    TLabel *DE;
    TLabel *A;
    TLabel *F;
    TLabel *HL_L;
    TLabel *HL_;
    TLabel *BC_;
    TLabel *DE_;
    TLabel *A_;
    TLabel *F_;
    TLabel *F_L;
    TLabel *A_L;
    TLabel *DE_L;
    TLabel *BC_L;
    TLabel *IXL;
    TLabel *IX;
    TLabel *IYL;
    TLabel *IY;
    TLabel *IRL;
    TLabel *IR;
    TLabel *SPL;
    TLabel *SP;
    TLabel *PCL;
    TLabel *PC;
    TLabel *IML;
    TLabel *IM;
    TLabel *INTL;
    TLabel *Interrupts;
    TLabel *HALTL;
    TLabel *Halt;
    TLabel *EXL;
    TLabel *EX_L;
    TPopupMenu *MemDumpPopup;
    TMenuItem *OnExecute1;
    TMenuItem *OnRead1;
    TMenuItem *OnWrite1;
    TGroupBox *GroupBox2;
    TLabel *Disass0;
    TLabel *Disass1;
    TLabel *Disass2;
    TLabel *Disass3;
    TLabel *Disass4;
    TLabel *Disass5;
    TLabel *Disass6;
    TLabel *Disass7;
    TLabel *Disass8;
    TLabel *Disass9;
    TLabel *PGLBL;
    TLabel *Disass10;
    TLabel *Disass11;
    TGroupBox *GroupBox5;
        TLabel *Stack00;
        TLabel *Stack01;
        TLabel *Stack02;
        TLabel *Stack03;
        TLabel *Stack04;
        TLabel *Stack05;
        TLabel *Stack06;
    TGroupBox *GroupBox4;
    TLabel *BPTCL;
    TLabel *TStatesCount;
    TButton *AddBrkBtn;
    TButton *DelBrkBtn;
    TStringGrid *BPList;
    TButton *EditBrkBtn;
    TMainMenu *MainMenu1;
    TMenuItem *Debug1;
    TMenuItem *RunStop1;
    TMenuItem *Step1;
    TMenuItem *StepOver1;
    TMenuItem *StepOut1;
    TMenuItem *File1;
    TMenuItem *LoadSymbolFile1;
    TMenuItem *N1;
    TMenuItem *Exit1;
    TMenuItem *ShowSymbols1;
    TOpenDialog *OpenDialog1;
    TMenuItem *View;
    TMenuItem *AutoRefresh1;
    TMenuItem *Refresh1;
    TGroupBox *gBoxMemory;
    TScrollBar *sbMemory;
    TPanel *pMemory;
    TComboBox *cboMemory;
    TUpDown *udMemory;
    TPopupMenu *PopupMenu1;
    TMenuItem *SetAddress1;
    TMenuItem *Search1;
    TPanel *panMem;
    TLabel *Panel4;
        TLabel *Stack07;
        TLabel *Stack08;
        TLabel *Stack09;
    TLabel *Stack10;
    TLabel *Stack11;
    TLabel *Stack12;
    TLabel *Stack13;
        TEdit *eMemAdr;
        TMenuItem *RunStop2;
        TMenuItem *RunStop3;
        TLabel *Label1;
    TMenuItem *DumpMemory1;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall RunStop1Click(TObject *Sender);
    void __fastcall Step1Click(TObject *Sender);
    void __fastcall StepOver1Click(TObject *Sender);
    void __fastcall StepOut1Click(TObject *Sender);
    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall ShowSymbols1Click(TObject *Sender);
    void __fastcall LoadSymbolFile1Click(TObject *Sender);

    void __fastcall Stack00Click(TObject *Sender);
    void __fastcall Stack00MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall cboMemoryChange(TObject *Sender);

    void __fastcall FClick(TObject *Sender);
    void __fastcall F_Click(TObject *Sender);
    void __fastcall BCMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall DEMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall HLMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall IXMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall IYMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall IRMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall SPMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall PCMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall AClick(TObject *Sender);
    void __fastcall A_Click(TObject *Sender);
    void __fastcall SPClick(TObject *Sender);
    void __fastcall PCClick(TObject *Sender);
    void __fastcall BCClick(TObject *Sender);
    void __fastcall DEClick(TObject *Sender);
    void __fastcall HLClick(TObject *Sender);
    void __fastcall BC_Click(TObject *Sender);
    void __fastcall DE_Click(TObject *Sender);
    void __fastcall HL_Click(TObject *Sender);
    void __fastcall IXClick(TObject *Sender);
    void __fastcall IYClick(TObject *Sender);
    void __fastcall IMClick(TObject *Sender);
    void __fastcall InterruptsClick(TObject *Sender);
    void __fastcall IRClick(TObject *Sender);
    void __fastcall AddBrkBtnClick(TObject *Sender);
    void __fastcall DelBrkBtnClick(TObject *Sender);
    void __fastcall EditBrkBtnClick(TObject *Sender);
    void __fastcall BC_MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall DE_MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall HL_MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);

    void __fastcall FormPaint(TObject *Sender);
    void __fastcall sbMemoryChange(TObject *Sender);
    void __fastcall panMemMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall panMemClick(TObject *Sender);
    void __fastcall AutoRefresh1Click(TObject *Sender);
    void __fastcall Refresh1Click(TObject *Sender);
    void __fastcall SetAddress1Click(TObject *Sender);
    void __fastcall Search1Click(TObject *Sender);
    void __fastcall AddBreak1Click(TObject *Sender);
    void __fastcall Disass3MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall udMemoryChangingEx(TObject *Sender,
          bool &AllowChange, short NewValue, TUpDownDirection Direction);
        void __fastcall eMemAdrKeyPress(TObject *Sender, char &Key);
    void __fastcall DumpMemory1Click(TObject *Sender);
private:	// Déclarations de l'utilisateur
    void EnableValues(bool enable);
    void EnableVals(void);
    void DisableVals(void);
    void SetMenuContent(int memloc);

    struct breakpoint Breakpoint[99];
    int Breakpoints;

    AnsiString StrRep(AnsiString Text, int Pos, int Len, AnsiString NewText);

    // default values are evil. never ever use them >:)
    void SetLabelInfo(TLabel* label, int value, int valueWidth = 4);

    void __fastcall DoEditStack(int);
    void __fastcall DoEditReg(WORD&);
    void __fastcall DoEditReg(BYTE&);

    bool IsStepOverInstruction(int Addr);

    int getRegisterValue(int registerIndex);
    AnsiString ConstructRegisterBreakpointText(breakpoint* const bp);
    AnsiString ConstructExeBreakpointText(breakpoint* const bp);
    AnsiString ConstructTStatesBreakpointText(breakpoint* const bp);
    AnsiString ConstructFlagBreakpointText(breakpoint* const bp);
    AnsiString ConstructLowIOBreakpointText(AnsiString type, breakpoint* const bp);
    AnsiString ConstructHighIOBreakpointText(AnsiString type, breakpoint* const bp);
    AnsiString ConstructRdWrInOutMemBreakpointText(AnsiString type, breakpoint* const bp);
    AnsiString GetBreakpointText(breakpoint* const bp);

    bool BPInOutHit(BreakpointType type, int addr, int value, breakpoint* const bp);
    bool BPClockHit(int addr, breakpoint* const bp);
    bool BPReadWriteHit(BreakpointType type, int addr, int value, breakpoint* const bp);
    bool BPFlagValueHit(breakpoint* const bp);
    bool BPMemoryValueHit(breakpoint* const bp);
    bool BPRegisterValueHit(breakpoint* const bp);

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
    RowRenderer* mRowRenderer;
    int mSelectedAddress;
    AnsiString mSearchText;

    void CreateBitmap(void);
    bool FindSequence(std::vector<int>& bytes, int& addr);
    bool FindMatch(int& addr, std::vector<int>& bytes);
    void DoSearch();
    bool PerformSearch(std::vector<int>& bytes);
public:		// Déclarations de l'utilisateur
    __fastcall Tdebug(TComponent* Owner);
    int NMIRetAddr, INTRetAddr;
    void UpdateVals(void);
    bool DoNext;

    bool TStatesBreakPointHit(int Addr);
    void DelTempBreakPoints(void);
    bool BreakPointHit();
    bool BPExeHit(int addr, breakpoint* const bp, int idx);

    bool AddBreakPoint(struct breakpoint& bp);
    void DelBreakPoint(int index);

    AnsiString DisassembleAddress(int* Ad);
    AnsiString Disassemble(int addr, BYTE* bytes);
    AnsiString Disassemble(int *Ad);
    AnsiString Hex16(int value);
    AnsiString Hex8(int value);
    AnsiString Bin8(int Value);
    int Hex2Dec(AnsiString num);

    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);

    int lastPortInAddr, lastPortOutAddr;
    int lastPortInValue, lastPortOutValue;

    __property int BaseAddress  = { write=SetBaseAddress };
    int maxMemory;

    void __fastcall UpdateChanges();
    void __fastcall ClearChanges();
};
//---------------------------------------------------------------------------
extern PACKAGE Tdebug *debug;
//---------------------------------------------------------------------------
#endif
