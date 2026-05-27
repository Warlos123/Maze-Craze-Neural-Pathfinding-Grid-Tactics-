#include "GameScreen.hpp"
#include "../Factories/Button.hpp"
#include "../Factories/Text.hpp"
#include "../../Code/Game/Game.hpp"
#include "../../Code/Constants.hpp"
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
    constexpr float WALL_THICK  = 3.f;

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

    sf::Color powerUpColor(PowerUpType t){
        switch(t){
            case PowerUpType::JUMP_WALL:      return sf::Color(80, 150, 240); //blue
            case PowerUpType::DOUBLE_PLAY:    return sf::Color(80, 200, 100); //green
            case PowerUpType::CONTROL_ENEMY:  return sf::Color(220, 80, 80);  //red
            case PowerUpType::CHANGE_LOCATION:return sf::Color(170, 90, 200); //purple
            default: return sf::Color::Transparent;
        }
    }

    char powerUpLetter(PowerUpType t){
        switch(t){
            case PowerUpType::JUMP_WALL:      return 'J';
            case PowerUpType::DOUBLE_PLAY:    return 'D';
            case PowerUpType::CONTROL_ENEMY:  return 'C';
            case PowerUpType::CHANGE_LOCATION:return 'X';
            default: return ' ';
        }
    }
}


GameState runGame(sf::RenderWindow& window, sf::Sprite& swSprite,
    sf::Vector2f mousePos, bool mouseClicked, sf::Vector2f clickPos,
    const sf::Event* event, GameContext& ctx){

    //Game lifecycle: built once on entry, reset on exit.
    static std::unique_ptr<Game> game;
    static bool initialized = false;
    static bool jumpMode = false; //when true, next cell click triggers useJumpWall

    if(!initialized){
        game = std::make_unique<Game>();
        game->init(ctx.algo);
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
    cellBg.setFillColor(sf::Color(245, 235, 200));
    cellBg.setOutlineThickness(1.f);
    cellBg.setOutlineColor(sf::Color(180, 165, 130));
    for(int i = 0; i < ROWS * COLS; i++){
        cellBg.setPosition(cellTopLeft(i));
        window.draw(cellBg);
    }

    //2) walls: for each cell, look RIGHT and DOWN neighbors; if NO edge, draw wall.
    sf::RectangleShape wall;
    wall.setFillColor(sf::Color::Black);
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

    //4) power-ups: circle + letter
    for(int i = 0; i < ROWS * COLS; i++){
        Cell& c = game->getGraph().getCell(i);
        if(c.powerUp == PowerUpType::NONE) continue;
        sf::Vector2f center = cellCenter(i);
        sf::CircleShape circ(CELL_SIZE * 0.30f);
        circ.setOrigin({circ.getRadius(), circ.getRadius()});
        circ.setPosition(center);
        circ.setFillColor(powerUpColor(c.powerUp));
        window.draw(circ);

        std::string letter(1, powerUpLetter(c.powerUp));
        Text t{Constants::font, letter, 18, {0.f, 0.f}, sf::Color::White};
        t.centerOn(center);
        t.draw(window);
    }

    //5) portals: yellow diamond
    for(int i = 0; i < ROWS * COLS; i++){
        Cell& c = game->getGraph().getCell(i);
        if(!c.isPortal) continue;
        sf::Vector2f center = cellCenter(i);
        sf::ConvexShape diamond;
        diamond.setPointCount(4);
        float r = CELL_SIZE * 0.30f;
        diamond.setPoint(0, {center.x, center.y - r});
        diamond.setPoint(1, {center.x + r, center.y});
        diamond.setPoint(2, {center.x, center.y + r});
        diamond.setPoint(3, {center.x - r, center.y});
        diamond.setFillColor(sf::Color(240, 220, 60));
        diamond.setOutlineThickness(2.f);
        diamond.setOutlineColor(sf::Color::Black);
        window.draw(diamond);
    }

    //6) treasure: gold disc with "T"
    {
        sf::Vector2f center = cellCenter(game->getTreasureNode());
        sf::CircleShape tr(CELL_SIZE * 0.35f);
        tr.setOrigin({tr.getRadius(), tr.getRadius()});
        tr.setPosition(center);
        tr.setFillColor(sf::Color(255, 200, 50));
        tr.setOutlineThickness(2.f);
        tr.setOutlineColor(sf::Color(120, 80, 0));
        window.draw(tr);

        Text label{Constants::font, "T", 20, {0.f, 0.f}, sf::Color::Black};
        label.centerOn(center);
        label.draw(window);
    }

    //7) players
    {
        sf::Vector2f p1c = cellCenter(game->getP1().getNodeIndex());
        sf::CircleShape p1d(CELL_SIZE * 0.32f);
        p1d.setOrigin({p1d.getRadius(), p1d.getRadius()});
        p1d.setPosition(p1c);
        p1d.setFillColor(sf::Color(50, 100, 220));
        p1d.setOutlineThickness(2.f);
        p1d.setOutlineColor(sf::Color::White);
        window.draw(p1d);

        sf::Vector2f p2c = cellCenter(game->getP2().getNodeIndex());
        sf::CircleShape p2d(CELL_SIZE * 0.32f);
        p2d.setOrigin({p2d.getRadius(), p2d.getRadius()});
        p2d.setPosition(p2c);
        p2d.setFillColor(sf::Color(220, 60, 60));
        p2d.setOutlineThickness(2.f);
        p2d.setOutlineColor(sf::Color::White);
        window.draw(p2d);
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
                if(game->checkVictory(&game->getP1()) || game->checkVictory(&game->getP2())){
                    //TODO: victory screen. For now just go back.
                    initialized = false;
                    return GameState::MAINSCREEN;
                }
            }
        }
    }

    return GameState::PLAYING;
}