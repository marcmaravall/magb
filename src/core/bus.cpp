#include "bus.h"

Bus::Bus() {
    cpu = SharpSM83(shared_from_this());
    mem = std::array<byte_t, 0xFFFF>();
    mem.fill(0x00);
}

Bus::~Bus() {
    
}

byte_t Bus::read(const address_t addr) noexcept {
    return mem[addr];
    if (addr < 0x4000) {
        // bank 0
    } else if (addr < 0x8000) {
        // bank 1
    } else if (addr < 0xA000) {
        // vram
    } else if (addr < 0xC000) {
        // external ram
    } else if (addr < 0xD000) {
        // wram
    } else if (addr < 0xE000) {
        // cgb wram switchable
    } else if (addr < 0xFE00) {
        // echo ram PROHIBITED
    } else if (addr < 0xFEA0) {
        // oam
    } else if (addr < 0xFF00) {
        // not usable
    } else if (addr < 0xFF80) {
        // io registers
    } else if (addr < 0xFFFF) {
        // hram
    } else {
        // interrupt enable register
    }


    //return mem[addr];
    return 0x00;
}

void Bus::write(const address_t addr, const byte_t value) noexcept {
    mem[addr] = value;
    
    //(void)value;
    //(void)addr;
}
