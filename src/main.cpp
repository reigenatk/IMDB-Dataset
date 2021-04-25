#include "movie.h"
#include "actor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <iterator>
#include <iomanip>
#include <list>

using std::cout;
using std::map;
using std::list;

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

map<string, Movie*> string_to_movie;
map<string, Actor*> actors;
map<string, string> actor_name_to_id;
bool isFirstLine;

Actor* getActorFromName(string name) {
    return actors[actor_name_to_id[name]];
}

string find_movie_in_common(string actor_name_1, string actor_name_2) {
    Actor* actor1 = getActorFromName(actor_name_1);
    Actor* actor2 = getActorFromName(actor_name_2);

    set<string> common_movies;
    for (auto x : actor1->getMovies()) {
        common_movies.insert(x->getTitle());
    }
    for (auto x : actor2->getMovies()) {
        if (common_movies.find(x->getTitle()) != common_movies.end()) {
            // we have the matching movie
            return x->getTitle();
        }
    }
}

void parseMovieNames(int numOfMoviesToRead) {
    ifstream read_titles_txt("../data/titles.txt");
    // parse movies
    isFirstLine = true;

    string line;

    cout << "Parsing movies...\n";
    while (getline(read_titles_txt, line)) {
        // ignore first line
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        if (numOfMoviesToRead == 0) {
            break;
        }
        // split the text
        std::vector<std::string> words;
        split2(line, words);

        if (words.size() < 9) {
            continue;
        }
        bool isAdult = words[4] == "1";
        Movie* m = new Movie(words[2], words[1], isAdult, words[5], words[6], words[7]);
        string_to_movie[words[0]] = m; // put entry in map
        numOfMoviesToRead--;
    }
    cout << "Done parsing " << string_to_movie.size() << " movies" << '\n';
}

void parseMovieRatings() {
    // parse ratings for movies that are already in the map
    ifstream read_ratings_txt("../data/ratings.txt");
    isFirstLine = true;
    string line;

    cout << "Parsing movie ratings...\n";
    int numRatingsParsed = 0;
    while (getline(read_ratings_txt, line)) {
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
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
        numRatingsParsed++;
    }
    cout << "Done parsing " << numRatingsParsed << " ratings" << '\n';
}

void parseActors(int numOfActorsToRead) {
    // parse actors
    ifstream read_actors_txt("../data/actors.txt");
    isFirstLine = true;
    
    cout << "Parsing actors...\n";
    string line;
    while (getline(read_actors_txt, line)) {
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        if (numOfActorsToRead == 0) {
            break;
        }
        // split the text
        std::vector<std::string> words;
        split2(line, words);
        if (words.size() < 5) {
            continue;
        }
 
        vector<string> movies_list = split_commas(words[5]);

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
        actor_name_to_id[words[1]] = words[0]; // name to ID mapping
        for (auto x : movies_list) {
            // add the actor to the list of actors for all of their movies
            // if movie is not in list don't bother adding
            if (string_to_movie.count(x) == 0) {
                continue;
            }
            string_to_movie[x]->addActor(words[0]);
            // cout << "add " << words[0] << " to movie " << x << '\n';
        }
        numOfActorsToRead--;
    }
    cout << "Done parsing " << actors.size() << " actors" << '\n';
}

void parseEdges() {
    cout << "Creating edges...\n";
    // for each movie, initialize the edges between all of its actors
    // an edge in this graph represents when two actors have acted together
    for (auto x : string_to_movie) {
        vector<string> actors_in_movie = x.second->getActors();
        // cout << x.first << " ";
        // for (auto y : actors_in_movie) {
        //     cout << y << " ";
        // }
        // cout << '\n';
        for (int i = 0; i < actors_in_movie.size(); i++) {
            for (int j = i+1; j < actors_in_movie.size(); j++) {
                Actor* actor1 = actors[actors_in_movie[i]];
                Actor* actor2 = actors[actors_in_movie[j]];
                actor1->addAdjacent(actor2);
                actor2->addAdjacent(actor1);
            }
        }
    }
    cout << "Done parsing edges" << '\n';
}

void BFS_to_bacon(string requested_actor) {
    // find actor
    if (requested_actor == "Kevin Bacon") {
        cout << "I like bacon";
    }
    if (actor_name_to_id.count(requested_actor) == 0) {
        // actor not in map
        cout << "I'm sorry, but we don't have this actor in our database.\n";
    }
    Actor* a = getActorFromName(requested_actor);

    list<Actor*> queue;

    // map[actor] = either empty string if not yet seen, or the string name of the actor that found it during the BFS traversal
    map<Actor*, string> visited;

    visited[a] = "root";
    queue.push_back(a);
    while (!queue.empty()) {
        Actor* aa = queue.front();
        queue.pop_front();
        // cout << aa->getName() << '\n';
        std::set<Actor*> adj = aa->getAdjacent();
        for (auto i = adj.begin(); i != adj.end(); i++) {
            if (visited[*i] == "") {
                // if not yet visited
                // visit and push into queue
                visited[*i] = aa->getName();
                queue.push_back(*i);
            }
        }
    }

    if (visited[actors[actor_name_to_id["Kevin Bacon"]]] == "") {
        cout << "We could not find Kevin Bacon from this actor. Try increasing the number of movies + actors parsed\n";
    } else {
        string actor = "Kevin Bacon";
        vector<string> chain;
        while (true) {
            chain.push_back(actor);
            if (visited[getActorFromName(actor)] != "root") {
                actor = visited[getActorFromName(actor)];
            } else {
                break;
            }
        }

        std::reverse(chain.begin(), chain.end());
        for (int i = 0; i < chain.size() - 1; i++) {
            string common_movie = find_movie_in_common(chain[i], chain[i+1]);
            cout << chain[i] << " and " << chain[i+1] << " starred together in " << common_movie << '\n';
        }
        cout << "The chain is " << chain.size() << " actors long\n";
    }
}

