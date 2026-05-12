#include "Button.hpp"
#include "Text.hpp"
#include "ConstantsGUI.hpp"

enum class GameState{

    MAINSCREEN
};


class GUI{
  public: 
    sf::RenderWindow window{sf::VideoMode({1200,900}), "Main Screen"};
    GameState estado = GameState::MAINSCREEN;
    sf::Font font{"./Groovy Volcano Demo.ttf"};
    sf::Texture backGround{"./696.jpg"};
    sf::Sprite bgSprite{backGround};

    Button playIA{font ,"Player vs IA", {200.f, 60.f}, {250.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Button play2P{font ,"Play vs Player", {200.f, 60.f}, {750.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};

    Text title{font, "Maze Craze", 60, {450.f, 80.f}, sf::Color::White};

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