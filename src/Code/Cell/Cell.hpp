#pragma once
#include "../Constants.hpp"

struct Cell{
    int row;
    int col;

    PowerUpType powerUp;
    bool isPortal;
    int portalTarget;

    Cell() : row(0), col(0), powerUp(PowerUpType::NONE), isPortal(false), portalTarget(-1) {}
    
};
