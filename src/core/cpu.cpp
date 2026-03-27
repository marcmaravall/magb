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

// --- ------------ ---

SharpSM83::SharpSM83(const std::shared_ptr<Bus> bus) {
    this->bus = bus;

	opTable = std::vector<Opcode>(0x100);
    opTable[0x00] = Opcode{ "NOP", &SharpSM83::nop, Opcode::OperandType::NONE, Opcode::OperandType::NONE, 4, 1, 0x00 };
	opTable[0x01] = Opcode{ "LD BC, d16", &SharpSM83::ld, Opcode::OperandType::R16, Opcode::OperandType::N16, 12, 3, 0x01 };
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
