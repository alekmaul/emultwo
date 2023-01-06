//---------------------------------------------------------------------------
#ifndef spriteviewer_H
#define spriteviewer_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Clipbrd.hpp>
#include <IniFiles.hpp>

//---------------------------------------------------------------------------
class Tspriteviewer : public TForm
{
__published:	// Composants gérés par l'EDI
        TListView *LVSprites;
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
        TGroupBox *GroupBox1;
        TLabel *lVVSprCurSAddr;
        TLabel *lVVTilCurSAddr;
        TStaticText *eVVSprCurSAddr;
        TStaticText *eVVSprCurTAddr;
        TPaintBox *sprScreen;
        TPaintBox *SpriteAlone;
        void __fastcall SaveasBMP1Click(TObject *Sender);
        void __fastcall ExitClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall LVSpritesDrawItem(TCustomListView *Sender,
          TListItem *Item, TRect &Rect, TOwnerDrawState State);
        void __fastcall LVSpritesSelectItem(TObject *Sender,
          TListItem *Item, bool Selected);
        void __fastcall AutoRefresh1Click(TObject *Sender);
        void __fastcall Refresh1Click(TObject *Sender);
        void __fastcall Copytoclipboard1Click(TObject *Sender);
private:	// Déclarations de l'utilisateur
    int sprAct;
    int spr8x8;
    int sprdisable;
    int spract;
    void __fastcall RefreshSprite(int address, TCanvas *Acanvas,int w, int h);
    void __fastcall CreateSprite(void);
    void __fastcall UpdateChanges();
public:		// Déclarations de l'utilisateur
    __fastcall Tspriteviewer(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);
    void __fastcall do_refresh();
};
//---------------------------------------------------------------------------
extern PACKAGE Tspriteviewer *spriteviewer;
//---------------------------------------------------------------------------
#endif
