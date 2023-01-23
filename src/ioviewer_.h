//---------------------------------------------------------------------------

#ifndef ioviewer_H
#define ioviewer_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Menus.hpp>
#include <IniFiles.hpp>

//---------------------------------------------------------------------------
class Tiomapviewer : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Exit;
    TMenuItem *View1;
    TMenuItem *AutoRefresh1;
    TMenuItem *N1;
    TMenuItem *Refresh1;
    TGroupBox *gPSG;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label21;
    TLabel *Label22;
    TLabel *Label23;
    TLabel *Label24;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TEdit *ePSGR0;
    TEdit *ePSGR1;
    TEdit *ePSGR2;
    TEdit *ePSGR3;
    TEdit *ePSGR4;
    TEdit *ePSGR5;
    TEdit *ePSGR6;
    TEdit *ePSGR7;
    TEdit *ePSGR8;
    TEdit *ePSGR9;
    TEdit *ePSGRA;
    TEdit *ePSGRB;
    TEdit *ePSGRC;
    TEdit *ePSGRD;
    TEdit *ePSGRE;
    TEdit *ePSGRF;
    TEdit *ePSGRG;
    TGroupBox *GroupBox1;
    TLabel *Label9;
    TLabel *lJP1;
    TStaticText *eJoyP0E;
    TStaticText *eJoyP1E;
    TStaticText *eJoyP0;
    TStaticText *eJoyP1;
    TGroupBox *GroupBox2;
    TLabel *Label1;
    TLabel *Label2;
    TStaticText *eSpinP0;
    TStaticText *eSpinP1;
    TGroupBox *GroupBox3;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TStaticText *eP20;
    TStaticText *eP53;
    TStaticText *eP60;
    TLabel *Label6;
    TStaticText *eBank;
    TStaticText *eP60exp;
    TStaticText *eP53exp;
    TStaticText *eP20exp;
    TStaticText *eSGMRAM;
    TLabel *Label7;
    void __fastcall ExitClick(TObject *Sender);
    void __fastcall Refresh1Click(TObject *Sender);
    void __fastcall AutoRefresh1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    void __fastcall UpdateChanges();
public:		// User declarations
    __fastcall Tiomapviewer(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);
    void __fastcall do_refresh();
};
//---------------------------------------------------------------------------
extern PACKAGE Tiomapviewer *iomapviewer;
//---------------------------------------------------------------------------
#endif
