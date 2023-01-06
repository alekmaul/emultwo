#ifndef __breakpoint_h
#define __breakpoint_h

#include "colecoconfig.h"

//If this order is changed then adjust the order in the Type list on the breakpoint dialog
enum BreakpointType
{
        BP_EXE = 0,
        BP_RD,
        BP_WR,
        BP_OUT,
        BP_OUTH,
        BP_OUTL,
        BP_IN,
        BP_INH,
        BP_INL,
        BP_TSTATES,
        BP_MEMORY,
        BP_REGISTER,
        BP_FLAG
};

enum BreakpointCondition
{
        Equal = 0,
        NotEqual,
        LessThanEquals,
        GreaterThanEquals,
        Range
};

enum RegisterType
{
        RegBC,
        RegDE,
        RegHL,
        RegIX,
        RegIY,
        RegPC,
        RegSP,
        RegAltBC,
        RegAltDE,
        RegAltHL,
        RegA,
        RegB,
        RegC,
        RegD,
        RegE,
        RegH,
        RegL,
        RegI,
        RegR,
        RegIXh,
        RegIXl,
        RegIYh,
        RegIYl,
        RegAltA,
        RegAltB,
        RegAltC,
        RegAltD,
        RegAltE,
        RegAltH,
        RegAltL
};

enum FlagType
{
        Carry = 0,
        AddSubtract,
        ParityOverflow,
        Bit3,
        HalfCarry,
        Bit5,
        Zero,
        Sign
};

struct breakpoint
{
    breakpoint(){}
    breakpoint(int addr, BreakpointType type)
    {
        Type = type;
        ConditionAddr = Equal;
        Addr = addr;
        ConditionValue = GreaterThanEquals;
        Value = addr;
        Permanent = true;
    }

    bool HitExe(BreakpointType reqType, int curAddr)
    {
        if (Type != reqType) return false;

        bool addrMatch = false;

        switch (ConditionAddr)
        {
            case Equal:
                addrMatch = (curAddr == Addr);
                break;

            case LessThanEquals:
                addrMatch = (curAddr <= Addr);
                break;

            case GreaterThanEquals:
                addrMatch = (curAddr >= Addr);
                break;

            case Range:
                addrMatch = (curAddr >= Addr) && (curAddr <= EndAddr);
                break;

            case NotEqual:
                addrMatch = (curAddr != Addr);
                break;
        }

        return addrMatch;
    }

    bool HitRdWrInOut(BreakpointType reqType, int curAddr, int curValue)
    {
        if (Type != reqType) return false;

        bool match = false;

        switch (ConditionAddr)
        {
            case Equal:
                match = (curAddr == Addr);
                break;

            case LessThanEquals:
                match = (curAddr <= Addr);
                break;

            case GreaterThanEquals:
                match = (curAddr >= Addr);
                break;

            case Range:
                match = (curAddr >= Addr) && (curAddr <= EndAddr);
                break;

            case NotEqual:
                match = (curAddr != Addr);
                break;
        }

        if (match && (Value != -1) && (ConditionAddr != Range))
        {
            switch (ConditionValue)
            {
                case Equal:
                    match = (curValue == Value);
                    break;

                case LessThanEquals:
                    match = (curValue <= Value);
                    break;

                case GreaterThanEquals:
                    match = (curValue >= Value);
                    break;

                case NotEqual:
                    match = (curValue != Value);
                    break;
                }
            }

            return match;
        }

        BreakpointType Type;
        BreakpointCondition ConditionAddr;
        union
        {
                int Addr;
                RegisterType RegisterId;
                FlagType FlagId;
        };
        BreakpointCondition ConditionValue;
        union
        {
                int Value;
                int EndAddr;
                int TStates;
        };
        bool Permanent;
};

#endif // __breakpoint_h

