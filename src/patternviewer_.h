//---------------------------------------------------------------------------
#ifndef patternviewer_H
#define patternviewer_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <IniFiles.hpp>
#include <Clipbrd.hpp>

//---------------------------------------------------------------------------
class Tpatternviewer : public TForm
{
__published:	// Composants gérés par l'EDI
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *View1;
    TMenuItem *AutoRefresh1;
    TMenuItem *N1;
    TMenuItem *Refresh1;
    TMenuItem *Copytoclipboard1;
    TMenuItem *SaveasBMP1;
    TMenuItem *N2;
    TMenuItem *Exit;
    TLabel *lVRAMOfs;
    TGroupBox *gBGTile;
    TLabel *lBGTTileNo;
    TLabel *lBGTTileAdr;
    TStaticText *eBGTTileNo;
    TStaticText *eBGTTileAdr;
    TSaveDialog *dlgSVPic;
        TStaticText *eVRAMTxt;
        TLabel *Label1;
    TRadioButton *rCol;
    TRadioButton *rBW;
    TStaticText *idTiValue;
    TStaticText *eGVVCF18No;
    TLabel *Label9;
        TPaintBox *TileAlone;
        TCheckBox *chkGrid;
        TPaintBox *VRam;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall SaveasBMP1Click(TObject *Sender);
    void __fastcall AutoRefresh1Click(TObject *Sender);
    void __fastcall Refresh1Click(TObject *Sender);
    void __fastcall VRamMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall ExitClick(TObject *Sender);
        void __fastcall Copytoclipboard1Click(TObject *Sender);
    void __fastcall rColClick(TObject *Sender);
private:	// Déclarations de l'utilisateur
    Graphics::TBitmap *mOffscreenBitmap;
    Graphics::TBitmap *mOfftileBitmap;

    int mBaseVram,mVramTile;
    void CreateBitmap(TCanvas *Acanvas, int w, int h);
    void CreateTile(void);
    void UpdateOfsText(void);
public:		// Déclarations de l'utilisateur
    __fastcall Tpatternviewer(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);

    void __fastcall SmallUpdateChanges();
    void __fastcall UpdateChanges();
    void __fastcall ClearChanges();

    void __fastcall do_refresh();
};
//---------------------------------------------------------------------------
extern PACKAGE Tpatternviewer *patternviewer;
//---------------------------------------------------------------------------
#endif
