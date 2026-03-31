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

public:
    enum class Reg8 {
        A = 0b111, 
        B = 0b000, 
        C = 0b010,
        D = 0b011, 
        E = 0b100,
        H = 0b101, 
        L = 0b110,
    };

    enum class Reg16 {
        BC = 0b00, 
        DE = 0b01, 
        HL = 0b10,
        SP = 0b11, 
        AF, 
        PC
	};

private:
    void   setReg8(const Reg8 r, const byte_t val);
    byte_t getReg8(const Reg8 r);

    void   setReg16(const Reg16 r, const uint16_t val);
    byte_t getReg16(const Reg16 r);

public:
    enum class AddrMode {
        None,

        Reg8,
        Reg16,

        Imm8,
        Imm16,
        Rel8,

        IndHL,
		IndHLInc,
		IndHLDec,
        IndBC,
        IndDE,
        IndC,
        Dir8,
        Dir16,

        CC,
        Vec
    };

    struct Operand {
        AddrMode mode;
        
        union {
            Reg8 r8;
			Reg16 r16;

			uint8_t n8;
			uint16_t n16;

			int8_t e8;
			uint8_t u3;

            bool cc;
            uint16_t vec;

            uint16_t raw;
        };
    };

    enum class Operation {
        LD,
        ADD, ADC, SUB, SBC, AND, OR, XOR, CP,
        INC, DEC,
        RLC, RRC, RL, RR, SLA, SRA, SRL, SWAP,
        BIT, SET, RES,
        NOP, HALT, STOP, DI, EI,
        JP, JR, CALL, RET, RST
    };

    struct Opcode {

        std::string mnemonic = "ERR";
        void (*exec)(SharpSM83&, const Opcode& op);
        
        Operand op1;
        Operand op2;
        
        byte_t cycles = 2;
        byte_t size = 1;
        byte_t opcode = 0x00;

    };

private:
    void writeOperand(const Operand& dest, const uint16_t val);
	uint16_t readOperand(const Operand& op);

private:

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

    static void stop(SharpSM83& cpu, const Opcode& op);
    static void halt(SharpSM83& cpu, const Opcode& op);
    static void di(SharpSM83& cpu, const Opcode& op);
    static void ei(SharpSM83& cpu, const Opcode& op);

private:
    // enum class RegDest {
    //     B = 0b000,
    //     C = 0b001,
    //     D = 0b010,
    //     E = 0b011,
    //     H = 0b100,
    //     L = 0b101,
	// 	_HL_ = 0b110,   // ptr to address in HL
	// 	A = 0b111
    // };

    Reg8 decodeRegister(const uint8_t z);
	void executeCB(const byte_t next);

    void writeRegToReg(const Reg8 dest, const Reg8 value);

	// void writeReg8(const RegDest reg, const byte_t val);
	// byte_t getReg8(const RegDest reg);

private:
	void decodeLdR8R8();
	void decodeAlu();

public:
    std::shared_ptr<Bus> bus;
public:
    SharpSM83() {}
    SharpSM83(const std::shared_ptr<Bus> bus);
    ~SharpSM83();

	void Reset();
    void Clock();

public:
    byte_t Read(const address_t addr) noexcept;
    void   Write(const address_t addr, const byte_t val) noexcept;
};

#endif // MAGB_CPU_H