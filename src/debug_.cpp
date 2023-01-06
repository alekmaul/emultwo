/* EmulTwo  - A Windows Colecovision emulator.
 * Copyright (C) 2018-2019 Alekmaul
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *
 * debug_.cpp
 *
 * Lots of parts inspired from EightyOne Sinclair Emulator
 *   https://sourceforge.net/projects/eightyone-sinclair-emulator/
 *
 */


//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "debug_.h"

#include "main_.h"
#include "coleco.h"
#include "colecoconfig.h"
#include "z80.h"

#include "dbgdisassemb.cpp"

//#include "profiler_.h"
#include "dumpmem_.h"
#include "editvalue_.h"
#include "searchsequence_.h"
#include "setbreakpoint_.h"
#include "accsound_.h"
#include "symbbrows_.h"

#include <set>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tdebug *debug;

std::set<int> dirtyBird;
std::set<int>::iterator changeCursor;

extern int tStatesCount;

//---------------------------------------------------------------------------
int StepOverStack;
int StepOverInstructionSize;
int StepOverAddr;
bool StepOverInstruction;
int StepOutRequested;
int RetExecuted;
int StackChange;

const int maxInstructionBytes = 4;		// Max instruction size should be 4 but in theory could be longer if there are repeated prefixes

struct InstructionEntry
{
	int Address;
	BYTE Bytes[maxInstructionBytes];
};

int displayedTStatesCount;

int recentHistory[4];
int recentHistoryPos=0;

//---------------------------------------------------------------------------
void DebugUpdate(void)
{
    static int NMISaveSingleStep=-1, INTSaveSingleStep=-1;
    static int lastpc;
    int i;

    if (lastpc!=z80.pc.w)
    {
        recentHistory[recentHistoryPos] = i;
        recentHistoryPos = (recentHistoryPos + 1) & 3;
        lastpc=z80.pc.w;
    }

    i=z80.pc.w;
    if ( (debug->NMIRetAddr==-1) && (NMISaveSingleStep!=-1) )
    {
        coleco.singlestep=NMISaveSingleStep;
        NMISaveSingleStep=-1;
    }
    if ( (debug->INTRetAddr==-1) && (INTSaveSingleStep!=-1) )
    {
        coleco.singlestep=INTSaveSingleStep;
        INTSaveSingleStep=-1;
    }
    /*if (z80.pc.w==0x66 && Debug->SkipNMIBtn->Checked
        && Debug->NMIRetAddr==-1 && Debug->AutoRefresh->Checked)
    {
        Debug->NMIRetAddr = getbyte(z80.sp.w) + 256*getbyte(z80.sp.w + 1);
        NMISaveSingleStep = coleco.singlestep;
        coleco.singlestep=0;
    } */
    if (z80.pc.w == debug->NMIRetAddr)
    {
        debug->NMIRetAddr=-1;
        coleco.singlestep=NMISaveSingleStep;
    }
    if (debug->NMIRetAddr!=-1) return;

    /*if (z80.pc.w==0x38 && Debug->SkipINTBtn->Checked
        && Debug->INTRetAddr==-1 && Debug->AutoRefresh->Checked)
    {
        Debug->INTRetAddr = getbyte(z80.sp.w) + 256*getbyte(z80.sp.w + 1);
        INTSaveSingleStep = coleco.singlestep;
        coleco.singlestep=0;
    } */
    if (z80.pc.w == debug->INTRetAddr)
    {
        debug->INTRetAddr=-1;
        coleco.singlestep=INTSaveSingleStep;
    }
    if (debug->INTRetAddr!=-1) return;

    displayedTStatesCount = tStatesCount;

    if (debug->BreakPointHit() || (RetExecuted && StepOutRequested && (StackChange < 0)))
    {
        StepOutRequested = 0;
        debug->DoNext=false;
        debug->UpdateVals();
        debug->RunStop1Click(NULL);
    }

    if (debug->DoNext)
    {
        coleco.stop=1;
        debug->DoNext=false;
        debug->UpdateVals();
        coleco.singlestep = debug->AutoRefresh1->Checked ? 1 : 0;
    }
#if 0
    Profiler->DebugTick(&z80);
#endif
    if (debug->AutoRefresh1->Checked==true && debug->Visible==true)
        debug->UpdateVals();
}

//---------------------------------------------------------------------------
int get_memoryvalue(int adrmemory)
{
    int by=0;
    switch (debug->cboMemory->ItemIndex) {
        case 0 : by=getbyte(adrmemory); break; // ROM
        case 1 : by=coleco_gettmsval(VRAM,adrmemory,0,0); break; // VRAM
        case 2 : by=coleco_gettmsval(RAM,adrmemory,0,0); break; // RAM
        case 3 : by=coleco_gettmsval(SGMRAM,adrmemory,0,0); break; // SGM RAM
        case 4 : by=coleco_gettmsval(EEPROM,adrmemory,0,0); break; // EEPROM
        case 5 : by=coleco_gettmsval(SRAM,adrmemory,0,0); break; // SRAM
    }

    return by;
}

void set_memoryvalue(int adr, int val)
{
    switch (debug->cboMemory->ItemIndex) {
        case 0 : coleco_setval(ROM, adr,val); break; // ROM
        case 1 : coleco_setval(VRAM, adr,val); break; // VRAM
        case 2 : coleco_setval(RAM, adr,val); break; // RAM
        case 3 : coleco_setval(SGMRAM, adr,val); break; // SGM RAM
        case 4 : coleco_setval(EEPROM, adr,val); break; // EEPROM
        case 5 : coleco_setval(SRAM, adr,val); break; // SRAM
    }
}

//---------------------------------------------------------------------------
void Tdebug::EnableValues(bool enable)
{
    HL->Enabled = enable;
    BC->Enabled = enable;
    DE->Enabled = enable;
    HL_->Enabled = enable;
    BC_->Enabled = enable;
    DE_->Enabled = enable;
    IX->Enabled = enable;
    IY->Enabled = enable;
    PC->Enabled = enable;
    SP->Enabled = enable;
    IR->Enabled = enable;
    A->Enabled = enable;
    A_->Enabled = enable;
    F->Enabled = enable;
    F_->Enabled = enable;

    Stack00->Enabled = enable;
    Stack01->Enabled = enable;
    Stack02->Enabled = enable;
    Stack03->Enabled = enable;
    Stack04->Enabled = enable;
    Stack05->Enabled = enable;
    Stack06->Enabled = enable;
    Stack07->Enabled = enable;
    Stack08->Enabled = enable;
    Stack09->Enabled = enable;
    Stack10->Enabled = enable;
    Stack11->Enabled = enable;
    Stack12->Enabled = enable;
    Stack13->Enabled = enable;

    Disass0->Enabled = enable;
    Disass1->Enabled = enable;
    Disass2->Enabled = enable;
    Disass3->Enabled = enable;
    Disass4->Enabled = enable;
    Disass5->Enabled = enable;
    Disass6->Enabled = enable;
    Disass7->Enabled = enable;
    Disass8->Enabled = enable;
    Disass9->Enabled = enable;
    Disass10->Enabled = enable;
    Disass11->Enabled = enable;

    Halt->Enabled = enable;
    Interrupts->Enabled = enable;
    IM->Enabled = enable;

/*
    cboMemory->Enabled = enable;
    udMemory->Enabled = enable;
    ButtonFirstChange->Enabled = enable;
    ButtonPrevChange->Enabled = enable;
    ButtonNextChange->Enabled = enable;
    ButtonLastChange->Enabled = enable;
*/

    TStatesCount->Enabled = enable;
}

