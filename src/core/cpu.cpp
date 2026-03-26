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


byte_t SharpSM83::read(const address_t addr) noexcept {
    return bus->read(addr);
}

void SharpSM83::write(const address_t addr, const byte_t val) noexcept {
    bus->write(addr, val);
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

int SharpSM83::ld(const address_t dest, const byte_t value) {
    write(dest, value);
    return 0;
}

// --- ------------ ---

SharpSM83::SharpSM83(const std::shared_ptr<Bus> bus) {
    this->bus = bus;
}

SharpSM83::~SharpSM83() {

}

void SharpSM83::Clock() {
    byte_t opcode = read(pc);
    if (opcode == 0x00) {
        // for testing
        std::cout << "nop\n";
    }
    pc++;

}
