// AI.hpp
#pragma once
#include <random>
#include "../Game/Game.hpp"
#include "../Path/Path.hpp"

class AI {
private:
    std::mt19937 random_;

public:
    AI();
    void takeTurn(Game& game, Path& path);
};