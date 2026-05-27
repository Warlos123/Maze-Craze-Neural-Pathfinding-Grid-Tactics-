#include "Game.hpp"

Game::Game() : graph_(ROWS, COLS), path_(graph_), p1_(-1), p2_(-1), treasureNode_(-1), currentPlayer_(PlayerTurn::P1), doublePlay_(false), controlEnemy_(false){
    std::random_device rd;
    random_ = std::mt19937(rd());
}

void Game::init(Algorithm a){
    if(a == Algorithm::DFS){
        generator_.DFS(graph_);
    }

    else{
        generator_.BFS(graph_);
    }

    std::uniform_int_distribution<int> dist(0, ROWS - 1);

    int p1Row = dist(random_);
    int p2Row = dist(random_);

    p1_.setNodeIndex(p1Row * COLS); //First column
    p2_.setNodeIndex(p2Row * COLS + (COLS - 1)); //Last column

    std::vector<int> pathVector = path_.getShortestPath(p1_.getNodeIndex(), p2_.getNodeIndex());

    treasureNode_ = pathVector[pathVector.size() / 2];

    //Reserve player and treasure cells so no power-up or portal spawns on them
    std::unordered_set<int> reserved;
    reserved.insert(p1_.getNodeIndex());
    reserved.insert(p2_.getNodeIndex());
    reserved.insert(treasureNode_);

    populate(reserved);
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
        return true;
    }

    //Cell with CHANGE_LOCATION, uses inmediatly, changes location between the two players
    else if(cell.powerUp == PowerUpType::CHANGE_LOCATION){
        int temp = p1_.getNodeIndex();
        p1_.setNodeIndex(p2_.getNodeIndex());
        p2_.setNodeIndex(temp);
        cell.powerUp = PowerUpType::NONE;
    }


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


void Game::populate(const std::unordered_set<int>& reserved){
    int total = ROWS * COLS;

    //Power-ups: each non-reserved cell has POWER_SPAWN_RATE% chance
    //of receiving a power-up. Type chosen uniformly at random from the 4 types.
    std::uniform_int_distribution<int> rateDist(1, 100);
    std::uniform_int_distribution<int> typeDist(1, POWER_UP_TYPE_COUNT); //1..4 maps to JUMP_WALL..CHANGE_LOCATION

    for(int i = 0; i < total; i++){
        if(reserved.count(i)) 
            continue;

        if(rateDist(random_) <= POWER_SPAWN_RATE){
            int t = typeDist(random_);
            graph_.getCell(i).powerUp = static_cast<PowerUpType>(t); //1=JUMP_WALL,2=DOUBLE_PLAY,3=CONTROL_ENEMY,4=CHANGE_LOCATION
        }
    }

    //Portals: PORTAL_PAIRS fixed pairs. Each pair uses two random free cells.
    //A cell is "free" if it's not reserved, not already a portal, and has no power-up
    //(to keep the spawn logic clean; a single cell holds one thing).
    std::vector<int> freeCells;
    freeCells.reserve(total);
    for(int i = 0; i < total; i++){
        if(reserved.count(i)) continue;
        if(graph_.getCell(i).isPortal) continue;
        if(graph_.getCell(i).powerUp != PowerUpType::NONE) continue;
        freeCells.push_back(i);
    }

    for(int p = 0; p < PORTAL_PAIRS; p++){
        if(freeCells.size() < 2) 
            break; //Not enough free cells, give up silently

        std::uniform_int_distribution<int> pick(0, freeCells.size() - 1);

        int aIdx = pick(random_);
        int a = freeCells[aIdx];
        std::swap(freeCells[aIdx], freeCells.back());
        freeCells.pop_back();

        std::uniform_int_distribution<int> pick2(0, freeCells.size() - 1);
        int bIdx = pick2(random_);
        int b = freeCells[bIdx];
        std::swap(freeCells[bIdx], freeCells.back());
        freeCells.pop_back();

        Cell& ca = graph_.getCell(a);
        Cell& cb = graph_.getCell(b);
        ca.isPortal = true;
        ca.portalTarget = b;
        cb.isPortal = true;
        cb.portalTarget = a;
    }
}