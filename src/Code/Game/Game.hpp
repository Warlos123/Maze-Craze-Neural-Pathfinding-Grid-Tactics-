#pragma once
#include <random>
#include <unordered_set>
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
    std::mt19937 random_;

    //Spawns power-ups and portals on the map, skipping reserved cells
    void populate(const std::unordered_set<int>& reserved);

public:
    Game();
    void init(Algorithm a);

    bool move(int targetNode);

    bool useJumpWall(int targetNode);

    bool checkVictory(Player* player);

    //Read-only accessors for the UI
    PlayerTurn getCurrentPlayer() const { return currentPlayer_; }
    Player& getP1() { return p1_; }
    Player& getP2() { return p2_; }
    Graph& getGraph() { return graph_; }
    int getTreasureNode() const { return treasureNode_; }
};
