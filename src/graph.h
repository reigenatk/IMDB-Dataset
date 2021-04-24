#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int sourceNode, destNode;
};

class Graph {
public:
    vector<vector<int> > adjacencyList;
    Graph(vector<Edge> edges, int N);
    // Function to print adjacency list representation of a graph
    void PrintGraph();

private:
    int N; // number of nodes
};