#include "cpu.h"
#include "bus.h"
#include "typedefs.h"
#include <cassert>
#include <cstdint>

constexpr bool SharpSM83::getFlag(const Flag flag) const {
    return (af & (byte_t)flag);
}

constexpr void SharpSM83::setFlag(const Flag flag, const bool val) {
    if (val)
        af |= (byte_t)flag;
    else
        af &= ~(byte_t)flag;
}

constexpr void SharpSM83::clearFlags() {
    af &= 0xFF0F;
}

constexpr void SharpSM83::setHalfCarry(const uint8_t val) {
    setFlag (Flag::CARRY, (val & 0xF) < ((val & 0xF) + getFlag(Flag::CARRY)));
}

byte_t SharpSM83::Read(const address_t addr) noexcept {
    return bus->Read(addr);
}

void SharpSM83::Write(const address_t addr, const byte_t val) noexcept {
    bus->Write(addr, val);
}

// TODO: do with less repetitions
void SharpSM83::setReg8(const Reg8 r, const byte_t val) {
    switch (r) {
        case Reg8::A: 
            af &= 0x00FF;
            af |= (word_t)val << 8;
            break;
        case Reg8::B: 
            bc &= 0x00FF;
            bc |= (word_t)val << 8;
            break; 
        case Reg8::C: 
            bc &= 0xFF00;
            bc |= val;
            break;
        case Reg8::D: 
            de &= 0x00FF;
            de |= (word_t)val << 8;
            break; 
        case Reg8::E: 
            de &= 0xFF00;
            de |= val;
            break;
        case Reg8::H: 
            hl &= 0x00FF;
            hl |= (word_t)val << 8;
            break;
        case Reg8::L: 
            break;
        
        [[unlikely]] default: 
            assert(0 && "ERROR: invalid subregister!");
    }
}

byte_t SharpSM83::getReg8(const Reg8 r) {
    switch (r) {
        case Reg8::A: return (af & 0x00FF) >> 8;
        case Reg8::B: return (bc & 0x00FF) >> 8;
        case Reg8::C: return (bc & 0xFF00);
        case Reg8::D: return (de & 0x00FF) >> 8;
        case Reg8::E: return (de & 0xFF00);
        case Reg8::H: return (hl & 0x00FF) >> 8;
        case Reg8::L: return (hl & 0xFF00);
        
        [[unlikely]] default: 
            assert(0 && "ERROR: invalid subregister!");
    }
    return 0x00;
}

void SharpSM83::setReg16(const Reg16 r, const uint16_t val) {
    switch (r) {
        case Reg16::AF: 
            af = val;
            break;
        case Reg16::BC: 
            bc = val;
			break;
        case Reg16::DE: 
			de = val;
            break;
		case Reg16::HL:
			de = val;
            break;
		case Reg16::SP:
            sp = val;
            break;
        case Reg16::PC:
            pc = val;
            break;
        
        [[unlikely]] default: 
			assert(0 && "ERROR: invalid register!");
    }
}

byte_t SharpSM83::getReg16(const Reg16 r) {
    switch (r) {
        case Reg16::AF: return af;
        case Reg16::BC: return bc;
        case Reg16::DE: return de;
        case Reg16::HL: return hl;
        case Reg16::SP: return sp;
        case Reg16::PC: return pc;

	[[unlikely]] default: assert(0 && "ERROR: invalid register!");
    }

    return 0x00;
}

constexpr bool SharpSM83::isCondition(const Condition c) {
    bool carry = getFlag(Flag::CARRY);
    bool zero  = getFlag(Flag::ZERO);
    switch (c) {
        case Condition::C:
            return carry;
        case Condition::NC:
            return !carry;
        case Condition::Z:
            return zero;
        case Condition::NZ:
            return !zero;
    }

    assert(0 && "ERROR: invalid condition!");
    return false;
}

// --- INSTRUCTIONS ---

void SharpSM83::nop(SharpSM83& cpu, const Opcode& op) {
	// nothing
}

void SharpSM83::ld(SharpSM83& cpu, const Opcode& op) {
    uint16_t val = cpu.readOperand(op.op2);
    cpu.writeOperand(op.op1, val);
}

void SharpSM83::ldh(SharpSM83& cpu, const Opcode& op) {
    uint16_t val = cpu.readOperand(op.op2);
    cpu.writeOperand(op.op1, val);
}

void SharpSM83::push(SharpSM83& cpu, const Opcode& op) {
    uint16_t val = cpu.readOperand(op.op1);
    cpu.Write(--cpu.sp, (val & 0xFF00) >> 8);
    cpu.Write(--cpu.sp, (val & 0x00FF));
}

