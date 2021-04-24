#include "actor.h"


Actor::Actor(string actor_name, string birth_year, string death_year, vector<Movie*> movies) {
    this->actor_name = actor_name;
    this->birth_year = birth_year;
    this->death_year = death_year;
    this->movies = movies;
}