void Tdebug::DisableVals(void)
{
    EnableValues(false);
}

void Tdebug::EnableVals(void)
{
    EnableValues(true);
}

//---------------------------------------------------------------------------
bool Tdebug::AddBreakPoint(struct breakpoint& bp)
{
    const int maxBreakpoints = 99;

    if (Breakpoints >= maxBreakpoints)
        return false;

    int i;
    for(i=0; i<Breakpoints; i++)
    {
        bool existingBreakpoint;

        switch (bp.Type)
        {
            case BP_TSTATES:
                existingBreakpoint = (Breakpoint[i].Type == bp.Type);
                break;

            case BP_REGISTER:
                existingBreakpoint = (Breakpoint[i].Type == bp.Type) &&
                    (Breakpoint[i].RegisterId == bp.RegisterId) &&
                    (Breakpoint[i].ConditionValue == bp.ConditionValue) &&
                    (Breakpoint[i].Value == bp.Value);
                break;

            case BP_FLAG:
                existingBreakpoint = (Breakpoint[i].Type == bp.Type) &&
                    (Breakpoint[i].FlagId == bp.FlagId);
                break;

            case BP_MEMORY:
                existingBreakpoint = (Breakpoint[i].Type == bp.Type) &&
                    (Breakpoint[i].Addr == bp.Addr) &&
                    (Breakpoint[i].ConditionValue == bp.ConditionValue) &&
                    (Breakpoint[i].Value == bp.Value);
                break;

            case BP_RD:
            case BP_WR:
            case BP_EXE:
            case BP_OUTL:
            case BP_OUTH:
            case BP_INL:
            case BP_INH:
                existingBreakpoint = (Breakpoint[i].Type == bp.Type) &&
                    (Breakpoint[i].ConditionAddr == bp.ConditionAddr) &&
                    (Breakpoint[i].Addr == bp.Addr) &&
                    (Breakpoint[i].ConditionValue == bp.ConditionValue) &&
                    (Breakpoint[i].Value == bp.Value);
                break;
        }

        if (existingBreakpoint)
        {
            DelBreakPoint(i);
            break;
        }
    }

    Breakpoint[Breakpoints] = bp;

    AnsiString str = GetBreakpointText(&bp);

    BPList->Cells[0][Breakpoints] = str;

    Breakpoints++;
    BPList->RowCount++;
    if (BPList->RowCount > 1)
        BPList->Row = BPList->RowCount - 2;

    AddBrkBtn->Enabled = (Breakpoints < maxBreakpoints);
    DelBrkBtn->Enabled = true;
    EditBrkBtn->Enabled = true;

    return true;
}

AnsiString Tdebug::GetBreakpointText(breakpoint* const bp)
{
    AnsiString str;

    switch (bp->Type)
    {
        case BP_EXE:
            str = ConstructExeBreakpointText(bp);
            break;

        case BP_RD:
            str = ConstructRdWrInOutMemBreakpointText("RD ", bp);
            break;

        case BP_WR:
            str = ConstructRdWrInOutMemBreakpointText("WR ", bp);
            break;

        case BP_IN:
            str = ConstructRdWrInOutMemBreakpointText("IN ", bp);
            break;

        case BP_OUT:
            str = ConstructRdWrInOutMemBreakpointText("OUT", bp);
            break;

        case BP_MEMORY:
            str = ConstructRdWrInOutMemBreakpointText("MEM", bp);
            break;

        case BP_INL:
            str = ConstructLowIOBreakpointText("IN ", bp);
            break;

        case BP_INH:
            str = ConstructHighIOBreakpointText("IN ", bp);
            break;

        case BP_OUTL:
            str = ConstructLowIOBreakpointText("OUT", bp);
            break;

        case BP_OUTH:
            str = ConstructHighIOBreakpointText("OUT", bp);
            break;

        case BP_TSTATES:
            str = ConstructTStatesBreakpointText(bp);
            break;

        case BP_REGISTER:
            str = ConstructRegisterBreakpointText(bp);
            break;

        case BP_FLAG:
            str = ConstructFlagBreakpointText(bp);
            break;
    }

    if (!bp->Permanent)
    {
        str = "{" + str + "}";
    }

    return str;
}

static AnsiString GetConditionAddr(BreakpointCondition condition)
{
    const AnsiString conditions[] = { " = ", " <>", " <=", " >=", " ->" };

    return conditions[condition];
}

static AnsiString GetConditionValue(BreakpointCondition condition)
{
    const AnsiString conditions[] = { " = ", " <>", " <=", " >=", "..." };

    return conditions[condition];
}

AnsiString Tdebug::ConstructTStatesBreakpointText(breakpoint* const bp)
{
    AnsiString ca = GetConditionAddr(bp->ConditionAddr);
    AnsiString str = "CLK" + ca + Hex16(bp->Addr) + " <>" + bp->TStates;
    return str;
}

static AnsiString Pad(AnsiString string)
{
    AnsiString paddedString = string + "    ";
    return paddedString.SubString(1, 5);
}

AnsiString Tdebug::ConstructFlagBreakpointText(breakpoint* const bp)
{
    AnsiString FlagNames[] = {"C", "N", "P/V", "Bit3", "H", "Bit5", "Z", "S"};

    AnsiString ca = GetConditionAddr(bp->ConditionAddr);
    AnsiString cv = GetConditionValue(bp->ConditionValue);
    AnsiString str = "FLG" + ca + Pad(FlagNames[bp->FlagId]) + cv + bp->Value;

    return str;
}

AnsiString Tdebug::ConstructRegisterBreakpointText(breakpoint* const bp)
{
    AnsiString RegisterNames[] = {"BC", "DE", "HL", "IX", "IY", "PC", "SP", "BC'", "DE'", "HL'",
                                      "A", "B", "C", "D", "E", "H", "L", "I", "R", "IXh", "IXl", "IYh", "IYl",
                                      "A'", "B'", "C'", "D'", "E'", "H'", "L'"};

    AnsiString ca = GetConditionAddr(bp->ConditionAddr);
    AnsiString cv = GetConditionValue(bp->ConditionValue);
    AnsiString str = "REG" + ca + Pad(RegisterNames[bp->RegisterId]) + cv;
    if (bp->RegisterId >= RegA)
    {
        str += Hex8(bp->Value);
    }
    else
    {
        str += Hex16(bp->Value);
    }

    return str;
}

AnsiString Tdebug::ConstructLowIOBreakpointText(AnsiString type, breakpoint* const bp)
{
    AnsiString ca = GetConditionAddr(bp->ConditionAddr);
    AnsiString cv = GetConditionValue(bp->ConditionValue);
    AnsiString str = type + ca + "xx" + Hex8(bp->Addr) + cv + Hex8(bp->Value);
    return str;
}

AnsiString Tdebug::ConstructHighIOBreakpointText(AnsiString type, breakpoint* const bp)
{
    AnsiString ca = GetConditionAddr(bp->ConditionAddr);
    AnsiString cv = GetConditionValue(bp->ConditionValue);
    AnsiString str = type + ca + Hex8(bp->Addr >> 8) + "xx" + cv + Hex8(bp->Value);
    return str;
}

