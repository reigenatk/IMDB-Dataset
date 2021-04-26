#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "movie.h"
#include <set>
#include <map>

using namespace std;
using std::string;
using std::map;

class Actor {
public:
    Actor(std::string actor_name, string birth_year, string death_year, vector<Movie*> movies);
    void addAdjacent(Actor* a);
    std::set<Actor*> getAdjacent();
    string getName();
    vector<Movie*> getMovies();
    map<Actor*, int> getEdges();
    void addEdge(Actor* a, int d);
    int getEdge(Actor* a);

private:
    string actor_name;
    string birth_year;
    string death_year;
    vector<string> genres;
    vector<Movie*> movies;
    // these house the actor pointers, not the names or ids
    std::set<Actor*> acted_with;
    map<Actor*, int> edges;
};