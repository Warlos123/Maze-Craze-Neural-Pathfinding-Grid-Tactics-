#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button{
    public: 
        
        sf::RectangleShape shape;
        sf::Text text;
        sf::Color normalColor;
        sf::Color hoverColor;

        Button(const sf::Font& font ,const std::string& label,
            sf::Vector2f size, sf::Vector2f pos, sf::Color normal, sf::Color hover) : 
            shape(size), text(font, label, 24), normalColor(normal), hoverColor(hover){
                
                shape.setPosition(pos);
                shape.setFillColor(normal);
                text.setFillColor(sf::Color::White);

                sf::FloatRect tb = text.getLocalBounds();
                text.setPosition({
                    pos.x + (size.x - tb.size.x) / 2.f -tb.position.x,
                    pos.y + (size.y - tb.size.y) / 2.f -tb.position.y
                 });
            }
        
            void update(sf::Vector2i mousePos){
                if(shape.getGlobalBounds().contains(sf::Vector2f(mousePos))){
                    shape.setFillColor(hoverColor);
                }else{
                    shape.setFillColor(normalColor);
                }
            }

            void draw(sf::RenderWindow& window){
                window.draw(shape);
                window.draw(text);
            }
};

