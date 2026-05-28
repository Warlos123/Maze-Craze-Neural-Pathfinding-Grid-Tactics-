#pragma once
#include "../ConstantsGUI.hpp"
#include "../GameState.hpp"

enum class GameState;
struct GameContext;

//Renders and drives the active match. Mutates ctx as needed (mainly when match ends).
GameState runGame(sf::RenderWindow& window, sf::Sprite& swSprite,
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos,
    const sf::Event* event, GameContext& ctx);
