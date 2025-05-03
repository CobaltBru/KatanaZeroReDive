#pragma once
#include "config.h"
#include "LineManager.h"


class PathFinder
{
public:
    PathFinder(const vector<FPOINT>& nodes, const vector<vector<Edge>>& graph) : nodes(nodes), graph(graph) {}
    int findClosestNode(const FPOINT& pos) const;
    vector<int> FindPath(int start, int goal) const;
    void SetPath(vector<int> InPath) { path = InPath; }
private:
    const vector<FPOINT>& nodes;
    const vector<vector<Edge>>& graph;
    vector<int> path;
    size_t           currentIdx = 0;
};

