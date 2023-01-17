//---------------------------------------------------------------------------
#ifndef setbreakpoint_H
#define setbreakpoint_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "debug_.h"

//---------------------------------------------------------------------------
class Tsetbreakpoint : public TForm
{
__published:	// Composants gérés par l'EDI
    TLabel *LabelBreakAddress;
    TLabel *LabelConditionValue;
    TLabel *LabelBreakValue;
    TLabel *LabelType;
    TLabel *Label1;
    TComboBox *FlagList;
    TComboBox *RegisterList;
    TButton *OK;
    TButton *Cancel;
    TEdit *BreakAddress;
    TEdit *BreakValue;
    TComboBox *BreakConditionAddr;
    TComboBox *BreakType;
    TComboBox *BreakConditionValue;
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall RegisterListChange(TObject *Sender);
    void __fastcall OKClick(TObject *Sender);
    void __fastcall CancelClick(TObject *Sender);
    void __fastcall BreakAddressChange(TObject *Sender);
    void __fastcall BreakValueChange(TObject *Sender);
    void __fastcall BreakConditionAddrChange(TObject *Sender);
    void __fastcall BreakTypeChange(TObject *Sender);
private:	// Déclarations de l'utilisateur
    bool cancelled;
    bool Register16Bit(RegisterType registerIndex);
    void EnableOkButton();
    void ValidateBreakAddress();
    void ValidateBreakValue();
    void GetBreakAddressLimits(BreakpointType type, int& lowerLimit, int& upperLimit);
    void GetBreakValueLimits(BreakpointType type, BreakpointCondition condition, RegisterType registerIndex, int& lowerLimit, int& upperLimit);
    int GetBreakAddressMaxDigits(BreakpointType type);
    int GetBreakValueMaxDigits(BreakpointType type, BreakpointCondition condition, RegisterType registerIndex);
    void ConfigureBreakpointFields(struct breakpoint& bp);
    bool GetBreakpointFields(struct breakpoint& bp);
    void BreakTypeChangeExe();
    void BreakTypeChangeRdWrInOut();
    void BreakTypeChangeInOutByte();
    void BreakTypeChangeRegister();
    void BreakTypeChangeFlag();
    void BreakTypeChangeMem();
    void BreakTypeChangeTStates();
    void SetConditionList(TComboBox* const conditionList, AnsiString conditions);
    void SetEditBox(TEdit* const editBox, AnsiString defaultText);
    void SetEditBoxLabels(AnsiString breakAddressLabel, AnsiString breakValueLabel);
public:		// Déclarations de l'utilisateur
    __fastcall Tsetbreakpoint(TComponent* Owner);
    void SetTitle(AnsiString& title);
    bool EditBreakpoint(struct breakpoint& bp);
    void CentreOn(TForm* parent);
};
//---------------------------------------------------------------------------
extern PACKAGE Tsetbreakpoint *setbreakpoint;
//---------------------------------------------------------------------------
#endif
