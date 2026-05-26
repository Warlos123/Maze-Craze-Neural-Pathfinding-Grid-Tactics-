#pragma once
#include <vector>
#include <algorithm>
#include "../Constants.hpp"


class Player{
private:
    int nodeIndex_;
    std::vector<PowerUpType> powerUps_;

public:
    Player(int node);

    int getNodeIndex();
    void setNodeIndex(int node);

    void addPowerUp(PowerUpType powerUp);
    bool removePowerUp(PowerUpType powerUp);
    const std::vector<PowerUpType>& getPowerUps();
};