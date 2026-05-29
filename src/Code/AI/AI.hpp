#pragma once
#include <random>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp> 
#include "../Game/Game.hpp"
#include "../Path/Path.hpp"

class AI {
private:
    std::mt19937 random_;
    sf::Clock clock_;
    bool waiting_;

public:
    AI();
    void takeTurn(Game& game, Path& path);
};