#ifndef MAGB_BUS_H
#define MAGB_BUS_H

#include "typedefs.h"

#include <iostream>
#include <array>

class Bus {
private:

public:
    Bus();
    ~Bus();

    byte_t read(const address_t addr) noexcept;
    void   write(const address_t addr, const byte_t value) noexcept;
};

#endif // MAGB_BUS_H