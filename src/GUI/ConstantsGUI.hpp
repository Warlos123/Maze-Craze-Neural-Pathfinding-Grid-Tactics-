#pragma once
#include <SFML/Graphics.hpp>

namespace Constants{
    inline const unsigned WINDOW_WIDTH = 1200;
    inline const unsigned WINDOW_HEIGHT = 800;

    inline const sf::Color BUTTON_NOMRAL{184, 135, 70};
    inline const sf::Color BUTTON_HOVER{212, 161, 93}; 

    inline const sf::Color TEXTBOXNORMAL{209, 201, 201};
    inline const sf::Color TEXTBOXHOVER{176, 169, 169};

    inline const sf::Color TEXTCOLOR{246, 231, 193};

    inline sf::Font font{"./Resource/CinzelDecorative-Regular.ttf"};
    inline sf::Texture backGround{"./Resource/MainScreen.jpg"};
    inline sf::Texture subWindow{"./Resource/blurredMainScreen.jpg"};
    
    inline sf::Texture soldierTex{"./Resource/Soldier.png"};
    inline sf::Texture minotaurTex{"./Resource/Minotaur.png"};
    inline sf::Texture chestTex{"./Resource/Chest.png"};
    inline sf::Texture portalTex{"./Resource/Portal.png"};
    inline sf::Texture controllerTex{"./Resource/Controller.png"};
    inline sf::Texture bootsTex{"./Resource/Boots.png"};
    inline sf::Texture numberTwoTex{"./Resource/NumberTwo.png"};
    inline sf::Texture repeatTex{"./Resource/Repeat.png"};
    
}
    



