#include <vcl.h>

#include "symbolstore.h"

#include <assert.h>

#include <map>
#include <string>

using namespace std;

typedef map<int, AnsiString> VAL2SYM;
typedef map<AnsiString, int> SYM2VAL;

static VAL2SYM romV2S;
static SYM2VAL romS2V;

static VAL2SYM fileV2S;
static SYM2VAL fileS2V;

void splitnosval(AnsiString& inval, AnsiString& outval)
{
        AnsiString tempVal = inval;
        int wspos = tempVal.LastDelimiter(":");
        if (wspos == 0)
        {
                return;
        }

        AnsiString actualVal(tempVal.SubString(wspos + 1, tempVal.Length() - wspos));

        outval = actualVal;
}

void loadFileSymbolsProxy(const char* path)
{
        AnsiString pp = AnsiString(path).LowerCase();
        if (pp.Length() < 3) return;

        AnsiString ppp = pp.SubString(pp.Length() - 1, 2);
        if (ppp != ".p") return;

        pp += ".sym";
        symbolstore::loadFileSymbols(pp.c_str());
}

static int isHexNumber(const char* t)
{
    int isHexNum;
    int l = strlen(t);
    if (l == 0) {
        return 0;
    }

    if (t[l - 1] == 'h') {
        l--;
    }
    else if (t[0] == '$') {
        t++;
        l--;
    }

    isHexNum = l > 0;

    while (--l >= 0) {
        isHexNum &= (t[l] >= '0' && t[l] <= '9') ||
                    (t[l] >= 'A' && t[l] <= 'F') ||
                    (t[l] >= 'a' && t[l] <= 'f');
    }
    return isHexNum ? strlen(t) : 0;
}

static bool splitline(const char* input, AnsiString& symOut, AnsiString& valOut)
{
#if 0
        AnsiString sym(input);
        sym = sym.Trim();
        if (sym.Length() == 0 || sym[1] == ';')
        {
                return false;
        }

        // take the last word from the line leaving the remainder in sym
        //
        int wspos = sym.LastDelimiter(",= \t");
        if (wspos == 0)
        {
                return false;
        }

        AnsiString val(sym.SubString(wspos + 1, sym.Length() - wspos));

        // chop any middle words from the line, leaving the symbol name
        //
        wspos = sym.LastDelimiter(",= \t");
        while (wspos != 0)
        {
                sym.SetLength(wspos - 1);
                sym = sym.Trim();
                wspos = sym.LastDelimiter(",= \t");
        }
        symOut = sym;
        valOut = val;
#else
    char lineBuffer[512];
    strcpy(lineBuffer, input);
    char* t1 = strtok(lineBuffer, "\r\n\t ");
    char* t2  = strtok(NULL, "\r\n\t ");
    if (t2 && 0 == strcmp(t2, "equ")) {
        t2  = strtok(NULL, "\r\n\t ");
    }
    if (t1 && t2) {
        char* label;
        char* addr;
        if (isHexNumber(t1) > isHexNumber(t2)) {
            addr  = t1;
            label = t2;
        }
        else {
            addr  = t2;
            label = t1;
        }
        int labelLen = strlen(label);
        if (label[labelLen - 1] == ':') {
            label[labelLen - 1] = 0;
        }
        int address;
        int count = sscanf(addr, "%xh", &address);
        if (count == 0) {
            count = sscanf(addr, "$%x", &address);
        }
        if (count == 1 && labelLen) {
            symOut = label;
            valOut = IntToStr(address);
        }
        else
            return false;
    }
#endif
        return true;
}

void symbolstore_test(void)
{
        AnsiString sym, val;
        assert(!splitline("", sym, val));
        assert(!splitline("  \t", sym, val));
        assert(!splitline("  \t;", sym, val));
        assert(!splitline(";", sym, val));
        assert(!splitline("bert", sym, val));
        assert(!splitline("  bert\t ", sym, val));
        assert(!splitline(" \tbert", sym, val));
        assert(!splitline("tbert  ", sym, val));

        assert(splitline("bert ernie", sym, val));
        assert(sym == "bert" && val == "ernie");

        assert(splitline("bart\tmaggie", sym, val));
        assert(sym == "bart" && val == "maggie");

        assert(splitline("barn \t= horse", sym, val));
        assert(sym == "barn" && val == "horse");

        assert(splitline("burn equ fire", sym, val));
        assert(sym == "burn" && val == "fire");

        assert(splitline("burp \tequ something rude", sym, val));
        assert(sym == "burp" && val == "rude");
}