AnsiString Tdebug::ConstructRdWrInOutMemBreakpointText(AnsiString type, breakpoint* const bp)
{
    AnsiString ca = GetConditionAddr(bp->ConditionAddr);
    AnsiString str = type + ca + Hex16(bp->Addr);

    if (bp->ConditionAddr == Range)
    {
        str += "..." + Hex16(bp->EndAddr);
    }
    else
    {
        AnsiString cv = GetConditionValue(bp->ConditionValue);
        str += cv + Hex8(bp->Value);
    }

    return str;
}

AnsiString Tdebug::ConstructExeBreakpointText(breakpoint* const bp)
{
    AnsiString ca = GetConditionAddr(bp->ConditionAddr);
    AnsiString str = "EXE" + ca + Hex16(bp->Addr);

    if (bp->ConditionAddr == Range)
    {
        str += "..." + Hex16(bp->EndAddr);
    }
    else
    {
        AnsiString symbol;
        if (symbolstore::addressToSymbol(bp->Addr, symbol))
        {
            str += " " + symbol;
        }
    }

    return str;
}

void Tdebug::DelBreakPoint(int index)
{
    int j;
    for(j=index; j<Breakpoints;j++)
    {
        Breakpoint[j] = Breakpoint[j+1];
        BPList->Cells[0][j] = BPList->Cells[0][j+1];
    }
    Breakpoints--;
    BPList->RowCount--;

    if ((BPList->Row > 0) && ((BPList->Row + 1) >= BPList->RowCount))
    {
        BPList->Row--;
    }
}

void Tdebug::DelTempBreakPoints(void)
{
    int i;
    for(i=0; i<Breakpoints; i++)
        if (!Breakpoint[i].Permanent)
            DelBreakPoint(i);
}

bool Tdebug::BreakPointHit()
{
    int lmrl = lastMemoryReadAddrLo;
    int lmrlv = lastMemoryReadValueLo;
    lastMemoryReadAddrLo = -1;

    int lmrh = lastMemoryReadAddrHi;
    int lmrhv = lastMemoryReadValueHi;
    lastMemoryReadAddrHi = -1;

    int lmwl = lastMemoryWriteAddrLo;
    int lmwlv = lastMemoryWriteValueLo;
    lastMemoryWriteAddrLo = -1;

    int lmwh = lastMemoryWriteAddrHi;
    int lmwhv = lastMemoryWriteValueHi;
    lastMemoryWriteAddrHi = -1;

    int lpi = debug->lastPortInAddr;
    int lpiv = debug->lastPortInValue;
    debug->lastPortInAddr = -1;

    int lpo = debug->lastPortOutAddr;
    int lpov = debug->lastPortOutValue;
    debug->lastPortOutAddr = -1;

    for (int idx = 0; idx < Breakpoints; ++idx)
    {
        breakpoint* bp = &Breakpoint[idx];

	if (debug->BPExeHit(z80.pc.w, bp, idx) ||
            debug->BPInOutHit(BP_IN, lpi, lpiv, bp) ||
            debug->BPInOutHit(BP_INL, lpi, lpiv, bp) ||
            debug->BPInOutHit(BP_INH, lpi, lpiv, bp) ||
            debug->BPInOutHit(BP_OUT, lpo, lpov, bp) ||
            debug->BPInOutHit(BP_OUTL, lpo, lpov, bp) ||
            debug->BPInOutHit(BP_OUTH, lpo, lpov, bp) ||
            debug->BPReadWriteHit(BP_WR, lmwh, lmwhv, bp) ||
            debug->BPReadWriteHit(BP_WR, lmwl, lmwlv, bp) ||
            debug->BPReadWriteHit(BP_RD, lmrh, lmrhv, bp) ||
            debug->BPReadWriteHit(BP_RD, lmrl, lmrlv, bp) ||
            debug->BPRegisterValueHit(bp) ||
            debug->BPFlagValueHit(bp) ||
            debug->BPMemoryValueHit(bp) ||
            debug->BPClockHit(z80.pc.w, bp))
        {
            if (bp->Permanent)
            {
                BPList->Row = idx;
            }
            return true;
        }
    }

    return false;
}

bool Tdebug::BPExeHit(int addr, breakpoint* const bp, int idx)
{
    if (bp->HitExe(BP_EXE, addr))
	{
        breakpoint* lastBP = &Breakpoint[Breakpoints - 1];

        if (bp->Permanent)
        {
            if (lastBP->Permanent ||
                (!lastBP->Permanent && (((lastBP->Addr - bp->Addr) != StepOverInstructionSize) || (bp->ConditionAddr != Equal))))
            {
                return true;
            }
        }
        else
        {
            if (!bp->Permanent && (StepOverStack == z80.sp.w))
            {
                DelBreakPoint(idx);
                return true;
            }
        }
    }

    return false;
}

bool Tdebug::BPInOutHit(BreakpointType type, int addr, int value, breakpoint* const bp)
{
    if (addr == -1)
    {
        return false;
    }

    switch (type)
    {
        case BP_INL:
        case BP_OUTL:
            addr = addr & 0x00FF;
            break;

        case BP_INH:
        case BP_OUTH:
            addr = addr & 0xFF00;
            break;
    }

    if (bp->HitRdWrInOut(type, addr, value))
	{
        return true;
    }

	return false;
}

bool Tdebug::BPReadWriteHit(BreakpointType type, int addr, int value, breakpoint* const bp)
{
	if ((addr != -1) && bp->HitRdWrInOut(type, addr, value))
	{
		return true;
	}

    return false;
}

bool Tdebug::BPClockHit(int addr, breakpoint* const bp)
{
    if (bp->Type != BP_TSTATES)
    {
        return false;
    }

    if (addr == bp->Addr)
	{
        tStatesCount = 0;

        if (displayedTStatesCount != bp->TStates)
		{
            return true;
        }
	}

    return false;
}


bool Tdebug::BPFlagValueHit(breakpoint* const bp)
{
    if (bp->Type != BP_FLAG)
    {
        return false;
    }
    BYTE regF = z80.af.b.l;
    int mask = (1 << bp->FlagId);

    switch (bp->ConditionValue)
    {
        case Equal:
            return ((regF & mask) == bp->Value);

        case NotEqual:
            return ((regF & mask) != bp->Value);
    }

    return false;
}

bool Tdebug::BPMemoryValueHit(breakpoint* const bp)
{
    if (bp->Type != BP_MEMORY)
    {
        return false;
    }

    BYTE value = getbyte(bp->Addr);

    switch (bp->ConditionValue)
    {
        case Equal:
            return (value == bp->Value);

        case LessThanEquals:
            return (value <= bp->Value);

        case GreaterThanEquals:
            return (value >= bp->Value);

        case NotEqual:
            return (value != bp->Value);
    }
    return false;
}

bool Tdebug::BPRegisterValueHit(breakpoint* const bp)
{
    if (bp->Type != BP_REGISTER)
    {
        return false;
    }

    int value = getRegisterValue(bp->RegisterId);

    switch (bp->ConditionValue)
    {
        case Equal:
            return (value == bp->Value);

        case LessThanEquals:
            return (value <= bp->Value);

        case GreaterThanEquals:
            return (value >= bp->Value);

        case NotEqual:
            return (value != bp->Value);
    }

    return false;
}

