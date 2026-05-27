#include "Multiplayer.hpp"
#include "../Factories/Button.hpp"
#include "../Factories/Text.hpp"
#include "../Factories/Textbox.hpp"


GameState runPvP(sf::RenderWindow& window, sf::Sprite& swSprite,
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos,
    const sf::Event* event, GameContext& ctx){

        static Button goBack{Constants::font, "Main Screen", {250.f, 60.f}, {5,5}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
        static Button startBtn{Constants::font, "Start", {500.f, 60.f}, {350.f, 700.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
        goBack.setVisible(true);
        startBtn.setVisible(true);

        static Text playerNameONE{Constants::font, "Type your name player 1!", 25, {100.f, 500.f}, sf::Color::White};
        static Text playerNameTWO{Constants::font, "Type your name player 2!", 25, {750.f, 500.f}, sf::Color::White};

        //...................Size...........Position......limit.Color normal and hover
        static Textbox nameP1({250.f, 60.f}, {100.f, 600.f}, 17, Constants::TEXTBOXNORMAL, Constants::TEXTBOXHOVER, Constants::font);
        static Textbox nameP2({250.f, 60.f}, {750.f, 600.f}, 17, Constants::TEXTBOXNORMAL, Constants::TEXTBOXHOVER, Constants::font);
        nameP1.setVisible(true);
        nameP2.setVisible(true);

        window.draw(swSprite);
        goBack.update(mousePos);
        startBtn.update(mousePos);
        goBack.draw(window);
        startBtn.draw(window);
        playerNameONE.draw(window);
        playerNameTWO.draw(window);
        nameP1.draw(window);
        nameP2.draw(window);

        if(mouseClicked){
            if(goBack.isClicked(clickPos)) return GameState::MAINSCREEN;
            if(startBtn.isClicked(clickPos)){
                std::string n1 = nameP1.getText();
                std::string n2 = nameP2.getText();
                ctx.p1Name = n1.empty() ? "Player 1" : n1;
                ctx.p2Name = n2.empty() ? "Player 2" : n2;
                nameP1.clearText();
                nameP2.clearText();
                return GameState::PLAYING;
            }
            nameP1.isClicked(clickPos);
            nameP2.isClicked(clickPos);
        }

        if(event){
            nameP1.handleInput(*event);
            nameP2.handleInput(*event);
        }

        return GameState::P1vsP2;
    }
