//---------------------------------------------------------------------------

#ifndef dumpmem_H
#define dumpmem_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Clipbrd.hpp>

//---------------------------------------------------------------------------
class Tdumpmem : public TForm
{
__published:	// IDE-managed Components
    TButton *OK;
    TLabel *Label1;
    TEdit *EditVal;
    TMemo *memdump;
    TButton *Save;
    void __fastcall EditValKeyPress(TObject *Sender, char &Key);
    void __fastcall EditValChange(TObject *Sender);
    void __fastcall OKClick(TObject *Sender);
    void __fastcall SaveClick(TObject *Sender);
private:	// User declarations
    int maxValue, curAddress,typememory;
    bool Translate(AnsiString str, int& value);
    int Tdumpmem::get_memvalue(int adrmemory);
public:		// User declarations
    __fastcall Tdumpmem(TComponent* Owner);
    void Dump2(int address, int maxval, int typmem);
    void CentreOn(TForm* parent);
};
//---------------------------------------------------------------------------
extern PACKAGE Tdumpmem *dumpmem;
//---------------------------------------------------------------------------
#endif
