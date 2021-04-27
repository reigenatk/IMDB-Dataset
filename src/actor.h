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
    void setDirector(bool val);
    bool getIsDirector();
    Movie* getLastMovie();
    void calculateLastMovie();
    std::set<Actor*> getInfluenced();
    std::set<Actor*> getInfluencedBy();
    void addInfluence(Actor* person);
    void addInfluenceBy(Actor* person);
    bool is_number(const std::string& s);

private:
    string actor_name;
    string birth_year;
    string death_year;
    bool is_director;
    vector<string> genres;
    vector<Movie*> movies;
    Movie* last_movie;

    // these house the actor pointers, not the names or ids. For BFS, undirected unweighted
    std::set<Actor*> acted_with;
    // for Dijkstra, undirected, weighted
    map<Actor*, int> edges;
    // for Kosaraju, directed, unweighted
    // the outbound edges. A -> B if A influenced B
    std::set<Actor*> influenced;
    // the inbound edges. Need this for the graph reversal
    std::set<Actor*> influenced_by;
};
