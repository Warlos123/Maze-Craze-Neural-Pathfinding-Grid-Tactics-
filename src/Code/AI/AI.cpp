#include "AI.hpp"

AI::AI() {
    std::random_device random;
    random_ = std::mt19937(random());
}

void AI::takeTurn(Game& game, Path& path) {
    if (game.getCurrentPlayer() != PlayerTurn::P2){
        return;
    }

    int nodeAI = game.getP2Node();
    int treasureNode = game.getTreasureNode_();

    std::vector<int> shortPath = path.getShortestPath(nodeAI, treasureNode);

    if (shortPath.size() < 2){
        return;
    }

    int nextNode = shortPath[1];

    if (!game.getGraph().hasEdge(nodeAI, nextNode)) {
        if (game.p2HasJumpWall()) {
            std::uniform_int_distribution<int> distance(0, 1);
            if (distance(random_) == 1) {
                game.useJumpWall(nextNode);
                return;
            }
        }
        return;
    }

    game.move(nextNode);
}