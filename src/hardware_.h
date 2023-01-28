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
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class Thardware : public TForm
{
__published:	// IDE-managed Components
        TButton *OK;
        TGroupBox *gSEMachine;
        TSpeedButton *maccol;
        TSpeedButton *macpho;
        TSpeedButton *macadam;
        TButton *bCancel;
        TGroupBox *tSCExpMod;
        TSpeedButton *spSGM;
        TSpeedButton *spF18A;
        TGroupBox *gSEEmul;
        TLabel *lSEFreq;
        TCheckBox *cSEStart;
        TComboBox *cSEPalNtsc;
        TCheckBox *cSENodelay;
        TCheckBox *cSEPFreq;
        TGroupBox *gSEVideo;
        TLabel *lSEPalette;
        TPaintBox *pbox;
        TComboBox *cboPal;
    TComboBox *cboSEDispD;
    TLabel *lSEDispD;
    TGroupBox *GroupBox2;
    TSpeedButton *coSteWhe;
    TSpeedButton *coRolCon;
    TSpeedButton *coSupAct;
        void __fastcall OKClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall chkNTSCClick(TObject *Sender);
        void __fastcall maccolClick(TObject *Sender);
        void __fastcall macphoClick(TObject *Sender);
        void __fastcall macadamClick(TObject *Sender);
        void __fastcall spSGMClick(TObject *Sender);
        void __fastcall cboPalChange(TObject *Sender);
        void __fastcall pboxPaint(TObject *Sender);
    void __fastcall bCancelClick(TObject *Sender);
    void __fastcall cboSEDispDChange(TObject *Sender);
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
