#pragma once 
#include <SFML/Graphics.hpp>
#include <string>

class Text{
    public: 
        sf::Text text;
        
        Text(const sf::Font& font, const std::string& content,
             unsigned int size, sf::Vector2f pos, sf::Color color)
             : text (font, content, size){
                text.setFillColor(color);
                text.setPosition(pos);
             }

        void centerX(unsigned int windowWidth, float y){
            sf::FloatRect tb = text.getLocalBounds();
            text.setPosition({
                (windowWidth - tb.size.x) / 2.f - tb.position.x, y
            });
        }

        void draw(sf::RenderWindow& window){
            window.draw(text);
        }
};

