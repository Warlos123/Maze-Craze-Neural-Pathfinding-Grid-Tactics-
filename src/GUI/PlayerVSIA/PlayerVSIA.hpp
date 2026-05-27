#include "../ConstantsGUI.hpp"
#include "../GameState.hpp"
#pragma once 

enum class GameState;

GameState runPlvsAI(sf::RenderWindow& window, sf::Sprite& swSprite, 
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos, const sf::Event* event);