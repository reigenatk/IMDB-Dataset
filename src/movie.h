#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using std::string;

class Movie {
public:
    Movie(string title, string movie_type, bool is_adult, string startYear, string endYear, string runtime);
    void addRatingInfo(string rating, string num_votes);
    string getTitle();
    void addActor(string a);
    vector<string> getActors();
    string getYear();
    string getRating();
    string getNumOfRaters();

private:
    string rating;
    string num_votes;
    string title;
    string movie_type;
    bool is_adult;
    string startYear;
    string endYear;
    string runtime;
    vector<string> actors;

};