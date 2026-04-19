#ifndef MAGB_RAYLIB_FRONTEND_H
#define MAGB_RAYLIB_FRONTEND_H

#include <raylib.h>

#include "frontend.h"

class RaylibFrontend : Frontend
{
private:
    void init () override;
    void update() override;

public:

    void Main(int argc, char* argv[]) override;

    RaylibFrontend();
    ~RaylibFrontend();
};

#endif // MAGB_RAYLIB_FRONTEND_H
       
