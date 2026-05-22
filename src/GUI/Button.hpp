#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ConstantsGUI.hpp"

class Button{
    public: 
        
        sf::RectangleShape shape;
        sf::Text text;
        sf::Color normalColor;
        sf::Color hoverColor;
        bool isVisable = false;

        Button(const sf::Font& font ,const std::string& label,
            sf::Vector2f size, sf::Vector2f pos, sf::Color normal, sf::Color hover) : 
            shape(size), text(font, label, 24), normalColor(normal), hoverColor(hover){
                
                shape.setPosition(pos);
                shape.setFillColor(normal);
                shape.setOutlineThickness(2.f);
                text.setFillColor(Constants::TEXTCOLOR);
                text.setOutlineThickness(1.f);

                sf::FloatRect tb = text.getLocalBounds();
                text.setPosition({
                    pos.x + (size.x - tb.size.x) / 2.f -tb.position.x,
                    pos.y + (size.y - tb.size.y) / 2.f -tb.position.y
                 });

                
            }
        
            void update(sf::Vector2f mousePos){
                if(shape.getGlobalBounds().contains(sf::Vector2f(mousePos))){
                    shape.setFillColor(hoverColor);
                }else{
                    shape.setFillColor(normalColor);
                }
            }

            void draw(sf::RenderWindow& window){
                if(isVisable){
                window.draw(shape);
                window.draw(text);
                }                
            }

            bool isClicked(sf::Vector2f clickPos){
                if(!isVisable){
                    return false;
                }
                return shape.getGlobalBounds().contains(clickPos);
            }

            void setVisable(bool visable){
                isVisable = visable;
            }

            
};

