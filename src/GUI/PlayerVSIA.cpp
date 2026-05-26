#include "PlayerVSIA.hpp"
#include "Button.hpp"
#include "Text.hpp"
#include "Textbox.hpp"
#include "ConstantsGUI.hpp"


GameState runPlvsAI(sf::RenderWindow& window, sf::Sprite& swSprite, 
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos, const sf::Event* event){

        static Button goBack{Constants::font, "Main Screen", {250.f, 60.f}, {5,5}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
        goBack.setVisible(true);

        //........................................................size..position
        static Text playerName{Constants::font, "Type your name!", 40, {450.f, 80.f}, sf::Color::White};
        
        //...................Size...........Position......limit.Color normal and hover............................font 
        static Textbox name({250.f, 60.f}, {500.f, 400.f}, 17, Constants::TEXTBOXNORMAL, Constants::TEXTBOXHOVER, Constants::font);
        name.setVisible(true);

        

        window.draw(swSprite);
        name.update(mousePos);
        goBack.update(mousePos);
        goBack.draw(window);
        playerName.draw(window);
        name.draw(window);

        if(mouseClicked){
            if(goBack.isClicked(clickPos)) return GameState::MAINSCREEN;
            name.isClicked(clickPos);
        }
        
        if(event){
            name.handleInput(*event);
        }

        return GameState::P1vsAI;
    }

   