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
    
    cout << "Parsing actors\n";
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
        return;
    }
    if (actor_name_to_id.count(requested_actor) == 0) {
        // actor not in map
        cout << "I'm sorry, but we don't have this actor in our database.\n";
        return;
    }
    string ID = actor_name_to_id[requested_actor]; // for example m0000001
    Actor* a = actors[ID];

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
        cout << "We could not find Kevin Bacon. Try increasing the number of movies + actors parsed\n";
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

map<pair<Actor*, Actor*>, int> FloydWarshall() {
    map<pair<Actor*, Actor*>, int> distance; // use this for floyd warshall
    int INF = 1000000; // arbitrary large number

    cout << "Starting floyd warshall...\n";
    // initialization- if same node then dist = 0, else set to INF
    for (auto i = actors.begin(); i != actors.end(); i++) {
        for (auto j = actors.begin(); j != actors.end(); j++) {
            if (i == j) {
                distance[{i->second, j->second}] = 0;
            }
            else {
                distance[{i->second, j->second}] = INF;
            }
        }
    }

    // now for the nodes with edges, set distance = 1
    for (auto i = actors.begin(); i != actors.end(); i++) {
        std::set<Actor*> adjacentActors = (i->second)->getAdjacent();
        for (auto x : adjacentActors) {
            distance[{i->second, x}] = 1;
        }
    }
    cout << "hi\n";
    // do the DP
    for (auto k = actors.begin(); k != actors.end(); k++) {
        for (auto i = actors.begin(); i != actors.end(); i++) {
            for (auto j = actors.begin(); j != actors.end(); j++) {
                distance[{i->second, j->second}] = min(distance[{i->second, j->second}], distance[{i->second, k->second}]+distance[{k->second, j->second}]);
            }
        }
    }
    cout << "Done with floyd warshall\n";
    return distance;
}

void printHelp() {
    cout << "Hello, welcome to the IMDB databank\n";
    cout << "Press 1 to start a Kevin Bacon Search using BFS\n";
    cout << "Press 2 for a broader search between two individuals using Floyd Warshall\n";
    cout << "Press 3 for analytics on the 'Six Degrees of Kevin Bacon' theory\n";
    cout << "Press 4 to terminate the program\n";
    cout << "Press 0 to see this help menu again\n";
}

string promptUserInput() {
    printHelp();
    string userInput;
    getline(cin, userInput);
    return userInput;
}

int main() {

    // pre user-input processing
    int numOfMoviesToRead = 3000000; // file is 7 million lines long 
    parseMovieNames(numOfMoviesToRead);

    parseMovieRatings();

    int numOfActorsToRead = 200; // file is 10 million lines long 
    parseActors(numOfActorsToRead);

    parseEdges();
    // map<pair<Actor*, Actor*>, int> distance = FloydWarshall();
    while (true) {
        string userInput = promptUserInput();
        if (userInput == "0") {
            printHelp();
        } else if (userInput == "1") {
            string requested_actor;
            cout << "Please enter in an actor and we will find the path to Kevin Bacon.\n";
            getline(cin, requested_actor);
            BFS_to_bacon(requested_actor);
        } else if (userInput == "2") {
            string actor1, actor2;
            cout << "Please enter in your first actor\n";
            getline(cin, actor1);
            cout << "Please enter in your second actor\n";
            getline(cin, actor2);
            Actor* a1 = getActorFromName(actor1);
            Actor* a2 = getActorFromName(actor2);
            cout << "The distance is " << distance[{a1, a2}] << " actors long" << '\n';
        } else if (userInput == "3") {
            int numActors = actors.size();
            int numActorsWithDistLessThanSix = 0;
            Actor* kevin = getActorFromName("Kevin Bacon");
            for (auto i = actors.begin(); i != actors.end(); i++) {
                if (distance[{kevin, i->second}] <= 6) {
                    numActorsWithDistLessThanSix++;
                }
            }
            double percent = numActorsWithDistLessThanSix / numActors;
            cout << numActorsWithDistLessThanSix << " out of " << numActors << " had a distance less than or equal to 6 to Kevin Bacon\n";
            cout << setprecision(2) << "That's " << percent << "%!\n";
        } else if (userInput == "4") {
            cout << "Thanks for using the IMDB databank. Have a nice day!\n";
            break;
        }
    }
    return 0;
}