int Tdebug::getRegisterValue(int registerIndex)
{
    int value;
#if 0
    switch (registerIndex)
    {
	    case RegA:
            value = z80.af.b.h;
			break;

		case RegB:
            value = z80.bc.b.h;
			break;

		case RegC:
            value = z80.bc.b.l;
			break;

		case RegD:
            value = z80.de.b.h;
			break;

		case RegE:
            value = z80.de.b.l;
			break;

		case RegH:
            value = z80.hl.b.h;
			break;

		case RegL:
            value = z80.hl.b.l;
			break;

		case RegI:
            value = z80.i;
			break;

		case RegR:
            value = z80.r;
			break;

		case RegIXh:
            value = z80.ix.b.h;
			break;

		case RegIXl:
            value = z80.ix.b.l;
			break;

		case RegIYh:
            value = z80.iy.b.h;
			break;

		case RegIYl:
            value = z80.iy.b.l;
			break;

		case RegAltA:
            value = z80.af_.b.h;
			break;

		case RegAltB:
            value = z80.bc_.b.h;
			break;

		case RegAltC:
            value = z80.bc_.b.l;
			break;

		case RegAltD:
            value = z80.de_.b.h;
			break;

		case RegAltE:
            value = z80.de_.b.l;
			break;

		case RegAltH:
            value = z80.hl_.b.h;
			break;

		case RegAltL:
            value = z80.hl_.b.l;
			break;

		case RegBC:
            value = z80.bc.w.l;
			break;

		case RegDE:
            value = z80.de.w.l;
			break;

		case RegHL:
            value = z80.hl.w.l;
			break;

		case RegIX:
            value = z80.ix.w.l;
			break;

		case RegIY:
            value = z80.iy.w.l;
			break;

		case RegPC:
            value = z80.pc.w.l;
			break;

		case RegSP:
            value = z80.sp.w.l;
			break;

		case RegAltBC:
            value = z80.bc_.w;
			break;

		case RegAltDE:
            value = z80.de_.w;
			break;

		case RegAltHL:
            value = z80.hl_.w;
			break;
    }
#endif
    return value;
}

//---------------------------------------------------------------------------
int Tdebug::Hex2Dec(AnsiString num)
{
    int val=0;
    int i;

    for(i=1; i<=num.Length(); i++)
    {
        if (num[i]>='0' && num[i]<='9') val = val*16 + num[i]-'0';
        else if (num[i]>='a' && num[i]<='f') val = val*16 + 10 + num[i]-'a';
        else if (num[i]>='A' && num[i]<='F') val = val*16 + 10 + num[i]-'A';
        else return(val);
    }
    return(val);
}
//---------------------------------------------------------------------------
AnsiString Tdebug::Hex16(int Value)
{
    return AnsiString::IntToHex(Value,4);
}
//---------------------------------------------------------------------------
AnsiString Tdebug::Hex8(int Value)
{
    return AnsiString::IntToHex(Value,2);
}
//---------------------------------------------------------------------------
AnsiString Tdebug::Bin8(int Value)
{
    char arry[9] = {0};
    for (int i = 7; i >= 0; --i)
    {
        arry[i] = (Value & 1) + '0';
        Value >>= 1;
    }

    return AnsiString(arry);
}

//---------------------------------------------------------------------------
// set the caption and other good stuff for a register label
void Tdebug::SetLabelInfo(TLabel* label, int value, int valueWidth)
{
    // the tag is used for the editvalue function
    label->Tag = value;

    // the caption is fairly standard
    label->Caption = AnsiString::IntToHex(label->Tag, valueWidth);
    if (valueWidth == 4)
    {
        // ahaa!
        AnsiString hint("");

        // enable showHint if there is a symbol for the given 16bit value
        label->ShowHint = symbolstore::addressToSymbol(label->Tag, hint);
        label->Hint = hint;

        // lowlight the label if there's a hint to show
        label->Color = label->ShowHint ? clInfoBk : clBtnFace;
    }
}

//---------------------------------------------------------------------------
void Tdebug::UpdateVals(void)
{
    int i, Stack;

/*
    RowCount->Caption = rowcounter;
    Scanline->Caption = RasterY;
    TStates->Caption = frametstates;
    NMIGen->Caption = NMI_generator ? "On":"Off";
    HSYNCGen->Caption = HSYNC_generator ? "On":"Off";
*/

    SetLabelInfo(HL, z80.hl.w);
    SetLabelInfo(BC, z80.bc.w);
    SetLabelInfo(DE, z80.de.w);
    SetLabelInfo(HL_, z80.hl_.w);
    SetLabelInfo(BC_, z80.bc_.w);
    SetLabelInfo(DE_, z80.de_.w);
    SetLabelInfo(IX, z80.ix.w);
    SetLabelInfo(IY, z80.iy.w);
    SetLabelInfo(PC, z80.pc.w);
    SetLabelInfo(SP, z80.sp.w);
    //TODO SetLabelInfo(IR, (z80.i<<8) | (z80.r7 & 128) | ((z80.r) & 127));
    SetLabelInfo(A, z80.af.b.h, 2);
    SetLabelInfo(A_, z80.af_.b.h, 2);
    F->Caption = Bin8(z80.af.b.l);
    F_->Caption = Bin8(z80.af_.b.l);

    i=z80.sp.w;
    Stack00->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack01->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack02->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack03->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack04->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack05->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack06->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack07->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack08->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack09->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack10->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack11->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack12->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;
    Stack13->Caption = Hex16(i)+" " +Hex16( getbyte(i)+256*getbyte(i+1) ); i+=2;

    i = recentHistory[(recentHistoryPos+0)&3];
    Disass0->Caption = Disassemble(&i);
    i = recentHistory[(recentHistoryPos+1)&3];
    Disass1->Caption = Disassemble(&i);
    i = recentHistory[(recentHistoryPos+2)&3];
    Disass2->Caption = Disassemble(&i);
    i=z80.pc.w;
    int stepOverStartAddr=i;
    StepOverInstruction = IsStepOverInstruction(i);
    Disass3->Caption = (Disassemble(&i) + "                              ").SetLength(63);
    StepOverAddr = i;
    StepOverStack=z80.sp.w;
    StepOverInstructionSize = StepOverAddr - stepOverStartAddr;
    StepOutRequested = 0;
    Disass4->Caption = Disassemble(&i);
    Disass5->Caption = Disassemble(&i);
    Disass6->Caption = Disassemble(&i);
    Disass7->Caption = Disassemble(&i);
    Disass8->Caption = Disassemble(&i);
    Disass9->Caption = Disassemble(&i);
    Disass10->Caption = Disassemble(&i);
    Disass11->Caption = Disassemble(&i);

    //TODO Halt->Caption = z80.halted ? "Yes":"No" ;
    Interrupts->Caption = z80.iff1 ? "Enabled":"Disabled" ;
    IM->Caption = z80.im;

    if ((displayedTStatesCount >= 0) && (displayedTStatesCount < 1000000))
    {
        TStatesCount->Caption = displayedTStatesCount;
    }
    else
    {
        TStatesCount->Caption = "999999";
    }

    UpdateChanges();

    if (coleco.stop)
    {
        RunStop1->Caption = "Run";
        Step1->Enabled = true;
        StepOver1->Enabled = true;
        StepOut1->Enabled = true;
        EnableVals();
        DelTempBreakPoints();

        //Profiler->Refresh();
    }
    else
    {
        RunStop1->Caption = "Stop";
        Step1->Enabled = false;
        StepOver1->Enabled = false;
        StepOut1->Enabled = false;
        if (!AutoRefresh1->Checked) DisableVals();
    }
}
//---------------------------------------------------------------------------

