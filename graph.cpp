#include "graph.h"


Graph::Graph(vector<Edge> edges, int N) {
    this->N = N;
    adjacencyList.resize(N);
    for (Edge edge : edges) {
        adjacencyList[edge.sourceNode].push_back(edge.destNode);
    }
}

void Graph::PrintGraph()
{
    for (int i = 0; i < N; i++)
    {
        // print the current vertex number
        cout << i << " ——> ";
 
        // print all neighboring vertices of a vertex `i`
        for (int v: adjacencyList[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
}