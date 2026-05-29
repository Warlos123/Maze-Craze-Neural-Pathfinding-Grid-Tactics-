#include "GameScreen.hpp"
#include "../Factories/Button.hpp"
#include "../Factories/Text.hpp"
#include "../../Code/Game/Game.hpp"
#include "../../Code/Constants.hpp"
#include "../../Code/AI/AI.hpp"
#include <memory>
#include <string>

//Grid layout (pixels). 50px cells, 10 rows x 20 cols = 1000x500.
//Centered horizontally, top bar of 150px for turn info.
namespace{
    constexpr float CELL_SIZE   = 50.f;
    constexpr float GRID_X      = 100.f;
    constexpr float GRID_Y      = 150.f;
    constexpr float GRID_W      = CELL_SIZE * COLS; //1000
    constexpr float GRID_H      = CELL_SIZE * ROWS; //500
    constexpr float WALL_THICK  = 5.f;

    inline sf::Vector2f cellTopLeft(int nodeIndex){
        int row = nodeIndex / COLS;
        int col = nodeIndex % COLS;
        return {GRID_X + col * CELL_SIZE, GRID_Y + row * CELL_SIZE};
    }

    inline sf::Vector2f cellCenter(int nodeIndex){
        sf::Vector2f tl = cellTopLeft(nodeIndex);
        return {tl.x + CELL_SIZE / 2.f, tl.y + CELL_SIZE / 2.f};
    }

    //Returns node index of the cell under the given pixel position, or -1.
    int cellAtPixel(sf::Vector2f pos){
        if(pos.x < GRID_X || pos.x >= GRID_X + GRID_W) return -1;
        if(pos.y < GRID_Y || pos.y >= GRID_Y + GRID_H) return -1;
        int col = static_cast<int>((pos.x - GRID_X) / CELL_SIZE);
        int row = static_cast<int>((pos.y - GRID_Y) / CELL_SIZE);
        return row * COLS + col;
    }

    sf::Texture* powerUpTexture(PowerUpType t){
        switch(t){
            case PowerUpType::JUMP_WALL:       return &Constants::bootsTex;
            case PowerUpType::DOUBLE_PLAY:     return &Constants::numberTwoTex;
            case PowerUpType::CONTROL_ENEMY:   return &Constants::controllerTex;
            case PowerUpType::CHANGE_LOCATION: return &Constants::repeatTex;
            default: return nullptr;
        }
    }
}


