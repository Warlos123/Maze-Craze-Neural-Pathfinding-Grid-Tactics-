#include "AI.hpp"

AI::AI() {
    waiting_ = false;
    std::random_device random;
    random_ = std::mt19937(random());
}

void AI::takeTurn(Game& game, Path& path) {

    constexpr float AI_DELAY = 1.f;    

    if (game.getCurrentPlayer() != PlayerTurn::P2){
        waiting_ = false;
        return;
    }

    if (waiting_ == false){
        clock_.restart();
        waiting_ = true;
        return;
    }

    if (clock_.getElapsedTime().asSeconds() < AI_DELAY){
        return;
    }

    int nodeAI = game.getP2().getNodeIndex();
    int treasureNode = game.getTreasureNode();

    std::vector<int> shortPath = path.getShortestPath(nodeAI, treasureNode);

    if (shortPath.size() < 2){
        waiting_ = false;
        return;
    }

    int nextNode = shortPath[1];
    
    bool hasJumpWall = false;
    const std::vector<PowerUpType>& powerList = game.getP2().getPowerUps();

    for (int i = 0; i < powerList.size(); ++i) {
        if (powerList[i] == PowerUpType::JUMP_WALL) {
            hasJumpWall = true;
            break;
        }
    }

    if (!game.getGraph().hasEdge(nodeAI, nextNode)) {
        if (hasJumpWall) {
            std::uniform_int_distribution<int> distance(0, 1);
            if (distance(random_) == 1) {
                game.useJumpWall(nextNode);
                return;
            }
        }
        waiting_ = false;
        return;
    }

    game.move(nextNode);
    waiting_ = false;
}