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
    byte_t Read(const address_t addr) noexcept;
    void   Write(const address_t addr, const byte_t val) noexcept;

private:
    struct Opcode {
        enum class OperandType {
            NONE,
            R8, R16,
            N8, N16,
            E8, U3,
            CC, VEC
        };

        std::string mnemonic = "ERR";
        void (*exec)(SharpSM83&, const Opcode& op);
        
        OperandType op1;
        OperandType op2;
        
        byte_t cycles = 2;
        byte_t size = 1;
        byte_t opcode = 0x00;

    };

    std::vector<Opcode> opTable;
    // Opcode getOpcode(const byte_t op);

    static void nop(SharpSM83& cpu, const Opcode& op);

    static void ld(SharpSM83& cpu, const Opcode& op);
    static void ldh(SharpSM83& cpu, const Opcode& op);
    static void push(SharpSM83& cpu, const Opcode& op);
    static void pop(SharpSM83& cpu, const Opcode& op);

    static void add (SharpSM83& cpu, const Opcode& op);
    static void adc (SharpSM83& cpu, const Opcode& op);
    static void sub (SharpSM83& cpu, const Opcode& op);
    static void sbc (SharpSM83& cpu, const Opcode& op);
    static void and_(SharpSM83& cpu, const Opcode& op);
    static void or_ (SharpSM83& cpu, const Opcode& op);
    static void xor_(SharpSM83& cpu, const Opcode& op);
    static void cp  (SharpSM83& cpu, const Opcode& op);

    static void inc (SharpSM83& cpu, const Opcode& op);
    static void dec (SharpSM83& cpu, const Opcode& op);

    static void daa (SharpSM83& cpu, const Opcode& op);
    static void cpl (SharpSM83& cpu, const Opcode& op);
    static void scf (SharpSM83& cpu, const Opcode& op);
    static void ccf (SharpSM83& cpu, const Opcode& op);

    static void rlca(SharpSM83& cpu, const Opcode& op);
    static void rrca(SharpSM83& cpu, const Opcode& op);
    static void rla (SharpSM83& cpu, const Opcode& op);
    static void rra (SharpSM83& cpu, const Opcode& op);

    static void rlc (SharpSM83& cpu, const Opcode& op);
    static void rrc (SharpSM83& cpu, const Opcode& op);
    static void rl  (SharpSM83& cpu, const Opcode& op);
    static void rr  (SharpSM83& cpu, const Opcode& op);
    static void sla (SharpSM83& cpu, const Opcode& op);
    static void sra (SharpSM83& cpu, const Opcode& op);
    static void srl (SharpSM83& cpu, const Opcode& op);
    static void swap(SharpSM83& cpu, const Opcode& op);

    static void bit (SharpSM83& cpu, const Opcode& op);
    static void set (SharpSM83& cpu, const Opcode& op);
    static void res (SharpSM83& cpu, const Opcode& op);

    static void jp  (SharpSM83& cpu, const Opcode& op);
    static void jr  (SharpSM83& cpu, const Opcode& op);
    static void call(SharpSM83& cpu, const Opcode& op);
    static void ret (SharpSM83& cpu, const Opcode& op);
    static void reti(SharpSM83& cpu, const Opcode& op);
    static void rst (SharpSM83& cpu, const Opcode& op);

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