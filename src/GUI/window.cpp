#include "Factories/Button.hpp"
#include "Factories/Text.hpp"
#include "ConstantsGUI.hpp"
#include "PlayerVSIA/PlayerVSIA.hpp"
#include "Multiplayer/Multiplayer.hpp"
#include "GameScreen/GameScreen.hpp"
#include "GameState.hpp"


class GUI{
  public:
    sf::RenderWindow window{sf::VideoMode({Constants::WINDOW_WIDTH,Constants::WINDOW_HEIGHT}), "Maze Craze"};
    GameState state = GameState::MAINSCREEN;
    GameContext ctx;

    sf::Sprite bgSprite{Constants::backGround};
    sf::Sprite swSprite{Constants::subWindow};

    //Main screen
    Button playIA{Constants::font ,"Play vs AI",  {250.f, 60.f}, {150.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Button play2P{Constants::font ,"Multiplayer", {250.f, 60.f}, {800.f, 600.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Text title{Constants::font, "Maze Craze", 60, {450.f, 80.f}, sf::Color::White};

    //Algorithm selection screen
    Button dfsBtn{Constants::font, "Deep-Sector Probe", {350.f, 70.f}, {125.f, 400.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Button bfsBtn{Constants::font, "Broad-Wave Scan",   {350.f, 70.f}, {725.f, 400.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Button algoBack{Constants::font, "Main Screen", {250.f, 60.f}, {10.f, 10.f}, Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    Text algoTitle{Constants::font, "Choose Maze Generator", 40, {0.f, 0.f}, sf::Color::White};
    Text dfsHint{Constants::font, "Long, complex paths", 18, {0.f, 0.f}, sf::Color::White};
    Text bfsHint{Constants::font, "Highly interconnected", 18, {0.f, 0.f}, sf::Color::White};

    GUI(){
        title.centerOn({Constants::WINDOW_WIDTH / 2.f, 150.f});
        algoTitle.centerOn({Constants::WINDOW_WIDTH / 2.f, 200.f});
        dfsHint.centerOn({300.f, 500.f});
        bfsHint.centerOn({900.f, 500.f});
    }

    int run(){
        while(window.isOpen()){
            bool clicked = false;
            sf::Vector2f clickPos;
            std::optional<sf::Event> lastEvent;

            while (const std::optional event = window.pollEvent()){
                lastEvent = event;
                if(event->is<sf::Event::Closed>()) window.close();

                if(const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()){
                    if(mouseClick->button == sf::Mouse::Button::Left){
                        clicked = true;
                        clickPos = window.mapPixelToCoords(mouseClick->position);
                    }
                }
            }
            window.clear();
            window.draw(bgSprite);

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            const sf::Event* evPtr = lastEvent.has_value() ? &(*lastEvent) : nullptr;

            if(state == GameState::MAINSCREEN){
                playIA.setVisible(true);
                play2P.setVisible(true);
                playIA.update(mousePos);
                play2P.update(mousePos);
                playIA.draw(window);
                play2P.draw(window);
                title.draw(window);

                if(clicked){
                    if(playIA.isClicked(clickPos)){
                        ctx.mode = GameMode::PvAI;
                        state = GameState::ALGO_SELECT;
                    }
                    else if(play2P.isClicked(clickPos)){
                        ctx.mode = GameMode::PvP;
                        state = GameState::ALGO_SELECT;
                    }
                }
            }
            else if(state == GameState::ALGO_SELECT){
                playIA.setVisible(false);
                play2P.setVisible(false);

                window.draw(swSprite);
                algoBack.setVisible(true);
                dfsBtn.setVisible(true);
                bfsBtn.setVisible(true);
                algoBack.update(mousePos);
                dfsBtn.update(mousePos);
                bfsBtn.update(mousePos);
                algoBack.draw(window);
                dfsBtn.draw(window);
                bfsBtn.draw(window);
                algoTitle.draw(window);
                dfsHint.draw(window);
                bfsHint.draw(window);

                if(clicked){
                    if(algoBack.isClicked(clickPos)){
                        state = GameState::MAINSCREEN;
                    }
                    else if(dfsBtn.isClicked(clickPos)){
                        ctx.algo = Algorithm::DFS;
                        state = (ctx.mode == GameMode::PvAI) ? GameState::P1vsAI : GameState::P1vsP2;
                    }
                    else if(bfsBtn.isClicked(clickPos)){
                        ctx.algo = Algorithm::BFS;
                        state = (ctx.mode == GameMode::PvAI) ? GameState::P1vsAI : GameState::P1vsP2;
                    }
                }
            }
            else if(state == GameState::P1vsAI){
                playIA.setVisible(false);
                play2P.setVisible(false);
                state = runPlvsAI(window, swSprite, mousePos, clicked, clickPos, evPtr, ctx);
            }
            else if(state == GameState::P1vsP2){
                playIA.setVisible(false);
                play2P.setVisible(false);
                state = runPvP(window, swSprite, mousePos, clicked, clickPos, evPtr, ctx);
            }
            else if(state == GameState::PLAYING){
                state = runGame(window, swSprite, mousePos, clicked, clickPos, evPtr, ctx);
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
