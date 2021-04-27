#include "actor.h"


Actor::Actor(string actor_name, string birth_year, string death_year, vector<Movie*> movies) {
    this->actor_name = actor_name;
    this->birth_year = birth_year;
    this->death_year = death_year;
    this->movies = movies;
    is_director = false;
    calculateLastMovie();
}

void Actor::addAdjacent(Actor* a) {
    acted_with.insert(a);
}

// taken from https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool Actor::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
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

void Actor::addEdge(Actor* a, int d) {
    if (edges[a] == 0) {
        edges[a] = d;
    }
    else {
        edges[a] = std::min(edges[a], d); // take min here because smaller = higher rating and we want highest ratings
    }
}

map<Actor*, int> Actor::getEdges() {
    return edges;
}

int Actor::getEdge(Actor* a) {
    return edges[a];
}

void Actor::setDirector(bool val) {
    is_director = val;
}
bool Actor::getIsDirector() {
    return is_director;
}

Movie* Actor::getLastMovie() {
    return last_movie;
}
void Actor::calculateLastMovie() {
    int newest_movie_age = 0;
    Movie* ret;
    for (auto x : movies) {
        if (!is_number(x->getYear())) {
            continue;
        }
        int year = std::stoi(x->getYear());
        if (year > newest_movie_age) {
            ret = x;
            newest_movie_age = year;
        }
    }
    last_movie = ret;
}

std::set<Actor*> Actor::getInfluenced() {
    return influenced;
}
std::set<Actor*> Actor::getInfluencedBy() {
    return influenced_by;
}
void Actor::addInfluence(Actor* person) {
    influenced.insert(person);
}

void Actor::addInfluenceBy(Actor* person) {
    influenced_by.insert(person);
}
