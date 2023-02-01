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

class Tdebug : public TForm
{
__published:	// Composants g�r�s par l'EDI
    TGroupBox *grRegisters;
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
    TGroupBox *grStack;
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
    TMenuItem *Symbols1;
    TOpenDialog *OpenDialog1;
    TMenuItem *View;
    TMenuItem *AutoRefresh1;
    TMenuItem *Refresh1;
        TLabel *Stack07;
        TLabel *Stack08;
        TLabel *Stack09;
    TLabel *Stack10;
    TLabel *Stack11;
    TLabel *Stack12;
    TLabel *Stack13;
        TMenuItem *RunStop3;
    TMenuItem *N2;
    TMenuItem *Memory1;
    TGroupBox *GroupBox3;
    TButton *btMemory;
    TButton *Button1;
    TGroupBox *GroupBox6;
    TButton *Button2;
    TButton *bStepIn;
    TButton *bStepOv;
    TButton *bStepOu;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall RunStop1Click(TObject *Sender);
    void __fastcall Step1Click(TObject *Sender);
    void __fastcall StepOver1Click(TObject *Sender);
    void __fastcall StepOut1Click(TObject *Sender);
    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall Symbols1Click(TObject *Sender);
    void __fastcall LoadSymbolFile1Click(TObject *Sender);

    void __fastcall Stack00Click(TObject *Sender);
    void __fastcall Stack00MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);

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

    void __fastcall AutoRefresh1Click(TObject *Sender);
    void __fastcall Refresh1Click(TObject *Sender);
    void __fastcall AddBreak1Click(TObject *Sender);
    void __fastcall Disass3MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Memory1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:	// D�clarations de l'utilisateur
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

public:		// D�clarations de l'utilisateur
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

};
//---------------------------------------------------------------------------
extern PACKAGE Tdebug *debug;
//---------------------------------------------------------------------------
#endif
