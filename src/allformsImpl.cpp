    // Create all forms
    Application->CreateForm(__classid(TForm1), &Form1);
    Application->CreateForm(__classid(Tabout), &about);
    Application->CreateForm(__classid(Tcartprofile), &cartprofile);
    Application->CreateForm(__classid(Tdebug), &debug);
    Application->CreateForm(__classid(Tdumpmem),&dumpmem);
    Application->CreateForm(__classid(Tdumpmem), &dumpmem);
    Application->CreateForm(__classid(Teditvalue), &editvalue);
    Application->CreateForm(__classid(Tiomapviewer), &iomapviewer);
    Application->CreateForm(__classid(Tjoyconf), &joyconf);
    Application->CreateForm(__classid(Tnametabviewer), &nametabviewer);
    Application->CreateForm(__classid(Tpaletteviewer), &paletteviewer);
    Application->CreateForm(__classid(Tpatternviewer), &patternviewer);
    Application->CreateForm(__classid(Tprintviewer), &printviewer);
    Application->CreateForm(__classid(Tsearchsequence), &searchsequence);
    Application->CreateForm(__classid(Tsetbreakpoint), &setbreakpoint);
    Application->CreateForm(__classid(Tsoundviewer), &soundviewer);
    Application->CreateForm(__classid(Tspriteviewer), &spriteviewer);
    Application->CreateForm(__classid(Tsymbbrows), &symbbrows);

    // Always do this last
    Application->CreateForm(__classid(Thardware), &hardware);

