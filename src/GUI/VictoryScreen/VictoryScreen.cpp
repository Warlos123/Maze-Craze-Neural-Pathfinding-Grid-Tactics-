#include "VictoryScreen.hpp"
#include "../Factories/Button.hpp"
#include "../Factories/Text.hpp"

GameState runVictory(sf::RenderWindow& window, sf::Sprite& swSprite,
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos,
    const sf::Event* event, GameContext& ctx){

    static Button mainBtn{Constants::font, "Main Screen", {250.f, 60.f}, {300.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    static Button playAgainBtn{Constants::font, "Play Again", {250.f, 60.f}, {650.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    mainBtn.setVisible(true);
    playAgainBtn.setVisible(true);

    window.draw(swSprite);

    Text title{Constants::font, "Victory!", 70, {0.f, 0.f}, sf::Color::White};
    title.centerOn({Constants::WINDOW_WIDTH / 2.f, 250.f});
    title.draw(window);

    Text winnerText{Constants::font, ctx.winnerName + " wins!", 40, {0.f, 0.f}, Constants::TEXTCOLOR};
    winnerText.centerOn({Constants::WINDOW_WIDTH / 2.f, 380.f});
    winnerText.draw(window);

    mainBtn.update(mousePos);
    mainBtn.draw(window);
    playAgainBtn.update(mousePos);
    playAgainBtn.draw(window);

    if(mouseClicked){
        if(mainBtn.isClicked(clickPos)){
            ctx.winnerName.clear();
            return GameState::MAINSCREEN;
        }
        if(playAgainBtn.isClicked(clickPos)){
            ctx.winnerName.clear();
            return GameState::PLAYING;
        }
    }

    return GameState::VICTORY;
}