void SharpSM83::pop(SharpSM83& cpu, const Opcode& op) {
    uint16_t val = cpu.Read(cpu.sp++);
    val |= cpu.Read(cpu.sp++) << 8;
    cpu.writeOperand(op.op1, val);
}

void SharpSM83::add(SharpSM83& cpu, const Opcode& op) {
    byte_t first = cpu.readOperand(op.op1);
    byte_t val = cpu.readOperand(op.op2);
    byte_t carry = cpu.getFlag(Flag::CARRY);
    uint16_t res = cpu.readOperand(op.op1) + val;

    cpu.writeOperand(op.op1, (res & 0xFF));
    cpu.setFlag(Flag::ZERO, (res&0xFF) == 0);
    cpu.setFlag(Flag::NEGATIVE, 0);
    cpu.setFlag(Flag::HALF_CARRY, ((first & 0xF) + (val & 0xF) + carry) > 0xF);
    cpu.setFlag(Flag::CARRY, res > 0xFF);
}

void SharpSM83::adc(SharpSM83& cpu, const Opcode& op) {
    byte_t a = cpu.getReg8(Reg8::A);
    byte_t val = cpu.readOperand(op.op2);
    byte_t carry = cpu.getFlag(Flag::CARRY);

    uint16_t res = a + val + carry;

    cpu.setFlag(Flag::ZERO, (res & 0xFF) == 0);
    cpu.setFlag(Flag::NEGATIVE, 0);
    cpu.setFlag(Flag::HALF_CARRY, ((a & 0xF) + (val & 0xF) + carry) > 0xF);
    cpu.setFlag(Flag::CARRY, res > 0xFF);

    cpu.setReg8(Reg8::A, res & 0xFF);
}

void SharpSM83::sub(SharpSM83& cpu, const Opcode& op) {
    byte_t a = cpu.readOperand(op.op1);     // this doesnt represent registers
    byte_t b = cpu.readOperand(op.op2);
    byte_t val = a - b;
    cpu.writeOperand(op.op1, val);
    cpu.setFlag(Flag::ZERO, val == 0);
    cpu.setFlag(Flag::NEGATIVE, 1);
    cpu.setHalfCarry(val);
    cpu.setFlag(Flag::CARRY, b > a);
}

void SharpSM83::sbc(SharpSM83& cpu, const Opcode& op) {
    uint8_t a = cpu.getReg8(Reg8::A);
    uint8_t val = cpu.readOperand(op.op2);
    uint8_t carry = cpu.getFlag(Flag::CARRY);

    uint16_t res = a - val - carry;

    cpu.setFlag(Flag::ZERO, (res & 0xFF) == 0);
    cpu.setFlag(Flag::NEGATIVE, 1);
    cpu.setFlag(Flag::HALF_CARRY, (a & 0xF) < ((val & 0xF) + carry));
    cpu.setFlag(Flag::CARRY, a < (val + carry));

    cpu.setReg8(Reg8::A, res & 0xFF);
}

void SharpSM83::and_(SharpSM83& cpu, const Opcode& op) {
    byte_t val = (byte_t)cpu.readOperand(op.op2);
    val &= cpu.readOperand(op.op1) & 0xFF;
    cpu.clearFlags();
    cpu.setFlag(Flag::ZERO, val == 0);
    
    cpu.writeOperand(op.op1, val);
}

void SharpSM83::or_(SharpSM83& cpu, const Opcode& op) {
    byte_t val = (byte_t)cpu.readOperand(op.op2);
    val |= cpu.readOperand(op.op1) & 0xFF;
    cpu.clearFlags();
    cpu.setFlag(Flag::ZERO, val == 0);
    
    cpu.writeOperand(op.op1, val);
}

void SharpSM83::xor_(SharpSM83& cpu, const Opcode& op) {
    byte_t val = (byte_t)cpu.readOperand(op.op2);
    val ^= cpu.readOperand(op.op1) & 0xFF;
    cpu.clearFlags();
    cpu.setFlag(Flag::ZERO, val == 0);
    
    cpu.writeOperand(op.op1, val);
}

void SharpSM83::cp(SharpSM83& cpu, const Opcode& op) {
    byte_t a = cpu.readOperand(op.op1);
    byte_t b = cpu.readOperand(op.op2);
    byte_t val = a-b;
    cpu.setFlag(Flag::ZERO, val == 0);
    cpu.setFlag(Flag::NEGATIVE, 1);
    cpu.setHalfCarry(val);
    cpu.setFlag(Flag::CARRY, b > a);
}