bool Tdebug::IsStepOverInstruction(int Addr)
{
	bool stepOverInstruction = false;

    int Opcode;

    Opcode = GetMem(Addr);
    Addr++;

    switch(Opcode)
    {
        case 0x10: // DJNZ fz
        case 0x76: // HALT
        case 0xc4: // CALL NZ,nnnn
        case 0xc7: // RST 00
        case 0xcc: // CALL Z,nnnn
        case 0xcd: // CALL nnnn
        case 0xcf: // RST 08
        case 0xd4: // CALL NC,nnnn
        case 0xd7: // RST 10
        case 0xdc: // CALL C,nnnn
        case 0xdf: // RST 18
        case 0xe4: // CALL PO,nnnn
        case 0xe7: // RST 20
        case 0xec: // CALL PE,nnnn
        case 0xef: // RST 28
        case 0xf4: // CALL P,nnnn
        case 0xf7: // RST 30
        case 0xfc: // CALL M,nnnn
        case 0xff: // RST 38
    		stepOverInstruction = true;
		    break;
        case 0xed:
            Opcode = GetMem(Addr);
            switch(Opcode)
            {
                case 0xb0:	// LDIR
                case 0xb1:	// CPIR
                case 0xb2:	// INIR
                case 0xb3:	// OTIR
                case 0xb8:	// LDDR
                case 0xb9:	// CPDR
                case 0xba:	// INDR
                case 0xbb:	// OTDR
			        stepOverInstruction = true;
                    break;
            }
            break;
    }
	return stepOverInstruction;
}

