#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "movie.h"
#include <set>

using namespace std;
using std::string;

class Actor {
public:
    Actor(std::string actor_name, string birth_year, string death_year, vector<Movie*> movies);
    void addAdjacent(Actor* a);
    std::set<Actor*> getAdjacent();
    string getName();
    vector<Movie*> getMovies();

private:
    string actor_name;
    string birth_year;
    string death_year;
    vector<string> genres;
    vector<Movie*> movies;
    std::set<Actor*> acted_with;
};