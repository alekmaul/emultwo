//---------------------------------------------------------------------------

#ifndef printviewer_H
#define printviewer_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>
#include <Clipbrd.hpp>
#include <Menus.hpp>

//---------------------------------------------------------------------------
class Tprintviewer : public TForm
{
__published:	// IDE-managed Components
    TMemo *printdump;
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Copytoclipboard1;
    TMenuItem *N2;
    TMenuItem *Exit;
    TMenuItem *Edit1;
    TMenuItem *clearprint;
    void __fastcall clearprintClick(TObject *Sender);
    void __fastcall Copytoclipboard1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall Tprintviewer(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);
    void SendPrint(byte Data);
};
//---------------------------------------------------------------------------
extern PACKAGE Tprintviewer *printviewer;
//---------------------------------------------------------------------------
#endif
