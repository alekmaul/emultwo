//---------------------------------------------------------------------------
#ifndef searchsequence_H
#define searchsequence_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <vector>
//---------------------------------------------------------------------------
class Tsearchsequence : public TForm
{
__published:	// Composants gérés par l'EDI
    TLabel *Label1;
    TEdit *EditSequence;
    TButton *Cancel;
    TButton *Find;
    void __fastcall FormKeyPress(TObject *Sender, char &Key);
    void __fastcall CancelClick(TObject *Sender);
    void __fastcall FindClick(TObject *Sender);
private:	// Déclarations de l'utilisateur
    bool cancelled;
public:		// Déclarations de l'utilisateur
    __fastcall Tsearchsequence(TComponent* Owner);
    void CentreOn(TForm* parent);
    bool EditSequenceBytes(AnsiString& searchText, std::vector<int>& bytes);
    bool ValidateSearchSequence(std::vector<int>& bytes);
};
//---------------------------------------------------------------------------
extern PACKAGE Tsearchsequence *searchsequence;
//---------------------------------------------------------------------------
#endif
