#pragma once
#include "../ConstantsGUI.hpp"
#include "../GameState.hpp"

enum class GameState;
struct GameContext;

GameState runVictory(sf::RenderWindow& window, sf::Sprite& swSprite,
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos,
    const sf::Event* event, GameContext& ctx);