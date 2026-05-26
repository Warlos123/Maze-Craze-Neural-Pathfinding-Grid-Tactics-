#include "Game.hpp"

Game::Game() : graph_(ROWS, COLS), path_(graph_), p1_(-1), p2_(-1), treasureNode_(-1), currentPlayer_(PlayerTurn::P1), doublePlay_(false), controlEnemy_(false){}

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

    //if controlEnemy is true. The current player will move the other player.
    if(controlEnemy_){
        if(currentPlayer_ == PlayerTurn::P1){
        currentP = &p2_;
        }

        else{
            currentP = &p1_;
        }

        controlEnemy_ = false;
    }   

    //No controlEnemy case
    else{
        if(currentPlayer_ == PlayerTurn::P1){
        currentP = &p1_;
        }

        else{
            currentP = &p2_;
        }
    }
    

    if(!graph_.hasEdge(currentP->getNodeIndex(), targetNode)){
        return false; //Wall
    }


    currentP->setNodeIndex(targetNode);

    Cell& cell = graph_.getCell(targetNode);


    if(cell.isPortal){
        currentP->setNodeIndex(cell.portalTarget);
    }


    //Cell with JUMP_WALL, saves powerUP
    if(cell.powerUp == PowerUpType::JUMP_WALL){
        currentP->addPowerUp(cell.powerUp);
        cell.powerUp = PowerUpType::NONE;
    }

    //Cell with DOUBLE_PLAY, uses inmediatly
    else if(cell.powerUp == PowerUpType::DOUBLE_PLAY){
        doublePlay_ = true;
        cell.powerUp = PowerUpType::NONE;
    }


    //Cell with CONTROL_ENEMY, uses inmediatly
    else if(cell.powerUp == PowerUpType::CONTROL_ENEMY){
        controlEnemy_ = true;
        cell.powerUp = PowerUpType::NONE;
    }

    /*ADD Cell WITH SPECIAL POWER
    else if(cell.powerUp == PowerUpType::XXX){
       
        cell.powerUp = PowerUpType::NONE;
    }
    */

    //Verifies if player has DOUBLE_PLAY. If he does, the current player doesn't change.
    if(!doublePlay_){
        //Change turn
        if(currentPlayer_ == PlayerTurn::P1){
            currentPlayer_ = PlayerTurn::P2;
        }

        else{
            currentPlayer_ = PlayerTurn::P1;
        }
    }

    else{
        doublePlay_ = false;
    }

    return true;

}


bool Game::useJumpWall(int targetNode){
    Player* currentP;
    if(currentPlayer_ == PlayerTurn::P1){
        currentP = &p1_;
    }
    else{
        currentP = &p2_;
    }

    //No JUMP_WALL
    if(currentP->getPowerUps().empty()) 
        return false;

    std::vector<int> gridNeighbors = graph_.getNeighbors(currentP->getNodeIndex());
    auto it = std::find(gridNeighbors.begin(), gridNeighbors.end(), targetNode);
    if(it == gridNeighbors.end()) //targetNode its no a neighbor of current player's position.
        return false;


    //targetNode is a neighbor of current player's position, jump wall.
    currentP->setNodeIndex(targetNode);


    currentP->removePowerUp(PowerUpType::JUMP_WALL);

    //Change turn
    if(currentPlayer_ == PlayerTurn::P1){
        currentPlayer_ = PlayerTurn::P2;
    }

    else{
        currentPlayer_ = PlayerTurn::P1;
    }

    return true;

}


bool Game::checkVictory(Player* player){
    return player->getNodeIndex() == treasureNode_;
}