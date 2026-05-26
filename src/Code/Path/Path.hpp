#pragma once
#include <queue>
#include "../Graph/Graph.hpp"

class Path{
private:
    Graph& graph_;

public:
    Path(Graph& graph);
    std::vector<int> getShortestPath(int p1, int p2);
};