#include "actor.h"


Actor::Actor(string actor_name, string birth_year, string death_year, vector<Movie*> movies) {
    this->actor_name = actor_name;
    this->birth_year = birth_year;
    this->death_year = death_year;
    this->movies = movies;
}

void Actor::addAdjacent(Actor* a) {
    acted_with.insert(a);
}

std::set<Actor*> Actor::getAdjacent() {
    return acted_with;
}

string Actor::getName() {
    return actor_name;
}

vector<Movie*> Actor::getMovies() {
    return movies;
}