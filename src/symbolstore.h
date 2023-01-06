#ifndef _symbolstore_h_
#define _symbolstore_h_

#include <string>

namespace symbolstore
{
        // all symbol files are assumed to have the basic form
        // A.B where . represents a delimeter, assumed to be
        // either whitespace or '='. they all have subtle diffs, though.
        // a munger function is called after the expression is split
        // and trimmed. the mapper expects whole-word symbols and hex addresses,
        // without any '$' or '0x' introducer.
        //
        typedef void (*SYMBOLMUNGER)(AnsiString& symbol, AnsiString& address);

        // loading a new symbol file clears old symbols,
        // but when resetting a machine file symbols could be left behind.
        // so call this :)
        //
        void reset(void);

        // rom symbols are associated with a ROM. file symbols are
        // associated with a particular P file.
        // returns true if file loaded ok
        // if you want to do special processing on each symbol/value pair as
        // they're parsed, pass in a munger.
        //
        bool loadROMSymbols(std::string& symbname, symbolstore::SYMBOLMUNGER munger);
//        bool loadROMSymbols(const char* filename, SYMBOLMUNGER munger = NULL);
        bool loadFileSymbols(const char* filename, SYMBOLMUNGER munger = NULL);

        // check if symbols are loaded
        //
        bool romLoaded(void);
        bool fileLoaded(void);

        // searches file set first, then ROM set.
        // returns true if the symbol was resolved into result
        //
        bool addressToSymbol(const int addr, AnsiString& result);

        // returns either the resolved symbol or the hex equivalent
        //
        AnsiString addressToSymbolOrHex(const int addr);

        // given a symbol, do a reverse look-up and return its address
        // returns true if val was resolved
        //
        bool symbolToAddress(const AnsiString& sym, int& addr);

        // query the content
        //
        void beginenumeration(void);
        bool enumerate(AnsiString& sym, int& addr, char& type);
};

#endif // _symbolstore_h_
