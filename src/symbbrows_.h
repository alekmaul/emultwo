//---------------------------------------------------------------------------
#ifndef symbbrows_H
#define symbbrows_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TSymbBrows : public TForm
{
__published:	// Composants g�r�s par l'EDI
    TListBox *ListBox1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ListBox1DblClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// D�clarations de l'utilisateur
public:		// D�clarations de l'utilisateur
    __fastcall TSymbBrows(TComponent* Owner);
    void __fastcall RefreshContent(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TSymbBrows *SymbBrows;
//---------------------------------------------------------------------------
#endif
