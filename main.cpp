#include "movie.h"
#include "actor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <iterator>

using std::cout;
using std::map;

// taken from http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
template <class Container>
void split2(const std::string& str, Container& cont, char delim = '\t')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

// taken from https://www.tutorialspoint.com/parsing-a-comma-delimited-std-string-in-cplusplus
vector<string> split_commas(string s) {
    vector<string> result;
   stringstream s_stream(s); //create string stream from the string
   while(s_stream.good()) {
      string substr;
      getline(s_stream, substr, ','); //get first string delimited by comma
      result.push_back(substr);
   }
   return result;
}

int main() {
    map<string, Movie*> string_to_movie;
    ifstream read_titles_txt("./data/titles.txt");
    string line;

    // parse movies
    while (getline(read_titles_txt, line)) {
        // split the text
        std::vector<std::string> words;
        split2(line, words);
        if (words.size() < 9) {
            continue;
        }
        bool isAdult = words[4] == "1";
        Movie* m = new Movie(words[2], words[1], isAdult, words[5], words[6], words[7]);
        string_to_movie[words[0]] = m; // put entry in map
    }

    // parse ratings for movies that are already in the map
    ifstream read_ratings_txt("./data/ratings.txt");
    while (getline(read_ratings_txt, line)) {
        // split the text
        std::vector<std::string> words;
        split2(line, words);
        if (words.size() < 3) {
            continue;
        }
        // if this movie was not added
        if (string_to_movie.find(words[0]) == string_to_movie.end()) {
            continue;
        }
        Movie* m = string_to_movie[words[0]];
        m->addRatingInfo(words[1], words[2]);
    }

    map<string, Actor*> actors;

    // parse actors
    ifstream read_actors_txt("./data/actors.txt");
    while (getline(read_actors_txt, line)) {
        // split the text
        std::vector<std::string> words;
        split2(line, words);
        if (words.size() < 5) {
            continue;
        }
        vector<string> movies_list = split_commas(words[4]);

        vector<Movie*> notable_movies;
        for (auto x : movies_list) {
            // if movie is not in list don't bother adding
            if (string_to_movie.find(x) == string_to_movie.end()) {
                continue;
            }
            notable_movies.push_back(string_to_movie[x]);
        }
        Actor* a = new Actor(words[1], words[2], words[3], notable_movies);
        actors[words[0]] = a;
    }

}