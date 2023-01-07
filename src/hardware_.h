//---------------------------------------------------------------------------

#ifndef hardware_H
#define hardware_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>
#include <Buttons.hpp>

//---------------------------------------------------------------------------
class Thardware : public TForm
{
__published:	// IDE-managed Components
        TButton *OK;
        TCheckBox *chkNTSC;
        TGroupBox *gSEMachine;
        TSpeedButton *maccol;
        TSpeedButton *macpho;
        TSpeedButton *macadam;
        TButton *bCancel;
        TGroupBox *tSCExpMod;
        TSpeedButton *spSGM;
        TSpeedButton *spF18A;
        void __fastcall OKClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall chkNTSCClick(TObject *Sender);
        void __fastcall maccolClick(TObject *Sender);
        void __fastcall macphoClick(TObject *Sender);
        void __fastcall macadamClick(TObject *Sender);
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
