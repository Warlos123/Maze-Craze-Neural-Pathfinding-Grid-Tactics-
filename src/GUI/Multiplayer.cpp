#include "Multiplayer.hpp"
#include "Button.hpp"
#include "Text.hpp"

GameState runPvP(sf::RenderWindow& window, sf::Sprite& swSprite, 
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos){

        static Button goBack{Constants::font, "Main Screen", {250.f, 60.f}, {5,5}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
        goBack.setVisable(true);

        static Text playerNameONE{Constants::font, "Type your name player 1!", 25, {100.f, 500.f}, sf::Color::White};
        static Text playerNameTWO{Constants::font, "Type your name player 2!", 25, {750.f, 500.f}, sf::Color::White};

        window.draw(swSprite);
        goBack.update(mousePos);
        goBack.draw(window);
        playerNameONE.draw(window);
        playerNameTWO.draw(window);

        if(mouseClicked && goBack.isClicked(clickPos)){
            return GameState::MAINSCREEN;
        }

        return GameState::P1vsP2;
    }

