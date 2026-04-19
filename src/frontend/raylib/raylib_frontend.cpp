#include "raylib_frontend.h"

RaylibFrontend::RaylibFrontend() {
    
}

RaylibFrontend::~RaylibFrontend() {

}

void RaylibFrontend::Main(int argc, char* argv[]) {
    if (argc == 2) {
        // load rom:
        
    }

    init();

    while (!WindowShouldClose()) {
        update();
    }

    CloseWindow();
}

void RaylibFrontend::init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "Magb");
}

void RaylibFrontend::update() {
    BeginDrawing();
    
    Color color;
    ClearBackground(WHITE);
    
    EndDrawing();
}