void SharpSM83::inc(SharpSM83& cpu, const Opcode& op) {
    if (op.op1.mode == AddrMode::Reg16) {
        // 16
        uint16_t val = cpu.readOperand(op.op1)+1;
        cpu.writeOperand(op.op1, val);
        cpu.setFlag(Flag::ZERO, val == 0);
        cpu.setHalfCarry(val);
    } else {
        // 8
        byte_t val = ((byte_t)cpu.readOperand(op.op1))+1;
        cpu.writeOperand(op.op1, val);
        cpu.setFlag(Flag::ZERO, val == 0);
        cpu.setHalfCarry(val);
    }
    cpu.setFlag(Flag::NEGATIVE, 0);
}

void SharpSM83::dec(SharpSM83& cpu, const Opcode& op) {
    if (op.op1.mode == AddrMode::Reg16) {
        // 16
        uint16_t val = cpu.readOperand(op.op1)-1;
        cpu.writeOperand(op.op1, val);
        cpu.setFlag(Flag::ZERO, val == 0);
        cpu.setHalfCarry(val);
    } else {
        // 8
        byte_t val = ((byte_t)cpu.readOperand(op.op1))-1;
        cpu.writeOperand(op.op1, val);
        cpu.setFlag(Flag::ZERO, val == 0);
        cpu.setHalfCarry(val);
    }
    cpu.setFlag(Flag::NEGATIVE, 1);
}

void SharpSM83::daa(SharpSM83& cpu, const Opcode& op) {
    bool n = cpu.getFlag(Flag::NEGATIVE);
    bool h = cpu.getFlag(Flag::HALF_CARRY);
    bool c = cpu.getFlag(Flag::CARRY);
    byte_t a = cpu.getReg8(Reg8::A);
    byte_t adj = 0;

    if (n) {
        if (h)
            adj += 0x06;
        if (c)
            adj += 0x60;
        cpu.setReg8(Reg8::A, a-adj);
    } else {
        if (h || a > 0x09) 
            adj += 0x06;
        if (c || a > 0x99) {
            cpu.setFlag(Flag::CARRY, 1);
        }
        cpu.setReg8(Reg8::A, a + adj);
    }

    cpu.setFlag(Flag::HALF_CARRY, 0);
    cpu.setFlag(Flag::ZERO, cpu.getReg8(Reg8::A) == 0);
}

void SharpSM83::cpl(SharpSM83& cpu, const Opcode& op) {
    cpu.setReg8(Reg8::A, ~cpu.getReg8(Reg8::A));
    cpu.setFlag(Flag::NEGATIVE, 1);
    cpu.setFlag(Flag::HALF_CARRY, 1);
}

void SharpSM83::scf(SharpSM83& cpu, const Opcode& op) {
	cpu.setFlag(Flag::CARRY, 1);
    cpu.setFlag(Flag::HALF_CARRY, 0);
    cpu.setFlag(Flag::NEGATIVE, 0);
}

void SharpSM83::ccf(SharpSM83& cpu, const Opcode& op) {
    cpu.setFlag(Flag::CARRY, !cpu.getFlag(Flag::CARRY));
    cpu.setFlag(Flag::HALF_CARRY, 0);
    cpu.setFlag(Flag::NEGATIVE, 0);
}

void SharpSM83::rlca(SharpSM83& cpu, const Opcode& op){
	bool carry = cpu.getFlag(Flag::CARRY);
    byte_t a  = cpu.getReg8(Reg8::A);
    bool newCarry = a & 0x80;
    
    a <<= 1;
    a |= carry;
    
    cpu.setFlag(Flag::CARRY, newCarry);
    cpu.setFlag(Flag::NEGATIVE, 0);
    cpu.setFlag(Flag::HALF_CARRY, 0);
    cpu.setFlag(Flag::ZERO, 0);
	cpu.setReg8(Reg8::A, a);
}

void SharpSM83::rrca(SharpSM83& cpu, const Opcode& op){
	bool carry = cpu.getFlag(Flag::CARRY);
	byte_t a = cpu.getReg8(Reg8::A);
	bool newCarry = a & 0x01;

	a >>= 1;
	a |= (carry << 7);
    
	cpu.setFlag(Flag::CARRY, newCarry);
    cpu.setFlag(Flag::NEGATIVE, 0);
    cpu.setFlag(Flag::HALF_CARRY, 0);
    cpu.setFlag(Flag::ZERO, 0);
    cpu.setReg8(Reg8::A, a);
}

