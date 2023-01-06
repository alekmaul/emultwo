//---------------------------------------------------------------------------

#ifndef hardware_H
#define hardware_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>

//---------------------------------------------------------------------------
class Thardware : public TForm
{
__published:	// IDE-managed Components
        TButton *OK;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TCheckBox *chkNTSC;
        TCheckBox *chkSGM;
        void __fastcall OKClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall chkNTSCClick(TObject *Sender);
private:	// User declarations
        int NewMachine;
        AnsiString NewMachineName;
        bool ResetRequired;
public:		// User declarations
        __fastcall Thardware(TComponent* Owner);
        void SaveSettings(TIniFile *ini);
        void LoadSettings(TIniFile *ini);
};
//---------------------------------------------------------------------------
extern PACKAGE Thardware *hardware;
//---------------------------------------------------------------------------
#endif
