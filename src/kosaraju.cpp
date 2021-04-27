#include "kosaraju.h"

void Kosaraju::dfs1(int node) {
    // cout << "now at " << node << '\n';
    visited[node] = true;
    Actor* actor = reverse_mapping[node];
    std::set<Actor*> edges = actor->getInfluenced();
    // cout << "adj size" << edges.size() << '\n';
    for (auto i=edges.begin(); i != edges.end(); i++)
    {
        int indx = mapping[*i];
        // cout << "adjacent node to " << node << " is " << indx << '\n';
        if (!visited[indx]) dfs1(indx);
    }
    // cout << "pushing " << node << " into stack\n";
    S.push(node);
}

void Kosaraju::dfs2(int node) {
    // cout << node << " "; 
    component[node] = numComponents;
    all_components[numComponents].push_back(node);
    visited[node] = true;
    Actor* actor = reverse_mapping[node];
    std::set<Actor*> reverse_edges = actor->getInfluencedBy();
    for (auto i=reverse_edges.begin(); i != reverse_edges.end(); i++)
    {
        int indx = mapping[*i];
        if (!visited[indx]) dfs2(indx);
    }
}

Kosaraju::Kosaraju(map<string, Actor*> actors) {
    actors_ = actors;
    N = actors.size();
    component.resize(N);
    all_components.resize(N);
    visited.resize(N, false);

    int index = 0;
    for (auto x : actors_) {
        mapping[x.second] = index;
        reverse_mapping[index] = x.second;
        index++;
    }

    
    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            dfs1(i);
        }
    }

    for (int i=0;i<N;i++)
    {
        visited[i] = false;
    }

    numComponents = 0;
    cout << "S size: " << S.size() << '\n';
    vector<int> scc_sizes;
    int sccone = 0;
    while (!S.empty())
    {
        int v = S.top(); S.pop();
        if (!visited[v])
        {
            // printf("Component %d: ", numComponents);
            dfs2(v);
            // cout << "The size of SCC " << numComponents << " is " << all_components[numComponents].size() << '\n';
            scc_sizes.push_back(all_components[numComponents].size());
            if (all_components[numComponents].size() == 1) {
                sccone++;
            }
            numComponents++;
        }
    }
    std::sort(scc_sizes.begin(), scc_sizes.end());
    for (int i = 1; i < 10; i++) {
        cout << scc_sizes[scc_sizes.size() - i] << '\n';
    }
    cout << "number of 1 sized scc: " << sccone << '\n';

}