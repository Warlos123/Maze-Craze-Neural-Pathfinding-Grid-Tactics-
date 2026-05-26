#pragma once 
#include "ConstantsGUI.hpp"
#include "GameState.hpp"

enum class GameState;

GameState runPvP(sf::RenderWindow& window, sf::Sprite& swSprite, 
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos, const sf::Event* event);