void SharpSM83::rla(SharpSM83& cpu, const Opcode& op) {
	bool carry = cpu.getFlag(Flag::CARRY);
	cpu.setFlag(Flag::CARRY, cpu.getReg8(Reg8::A) & 0x80);
	byte_t a = cpu.getReg8(Reg8::A);
	a <<= 1;
	a |= carry;

    cpu.setFlag(Flag::NEGATIVE, 0);
    cpu.setFlag(Flag::HALF_CARRY, 0);
    cpu.setFlag(Flag::ZERO, 0);
    cpu.setReg8(Reg8::A, a);
}

void SharpSM83::rra(SharpSM83& cpu, const Opcode& op) {
    bool carry = cpu.getFlag(Flag::CARRY);
    cpu.setFlag(Flag::CARRY, cpu.getReg8(Reg8::A) & 1);
    byte_t a = cpu.getReg8(Reg8::A);
    a >>= 1;
    a |= (carry << 7);

    cpu.setFlag(Flag::NEGATIVE, 0);
    cpu.setFlag(Flag::HALF_CARRY, 0);
    cpu.setFlag(Flag::ZERO, 0);
    cpu.setReg8(Reg8::A, a);
}

void SharpSM83::rlc (SharpSM83& cpu, const Opcode& op) {
	byte_t value = cpu.readOperand(op.op1);
    bool newCarry = value & 0x80;
    byte_t res = (value << 1) | (value >> 7);
    cpu.setFlag(Flag::CARRY, newCarry);
    cpu.setFlag(Flag::NEGATIVE, 0);
    cpu.setFlag(Flag::HALF_CARRY, 0);
    cpu.setFlag(Flag::ZERO, res == 0);
	cpu.writeOperand(op.op1, res);
}

void SharpSM83::rrc (SharpSM83& cpu, const Opcode& op) {
    byte_t value = cpu.readOperand(op.op1);
    bool newCarry = value & 1;
    byte_t res = (value >> 1) | (value << 7);
    
    cpu.setFlag(Flag::CARRY, newCarry);
    cpu.setFlag(Flag::NEGATIVE, 0);
    cpu.setFlag(Flag::HALF_CARRY, 0);
    cpu.setFlag(Flag::ZERO, res == 0);

    cpu.writeOperand(op.op1, res);
}

void SharpSM83::rl(SharpSM83& cpu, const Opcode& op) {
    byte_t value = cpu.readOperand(op.op1);
    bool oldCarry = cpu.getFlag(Flag::CARRY);
    bool newCarry = value & 0x80;
    byte_t result = (value << 1) | oldCarry;

    cpu.setFlag(Flag::CARRY, newCarry);
    cpu.setFlag(Flag::NEGATIVE, 0);
    cpu.setFlag(Flag::HALF_CARRY, 0);
    cpu.setFlag(Flag::ZERO, result == 0);
    cpu.writeOperand(op.op1, result);
}

void SharpSM83::rr  (SharpSM83& cpu, const Opcode& op) {
    uint16_t val = cpu.readOperand(op.op1);
    bool carry = val & 0x01;
    val >>= 1;
    val |= (cpu.getFlag(Flag::CARRY) << 7);
    
    cpu.setFlag(Flag::CARRY, carry);
    cpu.setFlag(Flag::NEGATIVE, 0);
    cpu.setFlag(Flag::HALF_CARRY, 0);
    cpu.setFlag(Flag::ZERO, (val & 0xFF) == 0);
    cpu.writeOperand(op.op1, val&0xFF);
}

void SharpSM83::sla(SharpSM83& cpu, const Opcode& op) {
    byte_t r8 = cpu.readOperand(op.op1);
    bool carry = r8 & (1 << 7);
    byte_t res = r8 << 1;
    cpu.clearFlags();
    cpu.setFlag(Flag::CARRY, carry);
    cpu.setFlag(Flag::ZERO, res == 0);
    cpu.writeOperand(op.op1, res);
}

void SharpSM83::sra(SharpSM83& cpu, const Opcode& op) {
    byte_t r8 = cpu.readOperand(op.op1);
    bool carry = r8 & 1;
    byte_t res = (r8 >> 1) | (r8 & 0x80);
    cpu.clearFlags();
    cpu.setFlag(Flag::CARRY, carry);
    cpu.setFlag(Flag::ZERO, res == 0);
    cpu.writeOperand(op.op1, res);
}

void SharpSM83::srl(SharpSM83& cpu, const Opcode& op) {
    byte_t r8 = cpu.readOperand(op.op1);
    bool carry = r8 & 1;
    byte_t res = r8 >> 1;
    cpu.clearFlags();
    cpu.setFlag(Flag::CARRY, carry);
    cpu.setFlag(Flag::ZERO, res == 0);
    cpu.writeOperand(op.op1, res);
}

