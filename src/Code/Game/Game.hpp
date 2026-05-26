#pragma once
#include "../Graph/Graph.hpp"
#include "../MazeGenerator/MazeGenerator.hpp"
#include "../Path/Path.hpp"
#include "../Player/Player.hpp"
#include "../Constants.hpp"

enum class Algorithm{
    DFS,
    BFS
};

enum class PlayerTurn{
    P1, 
    P2
};

class Game{
private:
    Graph graph_;
    MazeGenerator generator_;
    Path path_;
    Player p1_;
    Player p2_;
    PlayerTurn currentPlayer_;
    int treasureNode_;
    bool doublePlay_;
    bool controlEnemy_;

public:
    Game();
    void init(Algorithm a);

    bool move(int targetNode);

    bool useJumpWall(int targetNode);

    bool checkVictory(Player* player);
};