static void clearROMSymbols(void)
{
        romV2S.clear();
        romS2V.clear();
}

static void clearFileSymbols(void)
{
        fileV2S.clear();
        fileS2V.clear();
}

void symbolstore::reset(void)
{
        clearROMSymbols();
        clearFileSymbols();
}

static bool loadSymbols(const char* filename, VAL2SYM& v2s, SYM2VAL& s2v, symbolstore::SYMBOLMUNGER munger)
{
        FILE* symfile = fopen(filename, "r");
        if (!symfile)
        {
                return false;
        }

        char buf[128];
        AnsiString sym, val;

        while(fgets(buf, 128, symfile))
        {
                if (!splitline(buf,  sym, val))
                {
                        continue;
                }

                if (munger)
                {
                        munger(sym, val);
                }

                splitnosval(val, val);

                if (val.Trim()!="")
                {
                    val = val.Trim(); //"0x" + val.Trim();
                    int v = StrToInt(val);
                    v2s[v] = sym;
                    s2v[sym] = v;
                }
        }

        return true;
}

static bool loadSymbolsMem(string& buffer, VAL2SYM& v2s, SYM2VAL& s2v, symbolstore::SYMBOLMUNGER munger)
{
    char buf[128];
    AnsiString sym, val;

    bool lastLine = false;
    int index = 0;

    while (!lastLine) {
        int nextIndex = buffer.find('\n', index);

        lastLine = nextIndex ==(int) string::npos;

        string line = lastLine ? buffer.substr(index) : buffer.substr(index, nextIndex - index);

        if (line.length() > 0) {
            strcpy(buf, line.c_str());
            if (!splitline(buf,  sym, val))
            {
                continue;
            }

            if (munger)
            {
                munger(sym, val);
            }

            splitnosval(val, val);

            val = "0x" + val.Trim();
            int v = StrToInt(val);
            v2s[v] = sym;
            s2v[sym] = v;
        }

        index = nextIndex + 1;
    }
    return true;
}

bool symbolstore::loadROMSymbols(string& symbname, symbolstore::SYMBOLMUNGER munger)
{
        clearROMSymbols();
        return loadSymbolsMem(symbname, romV2S, romS2V, munger);
}

bool symbolstore::loadFileSymbols(const char* filename, symbolstore::SYMBOLMUNGER munger)
{
        clearFileSymbols();
        return loadSymbols(filename, fileV2S, fileS2V, munger);
}

bool symbolstore::addressToSymbol(const int addr, AnsiString& result)
{
        if (addr == 0) return false;  // zero is always 0

        VAL2SYM::iterator it = fileV2S.find(addr);
        if (it != fileV2S.end())
        {
                result = (*it).second;
                return true;
        }

        it = romV2S.find(addr);
        if (it != romV2S.end())
        {
                result = (*it).second;
                return true;
        }

        return false;
}

AnsiString symbolstore::addressToSymbolOrHex(const int addr)
{
        AnsiString temp;
        if (!addressToSymbol(addr, temp))
        {
                temp = /*"$" + */temp.IntToHex(addr,4);
        }

        return temp;
}

// NUNS! REVERSE!
//
bool symbolstore::symbolToAddress(const AnsiString& sym, int& val)
{
        SYM2VAL::iterator it = fileS2V.find(sym);
        if (it != fileS2V.end())
        {
                val = (*it).second;
                return true;
        }

        it = romS2V.find(sym);
        if (it != romS2V.end())
        {
                val = (*it).second;
                return true;
        }

        return false;
}


// NOT THREAD SAFE! actually - none of it is :P
//
static SYM2VAL::iterator nasty;
static char type;

void symbolstore::beginenumeration(void)
{
        nasty = romS2V.begin();
        type = 'r';
}

bool symbolstore::enumerate(AnsiString& sym, int& val, char& storetype)
{
        if (nasty == romS2V.end())
        {
                type = 'p';
                nasty = fileS2V.begin();
        }
        if (nasty == fileS2V.end())
        {
                return false;
        }

        sym = (*nasty).first;
        val = (*nasty).second;
        storetype = type;

        ++nasty;

        return true;
}

// this is cheesy
//
bool symbolstore::romLoaded(void)
{
        return romV2S.size() != 0;
}

bool symbolstore::fileLoaded(void)
{
        return fileV2S.size() != 0;
}

