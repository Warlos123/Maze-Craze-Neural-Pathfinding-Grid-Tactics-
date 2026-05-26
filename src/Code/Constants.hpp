#pragma once

//GRID
constexpr int ROWS = 10;
constexpr int COLS = 20;

//MAZE
constexpr int EXTRA_EDGE_PROB = 5; //Out of 100

//POWER_UPS
enum class PowerUpType{
    NONE,
    JUMP_WALL, 
    DOUBLE_PLAY,
    CONTROL_ENEMY,
    CHANGE_LOCATION
};

