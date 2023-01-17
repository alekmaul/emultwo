//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "joyconf_.h"

#include "colecoconfig.h"
#include "kbstatus_.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TJoyConf *JoyConf;

//---------------------------------------------------------------------------
#define NBKEYDEF         256

static char keydefname[NBKEYDEF][8];
int keyval[2][20];

#define VK_NONE 0x00

#define VK_A    0x41
#define VK_B    0x42
#define VK_C    0x43
#define VK_D    0x44
#define VK_E    0x45
#define VK_F    0x46
#define VK_G    0x47
#define VK_H    0x48
#define VK_I    0x48
#define VK_J    0x4A
#define VK_K    0x4B
#define VK_L    0x4C
#define VK_M    0x4D
#define VK_N    0x4E
#define VK_O    0x4F
#define VK_P    0x50
#define VK_Q    0x51
#define VK_R    0x52
#define VK_S    0x53
#define VK_T    0x54
#define VK_U    0x55
#define VK_V    0x56
#define VK_W    0x57
#define VK_X    0x58
#define VK_Y    0x59
#define VK_Z    0x5A

void initKeyNameTable(void)
{
    for (int i=0;i<NBKEYDEF;i++)
        strcpy(keydefname[i],"");

    strcpy(keydefname[VK_NONE],"none");         // 0x00

    strcpy(keydefname[VK_BACK],"Back");         // 0x08
    strcpy(keydefname[VK_TAB],"Tab");           // 0x09
    strcpy(keydefname[VK_RETURN],"Enter");      // 0x0D
    strcpy(keydefname[VK_SPACE],"Space");       // 0x20

    strcpy(keydefname[VK_PRIOR],"PgUp");        // 0x21
    strcpy(keydefname[VK_NEXT],"PgDn");         // 0x22
    strcpy(keydefname[VK_END],"End");           // 0x23
    strcpy(keydefname[VK_END],"Home");          // 0x24
    strcpy(keydefname[VK_LEFT], "left");        // 0x25
    strcpy(keydefname[VK_UP], "up");            // 0x26
    strcpy(keydefname[VK_RIGHT], "right");      // 0x27
    strcpy(keydefname[VK_DOWN],"down");         // 0x28

    strcpy(keydefname[VK_A],"a");               // 0x41
    strcpy(keydefname[VK_B],"b");               // 0x42
    strcpy(keydefname[VK_C],"c");               // 0x43
    strcpy(keydefname[VK_D],"d");               // 0x44
    strcpy(keydefname[VK_E],"e");               // 0x45
    strcpy(keydefname[VK_F],"f");               // 0x46
    strcpy(keydefname[VK_G],"g");               // 0x47
    strcpy(keydefname[VK_H],"h");               // 0x48
    strcpy(keydefname[VK_I],"i");               // 0x48
    strcpy(keydefname[VK_J],"j");               // 0x4A
    strcpy(keydefname[VK_K],"k");               // 0x4B
    strcpy(keydefname[VK_L],"l");               // 0x4C
    strcpy(keydefname[VK_M],"m");               // 0x4D
    strcpy(keydefname[VK_N],"n");               // 0x4E
    strcpy(keydefname[VK_O],"o");               // 0x4F
    strcpy(keydefname[VK_P],"p");               // 0x50
    strcpy(keydefname[VK_Q],"q");               // 0x51
    strcpy(keydefname[VK_R],"r");               // 0x52
    strcpy(keydefname[VK_S],"s");               // 0x53
    strcpy(keydefname[VK_T],"t");               // 0x54
    strcpy(keydefname[VK_U],"u");               // 0x55
    strcpy(keydefname[VK_V],"v");               // 0x56
    strcpy(keydefname[VK_W],"w");               // 0x57
    strcpy(keydefname[VK_X],"x");               // 0x58
    strcpy(keydefname[VK_Y],"y");               // 0x59
    strcpy(keydefname[VK_Z],"z");               // 0x5A

    strcpy(keydefname[VK_NUMPAD0],"pad 0");     // 0x60
    strcpy(keydefname[VK_NUMPAD1],"pad 1");     // 0x61
    strcpy(keydefname[VK_NUMPAD2],"pad 2");     // 0x62
    strcpy(keydefname[VK_NUMPAD3],"pad 3");     // 0x63
    strcpy(keydefname[VK_NUMPAD4],"pad 4");     // 0x64
    strcpy(keydefname[VK_NUMPAD5],"pad 5");     // 0x65
    strcpy(keydefname[VK_NUMPAD6],"pad 6");     // 0x66
    strcpy(keydefname[VK_NUMPAD7],"pad 7");     // 0x67
    strcpy(keydefname[VK_NUMPAD8],"pad 8");     // 0x68
    strcpy(keydefname[VK_NUMPAD9],"pad 9");     // 0x69

    strcpy(keydefname[VK_MULTIPLY],"pad *");    // 0x6A
    strcpy(keydefname[VK_ADD],"pad +");         // 0x6B
    strcpy(keydefname[VK_SUBTRACT],"pad -");    // 0x6D
    strcpy(keydefname[VK_DECIMAL],"pad .");     // 0x6E
    strcpy(keydefname[VK_DIVIDE],"pad /");      // 0x6F

//    strcpy(keydefname[VK_LSHIFT],"Lshift");     // 0xA0
//    strcpy(keydefname[VK_RSHIFT],"Rshift");     // 0xA1
//    strcpy(keydefname[VK_LCONTROL],"Lctrl");    // 0xA2
//    strcpy(keydefname[VK_RCONTROL],"Rctrl");    // 0xA3
    strcpy(keydefname[VK_SHIFT],"Shift");       // 0x10
    strcpy(keydefname[VK_CONTROL],"Control");   // 0x11
    strcpy(keydefname[VK_INSERT],"Ins");          // 0x2D
    strcpy(keydefname[VK_DELETE],"Del");          // 0x2E
}

