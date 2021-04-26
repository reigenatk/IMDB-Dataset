#include "alldata.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <iterator>
#include <string>
#include <iomanip>
#include <list>
#include <stdio.h>
#include <limits.h>

using std::cout;
using std::map;
using std::list;

void printHelp() {
    cout << "Hello, welcome to the IMDB databank\n";
    cout << "Press 1 to start a Kevin Bacon Search using BFS\n";
    cout << "Press 2 for analytics on the 'Six Degrees of Kevin Bacon' theory\n";
    cout << "Press 3 for a recommendation\n";
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

    string numOfActorsToRead; // example: 200000, file is 10 million lines long 
    while (!is_number(numOfActorsToRead)) {
        cout << "How many actors would you like to parse? (example: 200000. file is 10 million long total)\n";
        getline(cin, numOfActorsToRead);
    }
    AllData instance(stoi(numOfMoviesToRead), stoi(numOfActorsToRead));
    map<string, Actor*> actors = instance.getActors();
    while (true) {
        string userInput = promptUserInput();
        if (userInput == "1") {
            string requested_actor;
            cout << "Please enter in an actor and we will find the path to Kevin Bacon.\n";
            while (true) {
                getline(cin, requested_actor);
                if (actors.find(requested_actor) != actors.end()) {
                    break;
                } else {
                    cout << "I'm sorry, we don't have this actor. Please try again\n";
                }
            }
            instance.BFS_to_bacon(requested_actor);
        } else if (userInput == "2") {
            
            int numActors = actors.size();
            int numActorsWithDistLessThanSix = 0;
            Actor* kevin = instance.getActorFromName("Kevin Bacon");
            map<Actor*, int> distance = instance.getDistance();
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
            

            cout << "Please enter two actors that you like:\n";
            string actor1;
            while (true) {
                getline(cin, actor1);
                if (actors.find(actor1) != actors.end()) {
                    break;
                } else {
                    cout << "I'm sorry, we don't have this actor. Please try again\n";
                }
            }
            
            cout << "Second actor:\n";
            string actor2;
            while (true) {
                getline(cin, actor2);
                if (actors.find(actor2) != actors.end()) {
                    break;
                } else {
                    cout << "I'm sorry, we don't have this actor. Please try again\n";
                }
            }
            Actor* a1 = instance.getActorFromName(actor1);
            Actor* a2 = instance.getActorFromName(actor2);
            vector<Actor*> shortest_path = instance.dijkstra(a1, a2);
            if (shortest_path.size() == 1 && shortest_path[0] == nullptr) {
                cout << "We could not find a path. Try another pair or increase number of actors/movies read in\n";
                continue;
            }
            double total_sum = 0;
            double total_votes = 0;
            for (int i = 0; i < shortest_path.size() - 1; i++) {
                Actor* a1 = shortest_path[i];
                Actor* a2 = shortest_path[i+1];
                Movie* common_movie = instance.find_best_movie_in_common(a1->getName(), a2->getName());
                total_sum += (double) std::stod(common_movie->getNumOfRaters()) * std::stod(common_movie->getRating());
                total_votes += (double) std::stod(common_movie->getNumOfRaters());
                cout << common_movie->getTitle() << ", (" + common_movie->getYear() << ") rated " << common_movie->getRating() << " by " << common_movie->getNumOfRaters() << '\n';
            }
            double weighted_rating = total_sum / (double) total_votes;
            
            cout << "We recommend you watch " << shortest_path.size() << " movies with an weighted rating of " << weighted_rating << '\n';
            cout << "We think you will like these movies based on the two actors you listed!\n";
            
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