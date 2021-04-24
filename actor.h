#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "movie.h"

using namespace std;
using std::string;

class Actor {
public:
    Actor(std::string actor_name, string birth_year, string death_year, vector<Movie*> movies);

private:
    string actor_name;
    string birth_year;
    string death_year;
    vector<string> genres;
    vector<Movie*> movies;
};