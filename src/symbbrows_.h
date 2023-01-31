//---------------------------------------------------------------------------
#ifndef symbbrows_H
#define symbbrows_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class Tsymbbrows : public TForm
{
__published:	// Composants gérés par l'EDI
    TListBox *ListBox1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ListBox1DblClick(TObject *Sender);
private:	// Déclarations de l'utilisateur
public:		// Déclarations de l'utilisateur
    __fastcall Tsymbbrows(TComponent* Owner);
    void __fastcall RefreshContent(void);
};
//---------------------------------------------------------------------------
extern PACKAGE Tsymbbrows *symbbrows;
//---------------------------------------------------------------------------
#endif
