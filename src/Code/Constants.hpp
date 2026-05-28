#pragma once

//GRID
constexpr int ROWS = 10;
constexpr int COLS = 20;

//MAZE
constexpr int EXTRA_EDGE_PROB = 5; //Out of 100
constexpr int POWER_SPAWN_RATE = 5; //Out of 100
constexpr int PORTAL_SPAWN_RATE = 5; //Out of 100


//POWER_UPS
constexpr int POWER_SPAWN_RATE = 8; //Out of 100, per cell
constexpr int POWER_UP_TYPE_COUNT = 4; //JUMP_WALL, DOUBLE_PLAY, CONTROL_ENEMY, CHANGE_LOCATION

enum class PowerUpType{
    NONE,
    JUMP_WALL, 
    DOUBLE_PLAY,
    CONTROL_ENEMY,
    CHANGE_LOCATION
};

//PORTALS
constexpr int PORTAL_PAIRS = 2; //Fixed number of portal pairs per map
