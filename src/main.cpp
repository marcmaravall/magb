#include "core/bus.h"
#include "core/cpu.h"

int main(int argc, char** argv) {
    Bus bus;
    bus.Init();

    std::cout << "hello, world!\n";
    return 0;
}