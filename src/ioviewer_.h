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
    TGroupBox *GroupBox1;
    TLabel *Label9;
    TLabel *lJP1;
    TStaticText *eJoyP0E;
    TStaticText *eJoyP1E;
    TStaticText *eJoyP0;
    TStaticText *eJoyP1;
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
    TStaticText *eSGMRAM;
    TLabel *Label7;
    TStaticText *eSpinP0;
    TLabel *Label1;
    TStaticText *eSpinP1;
    TLabel *Label2;
    TStaticText *ePSGRL;
    TStaticText *ePSGR0;
    TStaticText *ePSGR1;
    TStaticText *ePSGR2;
    TStaticText *ePSGR3;
    TStaticText *ePSGR4;
    TStaticText *ePSGR5;
    TStaticText *ePSGR6;
    TStaticText *ePSGR7;
    TStaticText *ePSGR8;
    TLabel *Label8;
    TStaticText *ePSGR9;
    TLabel *Label10;
    TStaticText *ePSGRA;
    TLabel *Label20;
    TStaticText *ePSGRB;
    TLabel *Label21;
    TStaticText *ePSGRC;
    TLabel *Label22;
    TStaticText *ePSGRD;
    TLabel *Label23;
    TStaticText *ePSGRE;
    TLabel *Label24;
    TStaticText *ePSGRF;
    TLabel *Label25;
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
