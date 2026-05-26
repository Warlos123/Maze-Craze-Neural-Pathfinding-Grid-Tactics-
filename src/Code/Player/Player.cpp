#include "Player.hpp"

Player::Player(int node) : nodeIndex_(node){}

int Player::getNodeIndex(){
    return nodeIndex_;
}

void Player::setNodeIndex(int node){
    nodeIndex_ = node;
}

void Player::addPowerUp(PowerUpType powerUp){
    powerUps_.push_back(powerUp);
}

bool Player::removePowerUp(PowerUpType powerUp){
    auto it = std::find(powerUps_.begin(), powerUps_.end(), powerUp);
    if(it != powerUps_.end()){
        powerUps_.erase(it);
        return true;
    }

    return false;
}

const std::vector<PowerUpType>& Player::getPowerUps(){
    return powerUps_;
}