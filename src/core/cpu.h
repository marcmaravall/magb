#ifndef MAGB_CPU_H
#define MAGB_CPU_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <assert.h>

#include "typedefs.h"
class Bus;

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
    byte_t read(const address_t addr) noexcept;
    void   write(const address_t addr, const byte_t val) noexcept;

private:
    /*struct Opcode {
        enum class OperandType {
            NONE,
            R8, R16,
            N8, N16,
            E8, U3,
            CC, VEC
        };

        std::string mnemonic = "ERR";
        byte_t cycles = 2;
        byte_t size = 1;
        byte_t opcode = 0x00;

        OperandType op1;
        OperandType op2;

        int (*exec)();
    };*/

    // std::vector<Opcode> opTable;
    // Opcode getOpcode(const byte_t op);

    int nop();

    int ld(const address_t dest, const byte_t value);
    int ldh();
    int push();
    int pop();

    int add();
    int adc();
    int sub();
    int sbc();
    int and_();
    int or_();
    int xor_();
    int cp();

    int inc();
    int dec();

    int daa();
    int cpl();
    int scf();
    int ccf();

    int rlca();
    int rrca();
    int rla();
    int rra();

    int rlc();
    int rrc();
    int rl();
    int rr();
    int sla();
    int sra();
    int srl();
    int swap();

    int bit();
    int set();
    int res();

    int jp();
    int jr();
    int call();
    int ret();
    int reti();
    int rst();

    int stop();
    int halt();
    int di();
    int ei();
public:
    std::shared_ptr<Bus> bus;
public:
    SharpSM83() {}
    SharpSM83(const std::shared_ptr<Bus> bus);
    ~SharpSM83();

    void Clock();
};

#endif // MAGB_CPU_H