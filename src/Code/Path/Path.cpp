#include "Path.hpp"

Path::Path(Graph& graph) : graph_(graph){}

int Path::getShortestPath(int p1, int p2){
    int total = graph_.getRows() * graph_.getCols();
    std::vector<bool> visited(total, false);
    std::vector<int> parent(total, -1);
    std::queue<int> queue;

    //BFS
    queue.push(p1);
    visited[p1] = true;

    while (!queue.empty()){
        int current = queue.front();
        queue.pop();

        if (current == p2) 
            break;

        for (int i : graph_.getAdj(current)) {
            if (!visited[i]) {
                visited[i] = true;
                parent[i] = current;
                queue.push(i);
            }
        }
    }

    //path, from p2 to p1
    std::vector<int> path;
    for (int node = p2; node != -1; node = parent[node]){
        path.push_back(node);
    }

    //return middle node (treasure location)
    return path[path.size() / 2];
}