//---------------------------------------------------------------------------
__fastcall TJoyConf::TJoyConf(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    initKeyNameTable();

    ini = new TIniFile(machine.inipath);
    LoadSettings(ini);
    delete ini;
}

//---------------------------------------------------------------------------

void TJoyConf::LoadSettings(TIniFile *ini)
{
    int typejoy1,typejoy2,valkey;

    Top = ini->ReadInteger("JOYCFG","Top",Top);
    Left = ini->ReadInteger("JOYCFG","Left",Left);

    typejoy1 = ini->ReadInteger("JOYCFG","TypeJ1",0);
    typejoy2 = ini->ReadInteger("JOYCFG","TypeJ2",0);

    if ((typejoy1<0) || (typejoy1>2)) typejoy1=0;
    cboTypeJoy1->ItemIndex=typejoy1;
    if ( (typejoy2<0) || (typejoy2>2) ) typejoy2=0;
    cboTypeJoy2->ItemIndex=typejoy2;

    // Get assigment for each key
    KeysRead(ini);

    // get keys on current vars
    for (int i=0;i<20;i++)
    {
        keyval[0][i]=JoyP1[i];
        keyval[1][i]=JoyP2[i];
    }

    // Joy 1
    valkey=keyval[0][0];
    eTUP->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][1];
    eTDOWN->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][2];
    eTLEFT->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][3];
    eTRIGHT->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);

    valkey=keyval[0][4];
    eTTR->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][5];
    eTTL->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);

    valkey=keyval[0][6];
    eTDI->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][7];
    eTMU->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);

    valkey=keyval[0][8];
    eTN0->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][9];
    eTN1->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][10];
    eTN2->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][11];
    eTN3->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][12];
    eTN4->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][13];
    eTN5->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][14];
    eTN6->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][15];
    eTN7->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][16];
    eTN8->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[0][17];
    eTN9->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);

    // JOy 2
    valkey=keyval[1][0];
    eTUP2->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][1];
    eTDOWN2->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][2];
    eTLEFT2->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][3];
    eTRIGHT2->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);

    valkey=keyval[1][4];
    eTTR2->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][5];
    eTTL2->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);

    valkey=keyval[1][6];
    eTDI2->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][7];
    eTMU2->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);

    valkey=keyval[1][8];
    eTN02->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][9];
    eTN12->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][10];
    eTN22->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][11];
    eTN32->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][12];
    eTN42->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][13];
    eTN52->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][14];
    eTN62->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][15];
    eTN72->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][16];
    eTN82->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
    valkey=keyval[1][17];
    eTN92->Text=(strlen(keydefname[valkey])>0 ? keydefname[valkey] : keydefname[0]);
}

void TJoyConf::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("JOYCFG","Top",Top);
    ini->WriteInteger("JOYCFG","Left",Left);

    // get keys on current vars
    for (int i=0;i<20;i++)
    {
        JoyP1[i]=keyval[0][i];
        JoyP2[i]=keyval[1][i];
    }
    ini->WriteInteger("JOYCFG","J1Type",cboTypeJoy1->ItemIndex);
    ini->WriteInteger("JOYCFG","J2Type",cboTypeJoy2->ItemIndex);
    KeysWrite(ini);
}

//---------------------------------------------------------------------------

void __fastcall TJoyConf::bOKClick(TObject *Sender)
{
    TIniFile *ini;

    ini = new TIniFile(machine.inipath);
    SaveSettings(ini);
    delete ini;

    Close();
}

//---------------------------------------------------------------------------
void __fastcall TJoyConf::eTUPKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    // Assign key if possible
    if (dynamic_cast < TComponent * > (Sender))
    {
        TEdit *comp = (TEdit *) Sender;
        if ( (Key>0) && (Key<NBKEYDEF) )
        {
            if (strlen(keydefname[Key])>0)
            {
                comp->Text=keydefname[Key];
                keyval[0][comp->Tag]=Key;
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TJoyConf::eTUP2KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
    // Assign key if possible
    if (dynamic_cast < TComponent * > (Sender))
    {
        TEdit *comp = (TEdit *) Sender;
        if ( (Key>0) && (Key<NBKEYDEF) )
        {
            if (strlen(keydefname[Key])>0)
            {
                comp->Text=keydefname[Key];
                keyval[1][comp->Tag]=Key;
            }
        }
    }
}
//---------------------------------------------------------------------------

