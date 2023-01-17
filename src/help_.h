//---------------------------------------------------------------------------
#ifndef help_H
#define help_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class Thelp : public TForm
{
__published:	// Composants gérés par l'EDI
    TRichEdit *reHelp;
private:	// Déclarations de l'utilisateur
public:		// Déclarations de l'utilisateur
    __fastcall Thelp(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Thelp *help;
//---------------------------------------------------------------------------
#endif
