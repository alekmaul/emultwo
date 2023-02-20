//---------------------------------------------------------------------------

#ifndef keybadam_H
#define keybadam_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class Tadamkeyboard : public TForm
{
__published:	// IDE-managed Components
    TImage *iKeyboard;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
private:	// User declarations
public:		// User declarations
    __fastcall Tadamkeyboard(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tadamkeyboard *adamkeyboard;
//---------------------------------------------------------------------------
#endif
