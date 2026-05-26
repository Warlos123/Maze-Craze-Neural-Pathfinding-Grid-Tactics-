#pragma once
#include <vector>
#include "../Cell/Cell.hpp"

class Graph{
private:
    int rows_;
    int cols_;
    std::vector<Cell> cells;
    std::vector<std::vector<int>> adj; //graph[N]

public:
    Graph(int rows_, int cols_);

    void addEdge(int a, int b);

    std::vector<int> getNeighbors(int nodeIndex);

    bool hasEdge(int a, int b);

    int getRows();

    int getCols();

    const std::vector<int>& getAdj(int nodeIndex);
};