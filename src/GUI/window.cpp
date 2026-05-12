#include "Button.hpp"
#include "Text.hpp"
#include "ConstantsGUI.hpp"

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
    sf::Texture backGround{"./Resource/696.jpg"};
    sf::Sprite bgSprite{backGround};

    Button playIA{font ,"Play vs AI", {250.f, 60.f}, {250.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Button play2P{font ,"Multiplayer", {250.f, 60.f}, {750.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};

    Text title{font, "Maze Craze", 60, {450.f, 80.f}, sf::Color::White};

    GUI(){title.centerOn({Constants::WINDOW_WIDTH / 2.f, 150.f});}

    int run(){
        while(window.isOpen()){
            while (const std::optional event = window.pollEvent()){
                if(event->is<sf::Event::Closed>()) window.close();
            }            
            window.clear();
            window.draw(bgSprite);

            if(estado == GameState::MAINSCREEN){
                playIA.update(sf::Mouse::getPosition(window));
                playIA.draw(window);

                play2P.update(sf::Mouse::getPosition(window));
                play2P.draw(window);

                title.draw(window);
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