map<Actor*, int> BFS_from_bacon() {
    cout << "Calculating distances from Kevin Bacon\n";
    list<Actor*> queue;

    Actor* kevin = getActorFromName("Kevin Bacon");
    // map[actor] = a number representing (distance from kevin bacon + 1) since map defaults to 0 it kinda messes things up
    map<Actor*, int> visited;
    visited[kevin] = 1;
    queue.push_back(kevin);
    while (!queue.empty()) {
        Actor* aa = queue.front();
        queue.pop_front();
        // cout << aa->getName() << '\n';
        std::set<Actor*> adj = aa->getAdjacent();
        for (auto i = adj.begin(); i != adj.end(); i++) {
            if (visited[*i] == 0) {
                // if not yet visited
                // visit and push into queue
                visited[*i] = visited[aa] + 1;
                queue.push_back(*i);
            }
        }
    }
    cout << "Done calculating distances from Kevin Bacon\n";
    return visited;
}



void printHelp() {
    cout << "Hello, welcome to the IMDB databank\n";
    cout << "Press 1 to start a Kevin Bacon Search using BFS\n";
    cout << "Press 2 for analytics on the 'Six Degrees of Kevin Bacon' theory\n";
    cout << "Press 3 for Dijkstra\n";
    cout << "Press 4 for Kosaraju\n";
    cout << "Press 5 to terminate the program\n";
}

string promptUserInput() {
    cout << '\n';
    printHelp();
    string userInput;
    getline(cin, userInput);
    return userInput;
}

// taken from https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int main() {

    // pre user-input processing
    string numOfMoviesToRead;
    while (!is_number(numOfMoviesToRead)) {
        cout << "How many movies would you like to parse? (example: 3000000. file is 7 million long total)\n";
        getline(cin, numOfMoviesToRead);
    }

    parseMovieNames(stoi(numOfMoviesToRead));

    parseMovieRatings();

    string numOfActorsToRead; // example: 200000, file is 10 million lines long 
    while (!is_number(numOfActorsToRead)) {
        cout << "How many actors would you like to parse? (example: 200000. file is 10 million long total)\n";
        getline(cin, numOfActorsToRead);
    }
    
    parseActors(stoi(numOfActorsToRead));

    parseEdges();

    // this map has all the shortest distances from each actor to kevin bacon
    map<Actor*, int> distance = BFS_from_bacon();
    while (true) {
        string userInput = promptUserInput();
        if (userInput == "1") {
            string requested_actor;
            cout << "Please enter in an actor and we will find the path to Kevin Bacon.\n";
            getline(cin, requested_actor);
            BFS_to_bacon(requested_actor);
        } else if (userInput == "2") {
            int numActors = actors.size();
            int numActorsWithDistLessThanSix = 0;
            Actor* kevin = getActorFromName("Kevin Bacon");
            int sumOfDistances = 0;
            for (auto i = actors.begin(); i != actors.end(); i++) {
                if (distance[i->second] != 0) {
                    sumOfDistances += (distance[i->second]-1);
                }
                if (i->second->getName() == "Kevin Bacon") {
                    // if kevin himself, continue. he doesn't count
                    continue;
                }
                // if == 0 it means it isn't reachable
                if (distance[i->second] <= 7 && distance[i->second] != 0) {
                    numActorsWithDistLessThanSix++;
                }
            }
            double percent = numActorsWithDistLessThanSix / (double) numActors;
            double average = sumOfDistances / (double) numActors;
            cout << numActorsWithDistLessThanSix << " out of " << numActors << " had a distance less than or equal to 6 to Kevin Bacon\n";
            cout << "That's " << percent*100.0 << "%!\n";
            cout << "The average distance was " << average << '\n';
            cout << "Try changing the number of actors + movies parsed to see if you get a higher or lower percentage!\n";
        } else if (userInput == "3") {
            
        } else if (userInput == "4") {
            
        }
        else if (userInput == "5") {
            cout << "Thanks for using the IMDB databank. Have a nice day!\n";
            break;
        }
        else {
            cout << "I don't understand what you typed. Please press 0 for help\n";
        }
    }
    return 0;
}