void SharpSM83::swap(SharpSM83& cpu, const Opcode& op) {
    byte_t val = cpu.readOperand(op.op1);
    byte_t upper = val & 0x0F >> 4;
    byte_t lower = val & 0xF0;
    val = upper | lower << 4;

    cpu.clearFlags();
    cpu.setFlag(Flag::ZERO, val);
    cpu.writeOperand(op.op1, val);
}

void SharpSM83::bit(SharpSM83& cpu, const Opcode& op) {

}
void SharpSM83::set(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::res(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::jp  (SharpSM83& cpu, const Opcode& op) {
    uint16_t addr = cpu.readOperand(op.op1);
    cpu.pc = addr;
}

void SharpSM83::jr  (SharpSM83& cpu, const Opcode& op) {
    if (op.op2.mode != AddrMode::None) {
        // conditional jump
        // TODO: implement
        
    } else {
        int8_t offset = (int8_t)cpu.readOperand(op.op1);
        cpu.pc += offset;
    }
}

void SharpSM83::call(SharpSM83& cpu, const Opcode& op) {
    if (op.op1.mode == AddrMode::CC) {
        // TODO: implement:
    } else {
        
    }
}

void SharpSM83::ret (SharpSM83& cpu, const Opcode& op) {
    if (op.op1.mode == AddrMode::CC) {
        // TODO: implement
        if (op.op1.cc) {
            
        } else {

        }
    } else {
        cpu.pc = cpu.stackPop16();
    }
}

void SharpSM83::reti(SharpSM83& cpu, const Opcode& op) {
    cpu.ei(cpu, op);
    cpu.ret(cpu, op);
}

void SharpSM83::rst (SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::stop(SharpSM83& cpu, const Opcode& op) {
    
}

void SharpSM83::halt(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::di(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::ei(SharpSM83& cpu, const Opcode& op) {

}

// --- ------------ ---

void SharpSM83::stackPush8(const byte_t val) {
    Write(--sp, val);
}

void SharpSM83::stackPush16(const uint16_t val) {
    Write(sp--, (val & 0xFF00) >> 8);
    Write(sp--, (val&0xFF));
}

byte_t SharpSM83::stackPop8 () {
    byte_t res = Read(sp++);
    return res;
}

uint16_t SharpSM83::stackPop16() {
    uint16_t res = Read(sp++);
    res |= Read(sp++) << 8;
    return res;
}

SharpSM83::SharpSM83(const std::shared_ptr<Bus> bus) {
    this->bus = bus;

    // hardcoded table (for now)
    /*
	opTable = std::vector<Opcode>(0x100);
    opTable[0x00] = Opcode{ "NOP", &SharpSM83::nop, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x00 };
	opTable[0x01] = Opcode{ "LD BC, d16", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::N16, 12, 3, 0x01 };
	opTable[0x02] = Opcode{ "LD (BC), A", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::R8, 8, 1, 0x02 };
	opTable[0x03] = Opcode{ "INC BC", &SharpSM83::inc, Opcode::OperandType::R16, Opcode::OperandType::NONE, 8, 1, 0x03 };
	opTable[0x04] = Opcode{ "INC B", &SharpSM83::inc, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x04 };
	opTable[0x05] = Opcode{ "DEC B", &SharpSM83::dec, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x05 };
	opTable[0x06] = Opcode{ "LD B, d8", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::N8, 8, 2, 0x06 };
    opTable[0x07] = Opcode{ "RLCA", &SharpSM83::rlca, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x07 };
	opTable[0x08] = Opcode{ "LD (a16), SP", &SharpSM83::ld, Opcode::OperandType::E8, Opcode::OperandType::R16, 20, 3, 0x08 };
	opTable[0x09] = Opcode{ "ADD HL, BC", &SharpSM83::add, Opcode::OperandType::R16, Opcode::OperandType::R16, 8, 1, 0x09 };
	opTable[0x0A] = Opcode{ "LD A, (BC)", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::R16, 8, 1, 0x0A };
	opTable[0x0B] = Opcode{ "DEC BC", &SharpSM83::dec, Opcode::OperandType::R16, Opcode::OperandType::NONE, 8, 1, 0x0B };
	opTable[0x0C] = Opcode{ "INC C", &SharpSM83::inc, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x0C };
	opTable[0x0D] = Opcode{ "DEC C", &SharpSM83::dec, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x0D };
	opTable[0x0E] = Opcode{ "LD C, d8", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::N8, 8, 2, 0x0E };
	opTable[0x0F] = Opcode{ "RRC A", &SharpSM83::rrca, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x0F };

    opTable[0x10] = Opcode{ "STOP", &SharpSM83::stop, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 2, 0x10 };
    opTable[0x11] = Opcode{ "LD DE, d16", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::N16, 12, 3, 0x11 };
    opTable[0x12] = Opcode{ "LD (DE), A", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::R8, 8, 1, 0x12 };
    opTable[0x13] = Opcode{ "INC DE", &SharpSM83::inc, Opcode::OperandType::R16, Opcode::OperandType::NONE, 8, 1, 0x13 };
    opTable[0x14] = Opcode{ "INC D", &SharpSM83::inc, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x14 };
    opTable[0x15] = Opcode{ "DEC D", &SharpSM83::dec, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x15 };
    opTable[0x16] = Opcode{ "LD D, d8", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::N8, 8, 2, 0x16 };
    opTable[0x17] = Opcode{ "RLA", &SharpSM83::rla, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x17 };
    opTable[0x18] = Opcode{ "JR r8", &SharpSM83::jr, Opcode::OperandType::N8, Opcode::OperandType::NONE, 12, 2, 0x18 };
    opTable[0x19] = Opcode{ "ADD HL, DE", &SharpSM83::add, Opcode::OperandType::R16, Opcode::OperandType::R16, 8, 1, 0x19 };
    opTable[0x1A] = Opcode{ "LD A, (DE)", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::R16, 8, 1, 0x1A };
    opTable[0x1B] = Opcode{ "DEC DE", &SharpSM83::dec, Opcode::OperandType::R16, Opcode::OperandType::NONE, 8, 1, 0x1B };
    opTable[0x1C] = Opcode{ "INC E", &SharpSM83::inc, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x1C };
    opTable[0x1D] = Opcode{ "DEC E", &SharpSM83::dec, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x1D };
    opTable[0x1E] = Opcode{ "LD E, d8", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::N8, 8, 2, 0x1E };
    opTable[0x1F] = Opcode{ "RRA", &SharpSM83::rra, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x1F };

    opTable[0x20] = Opcode{ "JR NZ, r8", &SharpSM83::jr, Opcode::OperandType::CC, Opcode::OperandType::N8, 12, 2, 0x20 };
    opTable[0x21] = Opcode{ "LD HL, d16", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::N16, 12, 3, 0x21 };
    opTable[0x22] = Opcode{ "LD (HL+), A", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::R8, 8, 1, 0x22 };
    opTable[0x23] = Opcode{ "INC HL", &SharpSM83::inc, Opcode::OperandType::R16, Opcode::OperandType::NONE, 8, 1, 0x23 };
    opTable[0x24] = Opcode{ "INC H", &SharpSM83::inc, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x24 };
    opTable[0x25] = Opcode{ "DEC H", &SharpSM83::dec, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x25 };
    opTable[0x26] = Opcode{ "LD H, d8", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::N8, 8, 2, 0x26 };
    opTable[0x27] = Opcode{ "DAA", &SharpSM83::daa, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x27 };
    opTable[0x28] = Opcode{ "JR Z, r8", &SharpSM83::jr, Opcode::OperandType::CC, Opcode::OperandType::N8, 12, 2, 0x28 };
    opTable[0x29] = Opcode{ "ADD HL, HL", &SharpSM83::add, Opcode::OperandType::R16, Opcode::OperandType::R16, 8, 1, 0x29 };
    opTable[0x2A] = Opcode{ "LD A, (HL+)", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::R16, 8, 1, 0x2A };
    opTable[0x2B] = Opcode{ "DEC HL", &SharpSM83::dec, Opcode::OperandType::R16, Opcode::OperandType::NONE, 8, 1, 0x2B };
    opTable[0x2C] = Opcode{ "INC L", &SharpSM83::inc, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x2C };
    opTable[0x2D] = Opcode{ "DEC L", &SharpSM83::dec, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x2D };
    opTable[0x2E] = Opcode{ "LD L, d8", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::N8, 8, 2, 0x2E };
    opTable[0x2F] = Opcode{ "CPL", &SharpSM83::cpl, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x2F };

    opTable[0x30] = Opcode{ "JR NC, r8", &SharpSM83::jr, Opcode::OperandType::CC, Opcode::OperandType::N8, 12, 2, 0x30 };
    opTable[0x31] = Opcode{ "LD SP, d16", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::N16, 12, 3, 0x31 };
    opTable[0x32] = Opcode{ "LD (HL-), A", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::R8, 8, 1, 0x32 };
    opTable[0x33] = Opcode{ "INC SP", &SharpSM83::inc, Opcode::OperandType::R16, Opcode::OperandType::NONE, 8, 1, 0x33 };
    opTable[0x34] = Opcode{ "INC (HL)", &SharpSM83::inc, Opcode::OperandType::R16, Opcode::OperandType::NONE, 12, 1, 0x34 };
    opTable[0x35] = Opcode{ "DEC (HL)", &SharpSM83::dec, Opcode::OperandType::R16, Opcode::OperandType::NONE, 12, 1, 0x35 };
    opTable[0x36] = Opcode{ "LD (HL), d8", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::N8, 12, 2, 0x36 };
    opTable[0x37] = Opcode{ "SCF", &SharpSM83::scf, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x37 };
    opTable[0x38] = Opcode{ "JR C, r8", &SharpSM83::jr, Opcode::OperandType::CC, Opcode::OperandType::N8, 12, 2, 0x38 };
    opTable[0x39] = Opcode{ "ADD HL, SP", &SharpSM83::add, Opcode::OperandType::R16, Opcode::OperandType::R16, 8, 1, 0x39 };
    opTable[0x3A] = Opcode{ "LD A, (HL-)", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::R16, 8, 1, 0x3A };
    opTable[0x3B] = Opcode{ "DEC SP", &SharpSM83::dec, Opcode::OperandType::R16, Opcode::OperandType::NONE, 8, 1, 0x3B };
    opTable[0x3C] = Opcode{ "INC A", &SharpSM83::inc, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x3C };
    opTable[0x3D] = Opcode{ "DEC A", &SharpSM83::dec, Opcode::OperandType::R8, Opcode::OperandType::NONE, 4, 1, 0x3D };
    opTable[0x3E] = Opcode{ "LD A, d8", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::N8, 8, 2, 0x3E };
    opTable[0x3F] = Opcode{ "CCF", &SharpSM83::ccf, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x3F };
    */
	// 0x40 - 0x7F its always LD r8, r8 or LD (HL), r8 or LD r8, (HL) or LD (HL), (HL)
    decodeLdR8R8();

    // opTable[0x76] = Opcode{"HALT", &SharpSM83::halt, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x76};
   
	// alu: 0x80 - 0xBF its always ALU A, r8 or ALU A, (HL)
	decodeAlu();
}

void SharpSM83::decodeLdR8R8() {
    for (byte_t i = 0x40; i <= 0x7F; i++) {
        byte_t dest = (i & 0b00111000) >> 3;
        byte_t src  = (i & 0b00000111);
        if (dest == 6 && src == 6) {
        //    opTable[i] = Opcode{ "LD (HL), (HL)", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::R16, 8, 1, i };
        } else if (dest == 6) {
        //    opTable[i] = Opcode{ "LD (HL), r" + std::to_string(src), &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::R8, 8, 1, i };
        } else if (src == 6) {
        //    opTable[i] = Opcode{ "LD r" + std::to_string(dest) + ", (HL)", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::R16, 8, 1, i };
        } else {
        //    opTable[i] = Opcode{ "LD r" + std::to_string(dest) + ", r" + std::to_string(src), &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::R8, 4, 1, i };
        }
    }
}

SharpSM83::Reg8 SharpSM83::decodeRegister(const uint8_t z) {
	return static_cast<Reg8>(z);
}

// void SharpSM83::writeRegToReg(const Reg8 dest, const Reg8 value) {
// 	byte_t val = getReg8(value);
// 	setReg8(dest, val);
// }
// 
// void SharpSM83::writeImmToReg(const Reg8 dest, const byte_t val) {
//     setReg8(dest, val);
// }

uint16_t SharpSM83::readOperand(const Operand& op) {
    switch (op.mode) {

    case AddrMode::Reg8:
        return getReg8((Reg8)op.r8);

    case AddrMode::Reg16:
        return getReg16((Reg16)op.r16);

    case AddrMode::Imm8:
        return op.n8;

    case AddrMode::Imm16:
        return op.n16;

    case AddrMode::Rel8:
        return (int8_t)op.e8;

    case AddrMode::IndHL:
        return Read(hl);

    case AddrMode::IndHLInc: {
        uint8_t val = Read(hl++); 
        return val;
    }

    case AddrMode::IndHLDec: {
        uint8_t val = Read(hl--);
        return val;
    }

    case AddrMode::IndBC:
        return Read(bc);

    case AddrMode::IndDE:
        return Read(de);

    case AddrMode::IndC: {
        uint16_t addr = 0xFF00 + getReg8(Reg8::C);
        return Read(addr);
    }

    case AddrMode::Dir8: {
        uint16_t addr = 0xFF00 + op.n8;
        return Read(addr);
    }

    case AddrMode::Dir16:
        return Read(op.n16);

    [[unlikely]] case AddrMode::CC:
    [[unlikely]] case AddrMode::Vec:
    [[unlikely]] case AddrMode::None:
    [[unlikely]] default:
        return 0;
    }
}

void SharpSM83::writeOperand(const Operand& dest, const uint16_t val) {
    switch (dest.mode) {

    case AddrMode::Reg8:
        setReg8((Reg8)dest.r8, (uint8_t)val);
        break;

    case AddrMode::Reg16:
        setReg16((Reg16)dest.r16, val);
        break;

    case AddrMode::IndHL:
        Write(getReg16(Reg16::HL), (uint8_t)val);
        break;

    case AddrMode::IndHLInc: {
        Write(hl++, (uint8_t)val);
        break;
    }

    case AddrMode::IndHLDec: {
        Write(hl--, (uint8_t)val);
        break;
    }

    case AddrMode::IndBC:
        Write(bc, (uint8_t)val);
        break;

    case AddrMode::IndDE:
        Write(de, (uint8_t)val);
        break;

    case AddrMode::IndC: {
        uint16_t addr = 0xFF00 + getReg8(Reg8::C);
        Write(addr, (uint8_t)val);
        break;
    }

    case AddrMode::Dir8: {
        uint16_t addr = 0xFF00 + dest.n8;
        Write(addr, (uint8_t)val);
        break;
    }

    case AddrMode::Dir16:
        Write(dest.n16, (uint8_t)val);
        break;

    [[unlikely]]case AddrMode::Imm8:
    [[unlikely]]case AddrMode::Imm16:
    [[unlikely]]case AddrMode::Rel8:
    [[unlikely]]case AddrMode::CC:
    [[unlikely]]case AddrMode::Vec:
    [[unlikely]]case AddrMode::None:
    
    [[unlikely]]default:
        break;
    }
}

/*
void SharpSM83::writeReg8(const RegDest reg, const byte_t val) {
    switch (reg)
    {
	case RegDest::B:
		setSubReg(SubRegister::B, val);
	case RegDest::C:
		setSubReg(SubRegister::C, val);
	case RegDest::D:
		setSubReg(SubRegister::D, val);
	case RegDest::E:
		setSubReg(SubRegister::E, val);
	case RegDest::H:
		setSubReg(SubRegister::H, val);
	case RegDest::L:
		setSubReg(SubRegister::L, val);
	case RegDest::A:
		setSubReg(SubRegister::A, val);
	case RegDest::_HL_:
        Write(hl, val);

	[[unlikely]] default: 
        assert(0 && "ERROR: invalid register!");
    }
}

byte_t SharpSM83::getReg8(const RegDest reg) {
    switch (reg)
    {
    case RegDest::B:
		return getSubReg(SubRegister::B);
	case RegDest::C:
		return getSubReg(SubRegister::C);
	case RegDest::D:
		return getSubReg(SubRegister::D);
	case RegDest::E:
		return getSubReg(SubRegister::E);
	case RegDest::H:
		return getSubReg(SubRegister::H);
	case RegDest::L:
		return getSubReg(SubRegister::L);
	case RegDest::A:
		return getSubReg(SubRegister::A);
    case RegDest::_HL_:
		return Read(hl);    // this is not a subregister but its used

	[[unlikely]] default: 
        assert(0 && "ERROR: invalid register!");
    }
}*/

void executeCB(const byte_t next) {
	const byte_t group = (next & 0b11000000) >> 6;
	const byte_t y = (next & 0b00111000) >> 3;
	const byte_t z = (next & 0b00000111);

    // TODO: implement
    switch (group) {
        case 0b00: // RLC, RRC, RL, RR, SLA, SRA, SRL, SWAP
            
            switch (y) {
                case 0b000: // RLC
                    
                    break;
                case 0b001: // RRC
                    
                    break;
                case 0b010: // RL
                    
                    break;
                case 0b011: // RR
                    
                    break;
                case 0b100: // SLA
                    
                    break;
                case 0b101: // SRA
                    
                    break;
                case 0b110: // SRL
                    
                    break;
                case 0b111: // SWAP
					
                    break;
            }

            break;
        case 0b01: // BIT 
            break;
        case 0b10: // RES 
            break;
        case 0b11: // SET 

            break;
	}
}

void SharpSM83::decodeAlu() {
    // TODO: implement
}

void SharpSM83::Reset() {
    pc = 0x0000;
    sp = 0xFFFE;
    af = 0x01B0;
    bc = 0x0013;
    de = 0x00D8;
    hl = 0x014D;
}

SharpSM83::~SharpSM83() {

}

void SharpSM83::Clock() {
    byte_t opcode = Read(pc);
    if (opcode == 0x00) {
        // for testing
        std::cout << "nop\n";
    }
    pc++;

}
