#include "PlayerVSIA.hpp"
#include "Button.hpp"
#include "Text.hpp"


GameState runPlvsAI(sf::RenderWindow& window, sf::Sprite& swSprite, 
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos){

        static Button goBack{Constants::font, "Main Screen", {250.f, 60.f}, {5,5}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
        goBack.setVisable(true);

        static Text playerName{Constants::font, "Type your name!", 40, {450.f, 80.f}, sf::Color::White};
        

        window.draw(swSprite);
        goBack.update(mousePos);
        goBack.draw(window);
        playerName.draw(window);

        if(mouseClicked && goBack.isClicked(clickPos)){
            return GameState::MAINSCREEN;
        }
        
        

        return GameState::P1vsAI;
    }

   