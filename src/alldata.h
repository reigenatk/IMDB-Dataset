#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <iterator>
#include <iomanip>
#include <list>
#include <stdio.h>
#include <limits.h>

#include "movie.h"
#include "actor.h"
#include "kosaraju.h"
using namespace std;
using std::string;

class AllData {
public:
    AllData(int numOfMovies, int numOfActors);
    void parseMovieNames(int num, string path);
    void parseActors(int num, string path);
    void parseMovieRatings(string path);
    void parseEdges();
    void parseDirectors(string path);
    Actor* getActorFromName(string name);

    // BFS methods
    void BFS_to_bacon(string requested_actor);
    map<Actor*, int> BFS_from_bacon();

    // dijkstra
    vector<Actor*> dijkstra(Actor* src, Actor* dest);
    
    // kosaraju
    void kosaraju();

    map<string, Movie*> getStringToMovie();
    map<string, Actor*> getActors();
    map<string, string> getActorNameToID();
    map<Actor*, int> getDistance();

    Movie* find_movie_in_common(string actor_name_1, string actor_name_2);
    Movie* find_best_movie_in_common(string actor_name_1, string actor_name_2);

private:
    template <class Container>
    void split2(const std::string& str, Container& cont, char delim = '\t');
    
    int minDistance(vector<int> dist, vector<bool> isFinal);
    
    map<string, Movie*> string_to_movie;
    map<string, Actor*> actors;
    map<string, string> actor_name_to_id;
    int numOfMoviesToRead;
    int numOfActorsToRead;
    int numOfDirectorsToRead;

    // this map has all the shortest distances from each actor to kevin bacon
    map<Actor*, int> distance;

    // the first vector has the component that actor x belongs to
    // other vector is a list of all the components with their respective entries
    vector<int> component;
    vector<vector<int>> all_components;
    
    vector<string> split_commas(string s);
};
