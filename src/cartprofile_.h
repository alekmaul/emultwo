//---------------------------------------------------------------------------

#ifndef cartprofile_H
#define cartprofile_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>

//---------------------------------------------------------------------------
class Tcartprofile : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall Tcartprofile(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);
};
//---------------------------------------------------------------------------
extern PACKAGE Tcartprofile *cartprofile;
//---------------------------------------------------------------------------
#endif
