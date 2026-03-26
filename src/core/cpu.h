#ifndef MAGB_CPU_H
#define MAGB_CPU_H

#include <string>
#include <vector>
#include <iostream>
#include <assert.h>

#include "typedefs.h"

class SharpSM83 
{
private:
    enum class Flag {
        ZERO = (1 << 7),
        NEGATIVE = (1 << 6),
        HALF_CARRY = (1 << 5),
        CARRY = (1 << 4)
    };

    word_t af;
    
    word_t bc;
    word_t de;
    word_t hl;

    word_t sp;
    word_t pc;

private:
    bool getFlag(const Flag flag) const;
    void setFlag(const Flag flag, const bool val);

private:
    enum class SubRegister {
        A, 
        B, C,
        D, E,
        H, L
    };

    void   setSubReg(const SubRegister r, const byte_t val);
    byte_t getSubReg(const SubRegister r);

private:
    struct Opcode {
        enum class OperandType {
            r8, r16,
            n8, n16,
            e8, u3,
            cc, vec
        };

        std::string mnemonic = "ERR";
        byte_t cycles = 2;
        byte_t size = 1;
        byte_t opcode = 0x00;

        OperandType op1;
        OperandType op2;

        int (*exec)();
    };

    std::vector<Opcode> opTable;

    Opcode getOpcode(const byte_t op);

    // int nop();
    // int ld ();
    // int adc();
public:
    SharpSM83();
    ~SharpSM83();

    void Clock();
};

#endif // MAGB_CPU_H