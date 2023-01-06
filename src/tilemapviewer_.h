//---------------------------------------------------------------------------
#ifndef tilemapviewer_H
#define tilemapviewer_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <IniFiles.hpp>
#include <Clipbrd.hpp>
#include "PaintBoxGrid.h"

//---------------------------------------------------------------------------
class TTilemapViewer : public TForm
{
__published:	// Composants gérés par l'EDI
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Copytoclipboard1;
    TMenuItem *SaveasBMP1;
    TMenuItem *N2;
    TMenuItem *Exit;
    TMenuItem *View1;
    TMenuItem *AutoRefresh1;
    TMenuItem *N1;
    TMenuItem *Refresh1;
    TSaveDialog *dlgSVPic;
    TPaintBoxGrid *VRam;
    TGroupBox *gCHRTile;
        TStaticText *eGVVCloc;
    TStaticText *eGVVCTNo;
    TStaticText *eGVVCMAddr;
    TStaticText *eGVVCCAddr;
    TStaticText *eGVVCTAddr;
    TMenuItem *Options1;
    TMenuItem *ShowGrid1;
    TMenuItem *ShowTiles1;
        TPaintBoxGrid *TileAlone;
        TLabel *Label1;
    TGroupBox *gVDP;
    TLabel *lVDPR0;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *lVDPStat;
    TLabel *lVDPlatch;
    TStaticText *eVDPlatch;
    TStaticText *eVDPR0;
    TStaticText *eVDPMode;
    TStaticText *eVDPStat;
    TStaticText *eVDPR7;
    TStaticText *eVDPStatus;
    TStaticText *eVDPColF;
    TStaticText *eVDPColB;
    TStaticText *eVDPOption;
    TStaticText *eVDPR1;
    TStaticText *eVDPR2;
    TStaticText *eVDPR3;
    TStaticText *eVDPR4;
    TStaticText *eVDPR5;
    TStaticText *eVDPR6;
    TStaticText *eVDPBGmap;
    TStaticText *eVDPBGtile;
    TStaticText *eVDPBGcolor;
    TStaticText *eVDPSPRdata;
    TStaticText *eVDPSPRtile;
    TLabel *lGVVCXloc;
    TLabel *lGVVCTNo;
    TLabel *lGVVCMAddr;
    TLabel *lGVVCTAddr;
    TLabel *lGVVCCAddr;
    TStaticText *idTiValue;
    TLabel *Label9;
    TStaticText *eGVVCF18No;
    void __fastcall SaveasBMP1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall ExitClick(TObject *Sender);
    
    void __fastcall AutoRefresh1Click(TObject *Sender);
    void __fastcall ShowGrid1Click(TObject *Sender);
    void __fastcall ShowTiles1Click(TObject *Sender);
    void __fastcall VRamMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Copytoclipboard1Click(TObject *Sender);
private:	// Déclarations de l'utilisateur
    Graphics::TBitmap *mOffscreenBitmap;
    Graphics::TBitmap *mOfftileBitmap;

    int mBaseVram,mVramTile;
    void CreateMap(Graphics::TBitmap *Abitmap);
    void CreateTile(void);
public:		// Déclarations de l'utilisateur
    __fastcall TTilemapViewer(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);

    void __fastcall SmallUpdateChanges();
    void __fastcall UpdateChanges();
    void __fastcall ClearChanges();

    void __fastcall do_refresh();
};
//---------------------------------------------------------------------------
extern PACKAGE TTilemapViewer *TilemapViewer;
//---------------------------------------------------------------------------
#endif