GameState runGame(sf::RenderWindow& window, sf::Sprite& swSprite,
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos,
    const sf::Event* event, GameContext& ctx){

    //Game lifecycle: built once on entry, reset on exit.
    static std::unique_ptr<Game> game;
    static std::unique_ptr<Path> aiPath;
    static AI ai;
    static bool initialized = false;
    static bool jumpMode = false; //when true, next cell click triggers useJumpWall

    if(!initialized){
        game = std::make_unique<Game>();
        game->init(ctx.algo);
        aiPath = std::make_unique<Path>(game->getGraph());
        initialized = true;
        jumpMode = false;
    }

    //UI elements (static so they survive between calls)
    static Button goBack{Constants::font, "Main Screen", {220.f, 50.f}, {10.f, 10.f},
        Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    static Button jumpBtn{Constants::font, "Use Jump Wall", {220.f, 50.f}, {970.f, 700.f},
        Constants::BUTTON_NOMRAL, Constants::BUTTON_HOVER};
    goBack.setVisible(true);
    jumpBtn.setVisible(true);

    window.draw(swSprite);

    //Top bar: whose turn
    PlayerTurn turn = game->getCurrentPlayer();
    std::string turnLabel = "Turn: ";
    turnLabel += (turn == PlayerTurn::P1)
        ? (ctx.p1Name.empty() ? "P1" : ctx.p1Name)
        : (ctx.p2Name.empty() ? "P2" : ctx.p2Name);
    if(jumpMode) turnLabel += "  [JUMP WALL: click a neighbor]";

    Text turnText{Constants::font, turnLabel, 26, {0.f, 0.f}, sf::Color::White};
    turnText.centerOn({Constants::WINDOW_WIDTH / 2.f, 90.f});
    turnText.draw(window);

    //Power-up counter for current player
    Player& currentP = (turn == PlayerTurn::P1) ? game->getP1() : game->getP2();
    int jumps = 0;
    for(PowerUpType pu : currentP.getPowerUps()){
        if(pu == PowerUpType::JUMP_WALL) jumps++;
    }
    Text jumpCount{Constants::font, "Jump Walls: " + std::to_string(jumps), 20,
        {GRID_X, GRID_Y + GRID_H + 20.f}, sf::Color::White};
    jumpCount.draw(window);

    //---------- Grid render ----------
    //1) cell backgrounds
    sf::RectangleShape cellBg({CELL_SIZE, CELL_SIZE});
    cellBg.setFillColor(sf::Color(90, 90, 95));
    cellBg.setOutlineThickness(1.f);
    cellBg.setOutlineColor(sf::Color(60, 60, 65));
    for(int i = 0; i < ROWS * COLS; i++){
        cellBg.setPosition(cellTopLeft(i));
        window.draw(cellBg);
    }

    //2) walls: for each cell, look RIGHT and DOWN neighbors; if NO edge, draw wall.
    sf::RectangleShape wall;
    wall.setFillColor(sf::Color(30, 30, 35));
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            int i = row * COLS + col;
            sf::Vector2f tl = cellTopLeft(i);

            //wall to the right
            if(col < COLS - 1){
                int right = i + 1;
                if(!game->getGraph().hasEdge(i, right)){
                    wall.setSize({WALL_THICK, CELL_SIZE});
                    wall.setPosition({tl.x + CELL_SIZE - WALL_THICK / 2.f, tl.y});
                    window.draw(wall);
                }
            }
            //wall below
            if(row < ROWS - 1){
                int below = i + COLS;
                if(!game->getGraph().hasEdge(i, below)){
                    wall.setSize({CELL_SIZE, WALL_THICK});
                    wall.setPosition({tl.x, tl.y + CELL_SIZE - WALL_THICK / 2.f});
                    window.draw(wall);
                }
            }
        }
    }

    //3) outer border
    sf::RectangleShape border({GRID_W, GRID_H});
    border.setPosition({GRID_X, GRID_Y});
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(WALL_THICK);
    border.setOutlineColor(sf::Color::Black);
    window.draw(border);

    //4) power-ups: sprite
    for(int i = 0; i < ROWS * COLS; i++){
        Cell& c = game->getGraph().getCell(i);
        if(c.powerUp == PowerUpType::NONE) continue;

        sf::Texture* tex = powerUpTexture(c.powerUp);
        if(!tex) continue;

        sf::Vector2f center = cellCenter(i);
        sf::Sprite sprite(*tex);
        sf::Vector2u size = tex->getSize();

        float scale = (CELL_SIZE * 0.80f) / static_cast<float>(std::max(size.x, size.y));
        sprite.setScale({scale, scale});
        sprite.setOrigin({size.x / 2.f, size.y / 2.f});
        sprite.setPosition(center);
        window.draw(sprite);
    }

    //5) portals: sprite
    for(int i = 0; i < ROWS * COLS; i++){
        Cell& c = game->getGraph().getCell(i);
        if(!c.isPortal) continue;

        sf::Vector2f center = cellCenter(i);
        sf::Sprite sprite(Constants::portalTex);
        sf::Vector2u size = Constants::portalTex.getSize();

        float scale = (CELL_SIZE * 0.85f) / static_cast<float>(std::max(size.x, size.y));
        sprite.setScale({scale, scale});
        sprite.setOrigin({size.x / 2.f, size.y / 2.f});
        sprite.setPosition(center);
        window.draw(sprite);
    }

    //6) treasure: Chest
    {
        sf::Vector2f Chest = cellCenter(game->getTreasureNode());
        sf::Sprite chestSprite(Constants::chestTex);
        sf::Vector2u chestSize = Constants::chestTex.getSize();

        float chestScale = (CELL_SIZE * 0.90f) / static_cast<float>(std::max(chestSize.x, chestSize.y));
        chestSprite.setScale({chestScale, chestScale});

        chestSprite.setOrigin({chestSize.x / 2.f, chestSize.y / 2.f});
        chestSprite.setPosition(Chest);
        window.draw(chestSprite);
    }

    //7) players
    {
        //P1 - Soldier
        sf::Vector2f p1c = cellCenter(game->getP1().getNodeIndex());
        sf::Sprite p1Sprite(Constants::soldierTex);
        sf::Vector2u p1Size = Constants::soldierTex.getSize();

        float p1Scale = (CELL_SIZE * 1.f) / static_cast<float>(std::max(p1Size.x, p1Size.y));
        p1Sprite.setScale({p1Scale, p1Scale});

        p1Sprite.setOrigin({p1Size.x / 2.f, p1Size.y / 2.f});
        p1Sprite.setPosition(p1c);
        window.draw(p1Sprite);

        //P2 - Minotaur
        sf::Vector2f p2c = cellCenter(game->getP2().getNodeIndex());
        sf::Sprite p2Sprite(Constants::minotaurTex);
        sf::Vector2u p2Size = Constants::minotaurTex.getSize();

        float p2Scale = (CELL_SIZE * 0.85f) / static_cast<float>(std::max(p2Size.x, p2Size.y));
        p2Sprite.setScale({p2Scale, p2Scale});

        p2Sprite.setOrigin({p2Size.x / 2.f, p2Size.y / 2.f});
        p2Sprite.setPosition(p2c);
        window.draw(p2Sprite);
    }

    //---------- UI overlays ----------
    goBack.update(mousePos);
    goBack.draw(window);
    jumpBtn.update(mousePos);
    jumpBtn.draw(window);

    //---------- Input ----------
    if(mouseClicked){
        if(goBack.isClicked(clickPos)){
            initialized = false; //reset for next match
            return GameState::MAINSCREEN;
        }

        if(jumpBtn.isClicked(clickPos)){
            jumpMode = !jumpMode;
        }
        else{
            int target = cellAtPixel(clickPos);
            if(target >= 0){
                if(jumpMode){
                    if(game->useJumpWall(target)){
                        jumpMode = false;
                    }
                    //if invalid, stay in jump mode so the user can try another cell
                }
                else{
                    game->move(target);
                }

                //Victory check after any successful action
                if(game->checkVictory(&game->getP1())){
                    if(ctx.p1Name.empty()){
                        ctx.winnerName = "P1";
                    }
                    else{
                        ctx.winnerName = ctx.p1Name;
                    }
                    initialized = false;
                    return GameState::VICTORY;
                }
                if(game->checkVictory(&game->getP2())){
                    if(ctx.p2Name.empty()){
                        ctx.winnerName = "P2";
                    }
                    else{
                        ctx.winnerName = ctx.p2Name;
                    }
                    initialized = false;
                    return GameState::VICTORY;
                }
            }
        }
    }

    if(ctx.mode == GameMode::PvAI){
        ai.takeTurn(*game, *aiPath);

        if(game->checkVictory(&game->getP2())){
            if(ctx.p2Name.empty()){
                ctx.winnerName = "AI";
            } else {
                ctx.winnerName = ctx.p2Name;
            }
            initialized = false;
            return GameState::VICTORY;
        }
    }

    return GameState::PLAYING;
}