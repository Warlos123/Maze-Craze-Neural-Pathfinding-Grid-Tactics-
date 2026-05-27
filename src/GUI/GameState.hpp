#pragma once
#include <string>
#include "../Code/Game/Game.hpp"

enum class GameState{

    MAINSCREEN,
    ALGO_SELECT,
    P1vsAI,
    P1vsP2,
    PLAYING

};

enum class GameMode{
    NONE,
    PvAI,
    PvP
};

//Shared state across screens. Holds what's been chosen so far.
struct GameContext{
    GameMode mode = GameMode::NONE;
    Algorithm algo = Algorithm::DFS;
    std::string p1Name;
    std::string p2Name; //empty in PvAI
};