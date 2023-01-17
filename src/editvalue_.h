//---------------------------------------------------------------------------
#ifndef editvalue_H
#define editvalue_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class Teditvalue : public TForm
{
__published:	// Composants gérés par l'EDI
    TLabel *Label1;
    TEdit *EditVal;
    TButton *OK;
    TButton *Cancel;
    void __fastcall CancelClick(TObject *Sender);
    void __fastcall OKClick(TObject *Sender);
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall EditValChange(TObject *Sender);
private:	// Déclarations de l'utilisateur
    bool cancelled;
    bool Translate(AnsiString str, int& value);
    AnsiString IntToBin(int Value, int digits);
    AnsiString IntToDec(int value);
    int maxValue;
public:		// Déclarations de l'utilisateur
    __fastcall Teditvalue(TComponent* Owner);
    bool Edit2(int& value, int len);
    void CentreOn(TForm* parent);
};
//---------------------------------------------------------------------------
extern PACKAGE Teditvalue *editvalue;
//---------------------------------------------------------------------------
#endif