//---------------------------------------------------------------------------
__fastcall Tdebug::Tdebug(TComponent* Owner)
    : TForm(Owner)
{
    TIniFile *ini;

    DoNext=false;
    Breakpoints=0;
    NMIRetAddr=INTRetAddr=-1;

    ini = new TIniFile(coleco.inipath);
    LoadSettings(ini);
    delete ini;

    maxMemory = 0xFFFF+1;
    cboMemory->ItemIndex=0;
    BaseAddress = z80.pc.w;

    BPList->DefaultColWidth = BPList->Width;

    // Memory part
    mHWND = this->panMem->Handle;
    mOffscreenBitmap = NULL;
    mRowRenderer = NULL;
    mSelectedAddress = -1;
    ignoreScrollChange = false;
    mRowRenderer = new TraditionalRowRenderer; // SetViewMode(MWVM_TRADITIONAL);
    mCharSize = Canvas->TextExtent(AnsiString("0"));
    mHeadingHeight = mCharSize.cy + (mCharSize.cy / 2);

    //    ResetLastIOAccesses();
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::FormClose(TObject *Sender, TCloseAction &Action)
{
    coleco.singlestep=0;
    Form1->Debugger1->Checked=false;
    coleco.stop=0;
    StepOutRequested = 0;
}

//---------------------------------------------------------------------------
void __fastcall Tdebug::FormShow(TObject *Sender)
{
    AutoRefresh1->Enabled = true;

    if (AutoRefresh1->Checked==true) coleco.singlestep=1;
    UpdateVals();
}
//---------------------------------------------------------------------------

void Tdebug::LoadSettings(TIniFile *ini)
{
    Top = ini->ReadInteger("DEBUG","Top",Top);
    Left = ini->ReadInteger("DEBUG","Left",Left);

    if (Form1->Debugger1->Checked)
    {
        Form1->Debugger1->Checked=false;
    }

    AutoRefresh1->Checked = ini->ReadBool("DEBUG", "AutoRefresh", true);
}

void Tdebug::SaveSettings(TIniFile *ini)
{
    ini->WriteInteger("DEBUG","Top",Top);
    ini->WriteInteger("DEBUG","Left",Left);
    ini->WriteBool("DEBUG", "AutoRefresh", AutoRefresh1->Checked);
}
//---------------------------------------------------------------------------
void __fastcall Tdebug::RunStop1Click(TObject *Sender)
{
    coleco.stop = 1-coleco.stop;
    if(!coleco.stop)
    {
        ClearChanges();
        SoundResume();
    }
    else {
        SoundSuspend();
    }
    UpdateVals();
    StepOutRequested = 0;
    StackChange = 0;
}
//---------------------------------------------------------------------------
void __fastcall Tdebug::Step1Click(TObject *Sender)
{
    ClearChanges();
    coleco.stop=0;
    coleco.singlestep=1;
    StackChange = 0;
    DoNext=true;
}
//---------------------------------------------------------------------------
void __fastcall Tdebug::StepOver1Click(TObject *Sender)
{
    if (!StepOverInstruction)
    {
        Step1Click(Sender);
        return;
    }

    ClearChanges();
    breakpoint bp(StepOverAddr, BP_EXE);
    bp.Permanent = false;
    AddBreakPoint(bp);
    RunStop1Click(NULL);
}
//---------------------------------------------------------------------------
void __fastcall Tdebug::StepOut1Click(TObject *Sender)
{
    RunStop1Click(NULL);

    StepOutRequested = 1;
}
//---------------------------------------------------------------------------
void __fastcall Tdebug::Exit1Click(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall Tdebug::ShowSymbols1Click(TObject *Sender)
{
    if (SymbBrows->Visible)
    {
        ShowSymbols1->Caption="Show Symbols";
        SymbBrows->Close();
        return;
    }
    SymbBrows->Show();
    ShowSymbols1->Caption="Hide Symbols";
}
//---------------------------------------------------------------------------
void __fastcall Tdebug::LoadSymbolFile1Click(TObject *Sender)
{
    AnsiString Filename;

    try
    {
        if (!OpenDialog1->Execute())
        {
            return;
        }

        Filename=OpenDialog1->FileName;

        symbolstore::loadFileSymbols(Filename.c_str());
        // hmm there really should be an event here.
        SymbBrows->RefreshContent();
    }
    catch (Exception &exception)
    {
        // The default exception handler not only shows the exception that
        // occured, but also quits the message handler
        Application->ShowException(&exception);
    }
}
//---------------------------------------------------------------------------
void __fastcall Tdebug::DoEditStack(int offs)
{
    EditValue->CentreOn(this);

    int i = 2 * offs + z80.sp.w;
    int v = getbyte(i)+256*getbyte(i+1);
    if (EditValue->Edit2(v, 2))
    {
        setbyte(i, v&255);
        setbyte(i+1, v>>8);
        UpdateVals();
    }
}

void __fastcall Tdebug::Stack00Click(TObject *Sender)
{
    TLabel* label = (TLabel*)Sender;
    int len = label->Name.Length();
    int idx = label->Name.SubString(len-1, 2).ToInt();
    DoEditStack(idx);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::Stack00MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TLabel* label = (TLabel*)Sender;
    int len = label->Name.Length();
    int idx = label->Name.SubString(len-1, 2).ToInt();

    int i = z80.sp.w;
    i += 2 * idx;
    SetMenuContent(getbyte(i)+256*getbyte(i+1));
}
//---------------------------------------------------------------------------

void Tdebug::SetMenuContent(int memloc)
{
     MemDumpPopup->Tag = memloc;
}

//---------------------------------------------------------------------------

void __fastcall Tdebug::cboMemoryChange(TObject *Sender)
{
    // change max for memory
    switch (cboMemory->ItemIndex) {
        case 0 : // ROM
            maxMemory = 0xFFFF+1;
            break;
        case 1 : // VRAM
            maxMemory = 0x3FFF+1;
            break;
        case 2 : // RAM
            maxMemory = 0x3FF+1;
            break;
        case 3 : // SGM RAM
            maxMemory = 0x7FFF+1;
            break;
        case 4 : // EEPROM
            maxMemory = 0x7FFF+1;
            break;
        case 5 : // SRAM
            maxMemory = 0x7FF+1;
            break;
    }
    UpdateChanges();
}
//---------------------------------------------------------------------------


void __fastcall Tdebug::FClick(TObject *Sender)
{
    // this relies on the textbox that displays the flag bits
    // being exactly 8 char_width pixels wide.
    //
/*
    TPoint cp = ScreenToClient(Mouse->CursorPos);
    cp.x -= F->Left + 2;
    cp.y -= F->Top;
    int f = cp.x / (F->Width / 8);

    int bit = 0x80 >> f;
    z80.af.b.l ^= bit;
    UpdateVals()
*/
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::F_Click(TObject *Sender)
{
/*
    // see above.
    //
    TPoint cp = ScreenToClient(Mouse->CursorPos);
    cp.x -= F_->Left + 2;
    cp.y -= F_->Top;
    int f = (cp.x) / (F_->Width / 8);

    int bit = 0x80 >> f;
    z80.af_.b.l ^= bit;
    UpdateVals();
*/
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::BCMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   SetMenuContent(z80.bc.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::DEMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   SetMenuContent(z80.de.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::HLMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   SetMenuContent(z80.hl.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::IXMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   SetMenuContent(z80.ix.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::IYMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   SetMenuContent(z80.iy.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::IRMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
#if 0
        int ir = (z80.i << 8) | (z80.r7 & 128) | (z80.r & 127);
        SetMenuContent(ir);
#endif
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::SPMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   SetMenuContent(z80.sp.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::PCMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
   SetMenuContent(z80.pc.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::DoEditReg(WORD& value)
{
    EditValue->CentreOn(this);

    int n = value;
    if (EditValue->Edit2(n, 2))
    {
        value = n;
        UpdateVals();
    }
}

void __fastcall Tdebug::DoEditReg(BYTE& value)
{
        EditValue->CentreOn(this);

    int n = value;
    if (EditValue->Edit2(n, 1))
    {
        value = n;
        UpdateVals();
    }
}

//---------------------------------------------------------------------------

void __fastcall Tdebug::AClick(TObject *Sender)
{
    DoEditReg(z80.af.b.h);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::A_Click(TObject *Sender)
{
//    DoEditReg(z80.af_.b.h);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::SPClick(TObject *Sender)
{
//        DoEditReg(z80.sp.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::PCClick(TObject *Sender)
{
//        DoEditReg(z80.pc.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::BCClick(TObject *Sender)
{
//    DoEditReg(z80.bc.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::DEClick(TObject *Sender)
{
//    DoEditReg(z80.de.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::HLClick(TObject *Sender)
{
//    DoEditReg(z80.hl.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::BC_Click(TObject *Sender)
{
//    DoEditReg(z80.bc_.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::DE_Click(TObject *Sender)
{
//    DoEditReg(z80.de_.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::HL_Click(TObject *Sender)
{
//    DoEditReg(z80.hl_.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::IXClick(TObject *Sender)
{
//    DoEditReg(z80.ix.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::IYClick(TObject *Sender)
{
//    DoEditReg(z80.iy.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::IMClick(TObject *Sender)
{
#if 0
    if (++z80.im == 3) z80.im=0;
    UpdateVals();
#endif
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::InterruptsClick(TObject *Sender)
{
#if 0
    z80.iff1=z80.iff2= !z80.iff1;
    UpdateVals();
#endif
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::IRClick(TObject *Sender)
{
    EditValue->CentreOn(this);

    int v;
#if 0
    v = (z80.i << 8) | (z80.r7 & 128) | z80.r;
    if (EditValue->Edit2(v, 2))
    {
        z80.r = v&127;
        z80.r7 = v&128;
        z80.i = (v>>8)&255;
        UpdateVals();
    }
#endif
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::AddBrkBtnClick(TObject *Sender)
{
    SetBreakpoint->CentreOn(this);

    breakpoint bp(0, BP_EXE);
    if (SetBreakpoint->EditBreakpoint(bp))
    {
        AddBreakPoint(bp);
    }

    DelBrkBtn->Enabled = (BPList->RowCount > 1);
    EditBrkBtn->Enabled = (BPList->RowCount > 1);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::DelBrkBtnClick(TObject *Sender)
{
    if (BPList->Row < (BPList->RowCount-1))
        DelBreakPoint(BPList->Row);

    if ((BPList->RowCount > 1) && ((BPList->Row + 1) >= BPList->RowCount))
    {
        BPList->Row -= 1;
    }

    DelBrkBtn->Enabled = (BPList->RowCount > 1);
    EditBrkBtn->Enabled = (BPList->RowCount > 1);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::EditBrkBtnClick(TObject *Sender)
{
    SetBreakpoint->CentreOn(this);

    int idx = BPList->Row;
    breakpoint bp = Breakpoint[idx];

    if (SetBreakpoint->EditBreakpoint(bp))
    {
        Breakpoint[idx] = bp;

        AnsiString str = GetBreakpointText(&bp);
        BPList->Rows[idx]->Text = str;
    }
}
//---------------------------------------------------------------------------


void __fastcall Tdebug::BC_MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
//   SetMenuContent(z80.bc_.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::DE_MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
//   SetMenuContent(z80.de_.w);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::HL_MouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
//   SetMenuContent(z80.hl_.w);
}

//---------------------------------------------------------------------------
void RowRenderer::ChooseTextColour()
{
    if (mDirty != mLast && mAddress == *mDirty)
    {
        SetTextColor(mCHDC, RGB(200,0,0));
        ++mDirty;
    }
    else
    {
        SetTextColor(mCHDC, GetSysColor(COLOR_WINDOWTEXT));
    }
}

void RowRenderer::AddressOut(void)
{
    SetTextColor(mCHDC, GetSysColor(COLOR_WINDOWTEXT));
    if (mAddress >= 0)
    {
        TextOut(mCHDC, 0, mY, AnsiString::IntToHex(mAddress,4).c_str(), 4);
    }
    else
    {
        TextOut(mCHDC, 0, mY, AnsiString::IntToHex(65536 + mAddress,4).c_str(), 4);
    }
}

bool RowRenderer::ByteAtX(const int x, int& byte)
{
    if (x <= mLMargin || x >= mLMargin + (mDisplayCellsPerRow * mCellWidth))
    {
        return false;
    }

    byte = ((x - mLMargin) / mCellWidth) * mBytesPerCell;
    return true;
}

void RowRenderer::SetGeometry(int width, const TSize& charSize, int nCharsPerCell)
{
    mKern = charSize.cx / 2;
    mCellWidth = charSize.cx * (nCharsPerCell + 1);
    mLMargin = (charSize.cx * 6);

    int availWidth = width - mLMargin - SBARWIDTH;

    mDisplayCellsPerRow = availWidth / mCellWidth;

    if (mDisplayCellsPerRow * mCellWidth + (charSize.cx * nCharsPerCell) < availWidth)
    {
        ++mDisplayCellsPerRow;
    }
}

void RowRenderer::RenderColumnHeadings(const TSize& charSize)
{
    int columnInset = ((mCellWidth - charSize.cy) / 2) + mLMargin;

    for (int x = 0; x < mDisplayCellsPerRow; ++x)
    {
		AnsiString heading = AnsiString::IntToHex(x, 2);
        TextOut(mCHDC, (x * mCellWidth) + columnInset, 0, heading.c_str(), 2);
    }
}

bool TraditionalRowRenderer::ByteAtX(const int x, int& byte)
{
    int basex = mLMargin + (mDisplayCellsPerRow * mCellWidth) + mCellWidth;

    if (x >= basex && x < basex + (mDisplayCellsPerRow * 8))
    {
        byte = (x - basex) / 8;
    }
    else if (x <= mLMargin || x >= mLMargin + (mDisplayCellsPerRow * mCellWidth))
    {
        return false;
    }
    else
    {
        byte = (x - mLMargin) / mCellWidth;
    }

    return true;
}

void TraditionalRowRenderer::SetGeometry(int width, TSize& charSize)
{
    RowRenderer::SetGeometry(width, charSize, 2);

    double availableWidth = width - mLMargin - mCellWidth - SBARWIDTH;

    float cells = 8 + mCellWidth;
    int Rw = int(availableWidth * (8 / cells));
    int Lw = int(availableWidth) - Rw;

    mDisplayCellsPerRow = Lw / mCellWidth;
}

void TraditionalRowRenderer::RenderRow(void)
{
    HDC another = CreateCompatibleDC(mCHDC);
    HGDIOBJ oldBM = NULL;

    COLORREF prevBackgroundColour = GetBkColor(mCHDC);
    COLORREF paper = RGB(255,255,0);
    COLORREF ink = RGB(0,0,255);

    AddressOut();
    int basex = mLMargin + (mDisplayCellsPerRow * mCellWidth) + mCellWidth;
    for (int x = 0; x < mDisplayCellsPerRow; ++x)
    {
        ChooseTextColour();

        int by=get_memoryvalue(mAddress);
        if ((mAddress >= 0x0000) && (mAddress <= debug->maxMemory-1)) // 0xFFFF))
        {
            if (mSelectedAddress == mAddress)
            {
                SetBkColor(mCHDC, paper);
                SetTextColor(mCHDC, ink);
            }

            TextOut(mCHDC, x * mCellWidth + mLMargin + mKern, mY,
                AnsiString::IntToHex(by, 2).c_str(), 2);

            if ( (by>=32) && (by<=127) )
            {
                char W=static_cast<char>(by);
                TextOut(mCHDC, x * 8 + basex, mY, &W, 1);
            }
            else
                TextOut(mCHDC, x * 8 + basex, mY, ".", 1);

            if (mSelectedAddress == mAddress)
            {
                SetBkColor(mCHDC, prevBackgroundColour);
            }
        }
        ++mAddress;
    }

    SelectObject(another, oldBM);
    DeleteDC(another);

    SetBkColor(mCHDC, prevBackgroundColour);
}

//---------------------------------------------------------------------------
void Tdebug::CreateBitmap(void)
{
    if (mOffscreenBitmap)
    {
        ::DeleteObject(mOffscreenBitmap);
    }

    RECT rect;
    ::GetClientRect(mHWND, (LPRECT) &rect);

    mBMWidth = rect.right - rect.left;
    mBMHeight = rect.bottom - rect.top/* - StatusBar1->Height*/;
    if (mBMWidth < 1)
        mBMWidth  = 1;
    if (mBMHeight < 1)
        mBMHeight  = 1;

    HDC hdc = (HDC) Canvas->Handle;
    HDC chdc = CreateCompatibleDC(hdc);

    mOffscreenBitmap = ::CreateCompatibleBitmap(hdc, mBMWidth, mBMHeight);
    HGDIOBJ oldbm = SelectObject(chdc, mOffscreenBitmap);

    HGDIOBJ oldfont = SelectObject(chdc, ((TPanelCanvasHack*)panMem)->Canvas->Font->Handle);


    FillRect(chdc, &rect, (HBRUSH)(COLOR_BTNFACE+1));

    SetBkColor(chdc, GetSysColor(COLOR_BTNFACE));

    mRowRenderer->SetGeometry(mBMWidth, mCharSize);

    mRows = (mBMHeight - mHeadingHeight) / mCharSize.cy;

    int bytesPerRow = mRowRenderer->BytesPerCell() * mRowRenderer->mDisplayCellsPerRow;

    sbMemory->SmallChange = bytesPerRow;
    sbMemory->LargeChange = 8 * sbMemory->SmallChange;

    sbMemory->Min = 0;
    sbMemory->Max = maxMemory - (mRows * bytesPerRow) + bytesPerRow - 1;

    mRowRenderer->mAddress = mBaseAddress;
    mRowRenderer->mSelectedAddress = mSelectedAddress;
    mRowRenderer->mDirty = dirtyBird.lower_bound(mRowRenderer->mAddress);
    mRowRenderer->mLast = dirtyBird.end();
    mRowRenderer->mCHDC = chdc;
    mRowRenderer->mY = mHeadingHeight;

    mRowRenderer->RenderColumnHeadings(mCharSize);

    for (int i = 0; i < mRows; ++i)
    {
        mRowRenderer->RenderRow();
        mRowRenderer->mY += mCharSize.cy;
    }

    SelectObject(chdc, oldfont);
    SelectObject(chdc, oldbm);
    DeleteDC(chdc);
}

void __fastcall Tdebug::FormPaint(TObject *Sender)
{
    if (!mOffscreenBitmap)
    {
        CreateBitmap();
    }

    HDC hdc = (HDC)  ((TPanelCanvasHack*)panMem)->Canvas->Handle; //    HDC hdc = (HDC)  Canvas->Handle;
    HDC chdc = CreateCompatibleDC(hdc);
    HGDIOBJ oldbm = SelectObject(chdc, mOffscreenBitmap);
    ::BitBlt(hdc, 0, 0, mBMWidth, mBMHeight, chdc, 0, 0, SRCCOPY);
    SelectObject(chdc, oldbm);
    DeleteDC(chdc);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::SetBaseAddress(int value)
{
    mSelectedAddress = -1;
    ignoreScrollChange = true;
    sbMemory->Position = value;
    ignoreScrollChange = false;
}

//---------------------------------------------------------------------------

void __fastcall Tdebug::UpdateChanges()
{
    if (mOffscreenBitmap)
    {
        ::DeleteObject(mOffscreenBitmap);
        mOffscreenBitmap = NULL;
    }

    Invalidate();

    changeCursor = dirtyBird.lower_bound(mBaseAddress);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::ClearChanges()
{
    dirtyBird.clear();
    UpdateChanges();
}

//---------------------------------------------------------------------------

void __fastcall Tdebug::sbMemoryChange(TObject *Sender)
{
    if ((sbMemory->Position > 0) && !ignoreScrollChange)
    {
        int delta = sbMemory->Position - mBaseAddress;
        int bytesPerRow = mRowRenderer->BytesPerCell() * mRowRenderer->mDisplayCellsPerRow;
        int offset = delta % bytesPerRow;

        sbMemory->Position -= offset;
    }

    mBaseAddress = sbMemory->Position;

    UpdateChanges();
}

//---------------------------------------------------------------------------
bool  __fastcall Tdebug::xyToAddress(int xIn, int yIn, int& address)
{
    int x;
    int y = (yIn - mHeadingHeight) / mCharSize.cy;

    if (!mRowRenderer->ByteAtX(xIn, x) || yIn < mHeadingHeight || yIn >= mHeadingHeight + (mRows * mCharSize.cy))
    {
        return false;
    }

    address = mBaseAddress + x + (y * mRowRenderer->mDisplayCellsPerRow * mRowRenderer->BytesPerCell());

    return (address <= 0xFFFF);
}

//---------------------------------------------------------------------------

void __fastcall Tdebug::panMemMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
    int address;
    AnsiString t;
    mSelectedAddress = -1;

    if (xyToAddress(X,Y,address))
    {
        t = AnsiString::IntToHex(address,4);
        mSelectedAddress = address;
    }

    Panel4->Caption = t;

    UpdateChanges();
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::panMemClick(TObject *Sender)
{
    int address;

    // prise du point de référence de la souris
    TPoint cpm;
    GetCursorPos(&cpm);
    TPoint cp = ScreenToClient(cpm);

    // CR  this is nasty - but it's the quickest way to tell if the debugger
    // is running continuously
    if (coleco.singlestep!=1)  return;

    if (xyToAddress(cp.x-gBoxMemory->Left-panMem->Left, cp.y-gBoxMemory->Top-panMem->Top, address))
    {
        EditValue->Top = cpm.y;//Mouse->CursorPos.y;
        EditValue->Left = cpm.x;//Mouse->CursorPos.x;

        int n = get_memoryvalue(address);
        if (EditValue->Edit2(n, 1))
        {
            set_memoryvalue(address,n);
            UpdateChanges();  // CR  refresh after edit
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::AutoRefresh1Click(TObject *Sender)
{
    if (AutoRefresh1->Enabled)
    {
        AutoRefresh1->Checked=!AutoRefresh1->Checked;
        if (AutoRefresh1->Checked==true)
        {
            coleco.singlestep=1;
            EnableVals();
        }
        else
        {
            coleco.singlestep=0;
            DisableVals();
        }
        UpdateVals();
    }
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::Refresh1Click(TObject *Sender)
{
    UpdateVals();
}
//---------------------------------------------------------------------------



void __fastcall Tdebug::SetAddress1Click(TObject *Sender)
{
    EditValue->CentreOn(this);

    int v = SetAddress1->Tag;
    if (EditValue->Edit2(v, 2))
    {
        SetBaseAddress(v);
    }
}

//---------------------------------------------------------------------------

bool Tdebug::FindSequence(std::vector<int>& bytes, int& addr)
{
    bool found;

    do
    {
        found = FindMatch(addr, bytes);
    }
    while (!found && (addr < maxMemory));

    return found;
}

bool Tdebug::FindMatch(int& addr, std::vector<int>& bytes)
{
    int firstByte = bytes[0];

    while ((addr < maxMemory) && (getbyte(addr) != firstByte))
    {
        ++addr;
    }

    if (addr == maxMemory)
    {
        return false;
    }

    for (int b = 1; b < (int)bytes.size(); b++)
    {
        ++addr;
        int byte = getbyte(addr);

        if (byte != bytes[b])
        {
            return false;
        }
    }

    addr -= bytes.size() - 1;

    return true;
}

//---------------------------------------------------------------------------
void Tdebug::DoSearch()
{
    SearchSequence->CentreOn(this);

    std::vector<int> bytes;
    bool performSearch = SearchSequence->EditSequenceBytes(mSearchText, bytes);
    if (performSearch)
    {
        if (!PerformSearch(bytes))
        {
            AnsiString mErrortxt="Can't find '"+mSearchText+"'";
            Application->MessageBox(mErrortxt.c_str(),"Error", MB_OK | MB_ICONERROR);
        }
    }
    else
        Application->MessageBox("Invalid characters","Error", MB_OK | MB_ICONERROR);
}

bool Tdebug::PerformSearch(std::vector<int>& bytes)
{
    int address = mBaseAddress + 1;

    if ((address < maxMemory) && FindSequence(bytes, address))
    {
        SetBaseAddress(address);
        return true;
    }
    return false;
}

void __fastcall Tdebug::Search1Click(TObject *Sender)
{
    DoSearch();
}
//---------------------------------------------------------------------------


void __fastcall Tdebug::AddBreak1Click(TObject *Sender)
{
    TMenuItem* mi = (TMenuItem*)Sender;
    breakpoint bp(MemDumpPopup->Tag, (BreakpointType)mi->Tag);
    AddBreakPoint(bp);
    DelBrkBtn->Enabled = (BPList->RowCount > 1);
    EditBrkBtn->Enabled = (BPList->RowCount > 1);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::Disass3MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    AnsiString t = ((TLabel*)Sender)->Caption;
    AnsiString t2 = "$"+t.SubString(17,4);
    SetMenuContent(StrToInt(t2));
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::udMemoryChangingEx(TObject *Sender,
      bool &AllowChange, short NewValue, TUpDownDirection Direction)
{
    AllowChange = false;
    SetBaseAddress(mBaseAddress + NewValue);
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::eMemAdrKeyPress(TObject *Sender, char &Key)
{
    if (Key == VK_RETURN)
    {
        char* endPtr;
        int entryValue = int(strtol(eMemAdr->Text.c_str(), &endPtr, 16));
        if ( ((*endPtr == 0) || (*endPtr == 0x14))  && (entryValue >= 0) && (entryValue <= maxMemory))
        {
            SetBaseAddress(entryValue);
        }
        else
          eMemAdr->Text="";
    }
}
//---------------------------------------------------------------------------

void __fastcall Tdebug::DumpMemory1Click(TObject *Sender)
{
    int address;
    // prise du point de référence de la souris
    TPoint cpm;
    GetCursorPos(&cpm);
    TPoint cp = ScreenToClient(cpm);

    if (mSelectedAddress!=-1)
    {
        // dump memory regarding type of memory
        address=mSelectedAddress;
        DumpMem->CentreOn(this);
        switch (cboMemory->ItemIndex) {
            case 0 : // ROM
                DumpMem->Dump2(address, 0xFFFF+1,0);
                break;
            case 1 : // VRAM
                DumpMem->Dump2(address, 0x3FFF+1,1);
                break;
            case 2 : // RAM
                DumpMem->Dump2(address, 0x3FF+1,2);
                break;
            case 3 : // SGM RAM
                DumpMem->Dump2(address, 0x7FFF+1,3);
                break;
        }
    }

}
//---------------------------------------------------------------------------

