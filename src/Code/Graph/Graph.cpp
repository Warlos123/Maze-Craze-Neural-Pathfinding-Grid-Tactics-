#include "Graph.hpp"

Graph::Graph(int rows_, int cols_){
    this->rows_ = rows_;
    this->cols_ = cols_;
    this->cells.resize(rows_*cols_);
    this->adj.resize(rows_*cols_);

    for(int i = 0; i < rows_; i++){
        this->cells[i].row = i / cols_; 
        this->cells[i].col = i % cols_;
    }
    //Divide index / # of cols. To get row of index.
    //Module index % # of cols. To get col of index
    //This links the node of the graph with a position inside Cells, which is a matrix.
}


void Graph::addEdge(int a, int b){
    adj[a].push_back(b);
    adj[b].push_back(a);
}

//Calculates the (row, col) coords of the given nodeIndex, calculates its neighbors in the grid. Return a vector containing it's neighbor's index.
std::vector<int> Graph::getNeighbors(int nodeIndex){
    std::vector<int> neighbors;
    int row = cells[nodeIndex].row;
    int col = cells[nodeIndex].col;

    //UP
    if(row > 0) 
        neighbors.push_back((row - 1) * cols_ + col);

    //DOWN
    if(row < rows_ - 1)
        neighbors.push_back((row + 1) * cols_ + col);

    //LEFT
    if(col > 0)
        neighbors.push_back(row * cols_ + (col - 1));

    //RIGHT
    if(col < cols_ - 1)
        neighbors.push_back(row * cols_ + (col + 1));

    return neighbors;
}


bool Graph::hasEdge(int a, int b){
    for(int i : adj[a]){
        if(i == b) 
            return true;
    }

    return false;
}


int Graph::getRows(){
    return rows_;
}

int Graph::getCols(){
    return cols_;
}