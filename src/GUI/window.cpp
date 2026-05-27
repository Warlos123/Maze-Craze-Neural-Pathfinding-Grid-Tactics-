#include "Factories/Button.hpp"
#include "Factories/Text.hpp"
#include "ConstantsGUI.hpp"
#include "PlayerVSIA/PlayerVSIA.hpp"
#include "Multiplayer/Multiplayer.hpp"
#include "GameState.hpp"


class GUI{
  public: 
    sf::RenderWindow window{sf::VideoMode({Constants::WINDOW_WIDTH,Constants::WINDOW_HEIGHT}), "Main Screen"};
    GameState state = GameState::MAINSCREEN;
    
    
    

    sf::Sprite bgSprite{Constants::backGround};
    sf::Sprite swSprite{Constants::subWindow};

    Button playIA{Constants::font ,"Play vs AI", {250.f, 60.f}, {150.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Button play2P{Constants::font ,"Multiplayer", {250.f, 60.f}, {800.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Button goBack{Constants::font, "Main Screen", {250.f, 60.f}, {5,5}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Text title{Constants::font, "Maze Craze", 60, {450.f, 80.f}, sf::Color::White};

    GUI(){title.centerOn({Constants::WINDOW_WIDTH / 2.f, 150.f});}

    int run(){
        while(window.isOpen()){
            bool clicked = false;
            sf::Vector2f clickPos;
            std::optional<sf::Event> lastEvent;

            while (const  std::optional event = window.pollEvent()){
                lastEvent = event;
                if(event->is<sf::Event::Closed>()) window.close();
                
                if(const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()){
                    if(mouseClick->button == sf::Mouse::Button::Left){
                        clicked = true;
                        clickPos = window.mapPixelToCoords(mouseClick->position);
                        sf::Vector2f pos = window.mapPixelToCoords(mouseClick->position);
                        if(playIA.isClicked(pos)) state = GameState::P1vsAI;
                        if(play2P.isClicked(pos)) state = GameState::P1vsP2;
                        if(goBack.isClicked(pos)) state = GameState::MAINSCREEN;
                    }

                    
                }
            }            
            window.clear();
            window.draw(bgSprite);
            sf::RectangleShape overlay(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 70));

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if(state == GameState::MAINSCREEN){
                playIA.setVisible(true);
                playIA.update(mousePos);
                playIA.draw(window);

                play2P.setVisible(true);
                play2P.update(mousePos);
                play2P.draw(window);

                title.draw(window);
            }

            if(state == GameState::P1vsAI){
                play2P.setVisible(false);
                playIA.setVisible(false);
                const sf::Event* evPtr = lastEvent.has_value() ? &(*lastEvent) : nullptr;
                state = runPlvsAI(window, swSprite, mousePos, clicked, clickPos, evPtr);
            }

            if(state == GameState::P1vsP2){
                play2P.setVisible(false);
                playIA.setVisible(false);
                const sf::Event* evPtr = lastEvent.has_value() ? &(*lastEvent) : nullptr;
                state = runPvP(window,swSprite,mousePos,clicked,clickPos, evPtr);
            }
            
            
            window.display();
        }
        return 0;
    }
    
};

int main(){
    GUI app;
    return app.run();
}