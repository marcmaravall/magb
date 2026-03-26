#ifndef MAGB_BUS_H
#define MAGB_BUS_H

#include "cpu.h"
#include "typedefs.h"

#include <iostream>
#include <array>

class Bus : public
     std::enable_shared_from_this<Bus> {
private:
// for testing on early development
    std::array<byte_t, 0xFFFF> mem;
public:
    SharpSM83 cpu;

public:
    Bus();
    ~Bus();

public:
    byte_t read(const address_t addr) noexcept;
    void   write(const address_t addr, const byte_t value) noexcept;
};

#endif // MAGB_BUS_H