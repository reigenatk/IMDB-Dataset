#include "movie.h"

Movie::Movie(string title, string movie_type, bool is_adult, string startYear, string endYear, string runtime) {
    this->title = title;
    this->movie_type = movie_type;
    this->is_adult = is_adult;
    this->startYear = startYear;
    this->endYear = endYear;
    this->runtime = runtime;
}

void Movie::addRatingInfo(string rating, string num_votes) {
    this->rating = rating;
    this->num_votes = num_votes;
}

void Movie::addPerson(string a) {
    people.push_back(a);
}

vector<string> Movie::getPeople() {
    return people;
}

string Movie::getTitle() {
    return title;
}

string Movie::getYear() {
    return startYear;
}

string Movie::getRating() {
    return rating;
}

string Movie::getNumOfRaters() {
    return num_votes;
}

void Movie::addDirector(string a) {
    directors.push_back(a);
    
}
vector<string> Movie::getDirectors() {
    return directors;
}
