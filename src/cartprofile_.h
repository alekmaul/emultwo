//---------------------------------------------------------------------------

#ifndef cartprofile_H
#define cartprofile_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class Tcartprofile : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TGroupBox *grpMemFootPrint;
        TButton *Button1;
        TScrollBox *ScrollBanks;
        TImage *ImageBanks;
        TImage *iBank;
        TButton *Refresh;
        TRadioButton *EmptyFF;
        TRadioButton *Empty00;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall RefreshClick(TObject *Sender);
        void __fastcall EmptyFFClick(TObject *Sender);
        void __fastcall Empty00Click(TObject *Sender);
private:	// User declarations
        int valEmpty;
        void __fastcall DrawBANK(int x,int y,unsigned char *data,int fill);
        void __fastcall ShowBanks(void);
        void __fastcall ShowProfile(void);
public:		// User declarations
        __fastcall Tcartprofile(TComponent* Owner);
    void LoadSettings(TIniFile *ini);
    void SaveSettings(TIniFile *ini);
};
//---------------------------------------------------------------------------
extern PACKAGE Tcartprofile *cartprofile;
//---------------------------------------------------------------------------
#endif
