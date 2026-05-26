#include "Game.hpp"

Game::Game() : graph_(ROWS, COLS), path_(graph_), p1_(-1), p2_(-1), treasureNode_(-1), currentPlayer_(PlayerTurn::P1){}

void Game::init(Algorithm a){
    if(a == Algorithm::DFS){
        generator_.DFS(graph_);
    }

    else{
        generator_.BFS(graph_);
    }

    std::random_device rd;
    std::mt19937 random(rd());
    std::uniform_int_distribution<int> dist(0, ROWS - 1);

    int p1Row = dist(random);
    int p2Row = dist(random);

    p1_.setNodeIndex(p1Row * COLS); //First column
    p2_.setNodeIndex(p2Row * COLS + (COLS - 1)); //Last column

    std::vector<int> pathVector = path_.getShortestPath(p1_.getNodeIndex(), p2_.getNodeIndex());

    treasureNode_ = pathVector[pathVector.size() / 2];
}


bool Game::move(int targetNode){
    Player* currentP;
    if(currentPlayer_ == PlayerTurn::P1){
        currentP = &p1_;
    }

    else{
        currentP = &p2_;
    }


    if(!graph_.hasEdge(currentP->getNodeIndex(), targetNode)){
        return false; //Wall
    }


    currentP->setNodeIndex(targetNode);

    Cell& cell = graph_.getCell(targetNode);


    if(cell.isPortal){
        currentP->setNodeIndex(cell.portalTarget);
    }


    if(cell.powerUp != PowerUpType::NONE){
        currentP->addPowerUp(cell.powerUp);
        cell.powerUp = PowerUpType::NONE;
    }



    //Change turn
    if(currentPlayer_ == PlayerTurn::P1){
        currentPlayer_ = PlayerTurn::P2;
    }

    else{
        currentPlayer_ = PlayerTurn::P1;
    }

    return true;

}