//---------------------------------------------------------------------------

#ifndef soundviewer_H
#define soundviewer_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <IniFiles.hpp>
#include <Clipbrd.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class Tsoundviewer : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Copytoclipboard1;
    TMenuItem *N2;
    TMenuItem *Exit;
    TMenuItem *clearsn;
    TMenuItem *Edit1;
    TMemo *mSNPlay;
    TMenuItem *View1;
    TMenuItem *AutoRefresh1;
    TMenuItem *N1;
    TMenuItem *Refresh1;
    TGroupBox *GroupBox1;
    TLabel *Label8;
    TLabel *Label7;
    TStaticText *eNCRS;
    TStaticText *eT3GP;
    TStaticText *eT2GP;
    TStaticText *eT1GP;
    TStaticText *eNGP;
    TStaticText *eMPA;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TGroupBox *gSN76;
    TLabel *lTone0;
    TLabel *lsnLatch;
    TLabel *Label36;
    TLabel *Label37;
    TLabel *Label38;
    TLabel *Label39;
    TLabel *Label40;
    TLabel *Label41;
    TLabel *Label42;
    TLabel *Label43;
    TLabel *lTone1;
    TLabel *lTone2;
    TLabel *lNoise;
    TLabel *Label29;
    TProgressBar *pbTone0;
    TProgressBar *pbTone1;
    TProgressBar *pbTone2;
    TProgressBar *pbNoise;
    TStaticText *tsnRLatch;
    TStaticText *tsnR0;
    TStaticText *tsnR1;
    TStaticText *tsnR2;
    TStaticText *tsnR3;
    TStaticText *tsnRRand;
    TStaticText *tsnR4;
    TStaticText *tsnR5;
    TStaticText *tsnR7;
    TStaticText *tsnR6;
    TLabel *Label9;
    TLabel *Label10;
    TMemo *mSTContent;
    TButton *Button1;
    TGroupBox *GroupBox2;
    TMemo *mSN76489;
    TMenuItem *FlushSN1;
    TMenuItem *N3;
    void __fastcall ExitClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall mSN76489KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall clearsnClick(TObject *Sender);
    void __fastcall Refresh1Click(TObject *Sender);
    void __fastcall AutoRefresh1Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Copytoclipboard1Click(TObject *Sender);
    void __fastcall FlushSN1Click(TObject *Sender);
private:	// User declarations
    int nbayval,nbsnval;
    AnsiString bytesn,byteay;
    void __fastcall UpdateChanges();
public:		// User declarations
    __fastcall Tsoundviewer(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);

    void __fastcall do_refresh();

    void __fastcall addsnvalue(byte Data);
    void __fastcall addsnplay(byte Data);
};
//---------------------------------------------------------------------------
extern PACKAGE Tsoundviewer *soundviewer;
//---------------------------------------------------------------------------
#endif
