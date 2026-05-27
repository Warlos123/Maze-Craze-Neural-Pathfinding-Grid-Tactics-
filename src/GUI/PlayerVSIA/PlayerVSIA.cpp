#include "PlayerVSIA.hpp"
#include "../Factories/Button.hpp"
#include "../Factories/Text.hpp"
#include "../Factories/Textbox.hpp"
#include "../ConstantsGUI.hpp"


GameState runPlvsAI(sf::RenderWindow& window, sf::Sprite& swSprite,
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos,
    const sf::Event* event, GameContext& ctx){

        static Button goBack{Constants::font, "Main Screen", {250.f, 60.f}, {5,5}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
        static Button startBtn{Constants::font, "Start", {200.f, 60.f}, {500.f, 500.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
        goBack.setVisible(true);
        startBtn.setVisible(true);

        static Text playerName{Constants::font, "Type your name!", 40, {450.f, 80.f}, sf::Color::White};

        //...................Size...........Position......limit.Color normal and hover............................font
        static Textbox name({250.f, 60.f}, {475.f, 400.f}, 17, Constants::TEXTBOXNORMAL, Constants::TEXTBOXHOVER, Constants::font);
        name.setVisible(true);

        window.draw(swSprite);
        name.update(mousePos);
        goBack.update(mousePos);
        startBtn.update(mousePos);
        goBack.draw(window);
        startBtn.draw(window);
        playerName.draw(window);
        name.draw(window);

        if(mouseClicked){
            if(goBack.isClicked(clickPos)) return GameState::MAINSCREEN;
            if(startBtn.isClicked(clickPos)){
                std::string n = name.getText();
                ctx.p1Name = n.empty() ? "Player 1" : n;
                ctx.p2Name = "AI";
                name.clearText();
                return GameState::PLAYING;
            }
            name.isClicked(clickPos);
        }

        if(event){
            name.handleInput(*event);
        }

        return GameState::P1vsAI;
    }
