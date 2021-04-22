#include "graph.h"

int main() {
    vector<Edge> edges;

    // directed edges
    Edge e1 = {0, 1};
    Edge e2 = {0, 3};
    Edge e3 = {1, 2};
    Edge e4 = {2, 1};
    edges.push_back(e1);
    edges.push_back(e2);
    edges.push_back(e3);
    edges.push_back(e4);

    int N = 4;
    Graph* g = new Graph(edges, N);

    g->PrintGraph();
}