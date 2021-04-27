#pragma once
#include "actor.h"
#include <stack>
#include <algorithm>

using namespace std;

class Kosaraju {
    public:
    Kosaraju(map<string, Actor*> actors);

    void dfs1(int node);
    void dfs2(int node);

    private:

    vector<bool> visited;
    vector<int> component;
    vector<vector<int>> all_components;
    map<string, Actor*> actors_;
    map<Actor*, int> mapping;
    map<int, Actor*> reverse_mapping;
    int N;
    int numComponents;
    std::stack<int> S;
};