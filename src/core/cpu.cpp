#include "cpu.h"
#include "bus.h"

bool SharpSM83::getFlag(const Flag flag) const {
    return (af & (byte_t)flag);
}

void SharpSM83::setFlag(const Flag flag, const bool val) {
    if (val)
        af |= (byte_t)flag;
    else
        af &= ~(byte_t)flag;
}


byte_t SharpSM83::Read(const address_t addr) noexcept {
    return bus->Read(addr);
}

void SharpSM83::Write(const address_t addr, const byte_t val) noexcept {
    bus->Write(addr, val);
}

// TODO: do with less repetitions
void SharpSM83::setSubReg(const SubRegister r, const byte_t val) {
    switch (r) {
        case SubRegister::A: 
            af &= 0x00FF;
            af |= (word_t)val << 8;
            break;
        case SubRegister::B: 
            bc &= 0x00FF;
            bc |= (word_t)val << 8;
            break; 
        case SubRegister::C: 
            bc &= 0xFF00;
            bc |= val;
            break;
        case SubRegister::D: 
            de &= 0x00FF;
            de |= (word_t)val << 8;
            break; 
        case SubRegister::E: 
            de &= 0xFF00;
            de |= val;
            break;
        case SubRegister::H: 
            hl &= 0x00FF;
            hl |= (word_t)val << 8;
            break;
        case SubRegister::L: 
            break;
        
        [[unlikely]] default: 
            assert(0 && "ERROR: invalid subregister!");
    }
}

byte_t SharpSM83::getSubReg(const SubRegister r) {
    switch (r) {
        case SubRegister::A: return (af & 0x00FF) >> 8;
        case SubRegister::B: return (bc & 0x00FF) >> 8;
        case SubRegister::C: return (bc & 0xFF00);
        case SubRegister::D: return (de & 0x00FF) >> 8;
        case SubRegister::E: return (de & 0xFF00);
        case SubRegister::H: return (hl & 0x00FF) >> 8;
        case SubRegister::L: return (hl & 0xFF00);
        
        [[unlikely]] default: 
            assert(0 && "ERROR: invalid subregister!");
    }
    return 0x00;
}

// --- INSTRUCTIONS ---

void SharpSM83::nop(SharpSM83& cpu, const Opcode& op) {
	
}

void SharpSM83::ld(SharpSM83& cpu, const Opcode& op) {
    if (op.op1 == Opcode::OperandType::R8 && op.op2 == Opcode::OperandType::N8) {
        
    }
}

void SharpSM83::ldh(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::push(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::pop(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::add(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::adc(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::sub(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::sbc(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::and_(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::or_(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::xor_(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::cp(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::inc(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::dec(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::daa(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::cpl(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::scf(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::ccf(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::rlca(SharpSM83& cpu, const Opcode& op){
    
}

void SharpSM83::rrca(SharpSM83& cpu, const Opcode& op){
    
}

void SharpSM83::rla(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::rra(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::rlc (SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::rrc (SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::rl  (SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::rr  (SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::sla (SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::sra (SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::srl (SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::swap(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::bit(SharpSM83& cpu, const Opcode& op) {

}
void SharpSM83::set(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::res(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::jp  (SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::jr  (SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::call(SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::ret (SharpSM83& cpu, const Opcode& op) {

}

void SharpSM83::reti(SharpSM83& cpu, const Opcode& op) {

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

SharpSM83::SharpSM83(const std::shared_ptr<Bus> bus) {
    this->bus = bus;

    // hardcoded table (for now)
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

	// 0x40 - 0x7F its always LD r8, r8 or LD (HL), r8 or LD r8, (HL) or LD (HL), (HL)
    decodeLdR8R8();

    opTable[0x76] = Opcode{ "HALT", &SharpSM83::halt, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x76 };

	// alu: 0x80 - 0xBF its always ALU A, r8 or ALU A, (HL)
	decodeAlu();
}

void SharpSM83::decodeLdR8R8() {
    for (byte_t i = 0x40; i <= 0x7F; i++) {
        byte_t dest = (i & 0b00111000) >> 3;
        byte_t src  = (i & 0b00000111);
        if (dest == 6 && src == 6) {
            opTable[i] = Opcode{ "LD (HL), (HL)", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::R16, 8, 1, i };
        } else if (dest == 6) {
            opTable[i] = Opcode{ "LD (HL), r" + std::to_string(src), &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::R8, 8, 1, i };
        } else if (src == 6) {
            opTable[i] = Opcode{ "LD r" + std::to_string(dest) + ", (HL)", &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::R16, 8, 1, i };
        } else {
            opTable[i] = Opcode{ "LD r" + std::to_string(dest) + ", r" + std::to_string(src), &SharpSM83::ld, Opcode::OperandType::R8, Opcode::OperandType::R8, 4, 1, i };
        }
    }
}

SharpSM83::SubRegister SharpSM83::decodeRegister(const uint8_t z) {
	return static_cast<SubRegister>(z);
}

void SharpSM83::writeRegToReg(const SubRegister dest, const SubRegister value) {
	byte_t val = getSubReg(value);
	setSubReg(dest, val);
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
