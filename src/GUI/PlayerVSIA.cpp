#include "PlayerVSIA.hpp"
#include "Button.hpp"
#include "Text.hpp"

GameState runPlvsAI(sf::RenderWindow& window, sf::Sprite& swSprite, 
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos){

        static Button goBack{Constants::font, "Main Screen", {250.f, 60.f}, {5,5}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};

        window.draw(swSprite);
        goBack.update(mousePos);
        goBack.draw(window);

        if(mouseClicked && goBack.isClicked(clickPos)){
            return GameState::MAINSCREEN;
        }
        

        return GameState::P1vsAI;
    }

   