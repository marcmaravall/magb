#include "core/bus.h"
#include "core/cpu.h"

#include "frontend/raylib/raylib_frontend.h"

int main(int argc, char** argv) {
    // Bus bus;
    // bus.Init();

    RaylibFrontend frontend;

    frontend.Main(argc, argv);
    
    return 0;
}

