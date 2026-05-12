#include "Button.hpp"
#include "Text.hpp"
#include "ConstantsGUI.hpp"
#include "PlayerVSIA.cpp"
#include "Multiplayer.cpp"

enum class GameState{

    MAINSCREEN,
    P1vsAI,
    P1vsP2

};


class GUI{
  public: 
    sf::RenderWindow window{sf::VideoMode({Constants::WINDOW_WIDTH,Constants::WINDOW_HEIGHT}), "Main Screen"};
    GameState estado = GameState::MAINSCREEN;
    sf::Font font{"./Resource/CinzelDecorative-Regular.ttf"};
    sf::Texture backGround{"./Resource/MainScreen.jpg"};
    sf::Texture subWindow{"./Resource/blurredMainScreen.jpg"};

    sf::Sprite bgSprite{backGround};
    sf::Sprite swSprite{subWindow};

    Button playIA{font ,"Play vs AI", {250.f, 60.f}, {150.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Button play2P{font ,"Multiplayer", {250.f, 60.f}, {800.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Button goBack{font, "Main Screen", {250.f, 60.f}, {5,5}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};

    Text title{font, "Maze Craze", 60, {450.f, 80.f}, sf::Color::White};

    GUI(){title.centerOn({Constants::WINDOW_WIDTH / 2.f, 150.f});}

    int run(){
        while(window.isOpen()){
            while (const std::optional event = window.pollEvent()){
                if(event->is<sf::Event::Closed>()) window.close();
                
                if(const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()){
                    if(mouseClick->button == sf::Mouse::Button::Left){
                        sf::Vector2f pos{float(mouseClick->position.x), float(mouseClick->position.y)};
                        if(playIA.isClicked(pos)) estado = GameState::P1vsAI;
                        if(play2P.isClicked(pos)) estado = GameState::P1vsP2;
                        if(goBack.isClicked(pos)) estado = GameState::MAINSCREEN;
                    }

                    
                }
            }            
            window.clear();
            window.draw(bgSprite);
            sf::RectangleShape overlay(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 70));

            if(estado == GameState::MAINSCREEN){
                playIA.update(sf::Mouse::getPosition(window));
                playIA.draw(window);

                play2P.update(sf::Mouse::getPosition(window));
                play2P.draw(window);

                title.draw(window);
            }

            if(estado == GameState::P1vsAI){
                window.draw(swSprite);
                goBack.draw(window);   
            }

            if(estado == GameState::P1vsP2){
                window.draw(swSprite);
                goBack.draw(window);
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