#include "alldata.h"


AllData::AllData(int numOfMovies, int numOfActors, string moviePath, string ratingPath, string actorPath, string directorPath) {
    numOfMoviesToRead = numOfMovies;
    numOfActorsToRead = numOfActors;
    numOfDirectorsToRead = numOfMoviesToRead;

    // change file paths here
    parseMovieNames(numOfMovies, moviePath);
    parseMovieRatings(ratingPath);
    parseActors(numOfActors, actorPath);
    parseDirectors(directorPath);
    parseEdges();

    distance = BFS_from_bacon();
}

map<string, Movie*> AllData::getStringToMovie() {
    return string_to_movie;
}
map<string, Actor*> AllData::getActors() {
    return actors;
}
map<string, string> AllData::getActorNameToID() {
    return actor_name_to_id;
}
map<Actor*, int> AllData::getDistance() {
    return distance;
 }
// taken from https://www.tutorialspoint.com/parsing-a-comma-delimited-std-string-in-cplusplus
vector<string> AllData::split_commas(string s) {
    vector<string> result;
   stringstream s_stream(s); //create string stream from the string
   while(s_stream.good()) {
      string substr;
      getline(s_stream, substr, ','); //get first string delimited by comma
      result.push_back(substr);
   }
   return result;
}

// taken from http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
template <class Container>
void AllData::split2(const std::string& str, Container& cont, char delim)
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

void AllData::parseMovieNames(int num, string path) {
    ifstream read_titles_txt(path);
    // parse movies
    bool isFirstLine = true;

    string line;

    cout << "Parsing movies...\n";
    while (getline(read_titles_txt, line)) {
        // ignore first line
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        if (this->numOfMoviesToRead == 0) {
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

void AllData::parseMovieRatings(string path) {
    // parse ratings for movies that are already in the map
    ifstream read_ratings_txt(path);
    bool isFirstLine = true;
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


void AllData::parseActors(int numOfActorsToRead, string path) {
    // parse actors
    ifstream read_actors_txt(path);
    bool isFirstLine = true;
    
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
            x.resize(9);
            // if movie is not in list don't bother adding
            if (string_to_movie.count(x) == 0) {
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
            string_to_movie[x]->addPerson(words[0]);
            // cout << "add " << words[0] << " to movie " << x << '\n';
        }
        numOfActorsToRead--;
    }
    cout << "Done parsing " << actors.size() << " actors" << '\n';
}

void AllData::parseDirectors(string path) {
    int tmp = numOfDirectorsToRead;

    ifstream read_titles_txt(path);
    // parse directors
    bool isFirstLine = true;

    string line;

    cout << "Parsing director information...\n";
    while (getline(read_titles_txt, line)) {
        // ignore first line
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }
        if (numOfDirectorsToRead == 0) {
            break;
        }
        // split the text
        std::vector<std::string> words;
        split2(line, words);

        if (words.size() < 3) {
            continue;
        }
        vector<string> directors_list = split_commas(words[1]);
        string movie = words[0];
        if (string_to_movie.count(words[0]) == 0) {
            continue; // if we don't have this movie, don't bother
        }
        Movie* m = string_to_movie[movie];

        for (auto x : directors_list) {
            if (actors.count(x) == 0) {
                // cout << "skip" << numOfDirectorsToRead << '\n';
                // if this person wasn't added, just skip
                continue;
            }

            m->addDirector(x);
            // cout << "new size of directors: " << m->getDirectors().size() << '\n';
            // mark x as a director
            // cout << "added director to movie\n";
            actors[x]->setDirector(true);
        }
        numOfDirectorsToRead--;
    }
    cout << "Done parsing " << tmp << " directors" << '\n';
}

void AllData::parseEdges() {
    cout << "Creating edges...\n";
    for (auto x : string_to_movie) {
        vector<string> actors_in_movie = x.second->getPeople();

        for (int i = 0; i < actors_in_movie.size(); i++) {
            for (int j = i+1; j < actors_in_movie.size(); j++) {
                Actor* actor1 = actors[actors_in_movie[i]];
                Actor* actor2 = actors[actors_in_movie[j]];
                // for each movie, initialize the edges between all of its actors
                // an edge in this graph represents when two actors have acted together
                // this is the unweighted, undirected graph for BFS
                actor1->addAdjacent(actor2);
                actor2->addAdjacent(actor1);

                // process the rating x (0.0-10.0) by doing (10-x) and then multiplying by 10
                // this is for the weighted, undirected graph for Dijkstra
                if (x.second->getRating() == "") {
                    continue; // if no rating continue
                }
                double rating_of_movie = std::stod(x.second->getRating());
                double inverse_rating = 10 - rating_of_movie;
                int times10 = (int) 10 * inverse_rating;

                actor1->addEdge(actor2, times10);
                actor2->addEdge(actor1, times10);

            }
        }
    }
    cout << "Done parsing unweighted & weighted edges" << '\n';
    
    // final graph for Kosaraju, directed, unweighted graph. Loop through each person and
    // find every movie except their last, if they are a actor then draw an edge from all directors in their previous movies to them
    // and if they are director, draw edge from all actors they have worked with to them

    for (auto x : actors) {
        Movie* last_movie = x.second->getLastMovie();
        // cout << "actor " << x.first << "'s last movie is " << last_movie->getTitle() << '\n';
        vector<Movie*> all_movies = x.second->getMovies(); 


        for (auto movie: all_movies) {
            if (movie == last_movie) {
                continue;
            }
            vector<string> people = movie->getPeople();
            vector<string> directors = movie->getDirectors();
            // cout << "num of people " << people.size() << '\n';
            // cout << "num of dir " << directors.size() << '\n';
            // check if person is actor or director
            if (x.second->getIsDirector() == true) {

                // person was director
                // get all actors and directors
                
                for (auto person : people) {
                    if (person == x.first) {
                        // don't add the person himself
                        continue;
                    }
                    Actor* a = actors[person];
                    a->addInfluence(x.second);
                    x.second->addInfluenceBy(a);
                }
            } else {
                // if person is actor
                // get all directors for this movie
                for (auto person : directors) {
                    if (person == x.first) {
                        // don't add the person himself
                        continue;
                    }
                    // cout << "found dir influencing actr\n";
                    Actor* a = actors[person];
                    a->addInfluence(x.second);
                    x.second->addInfluenceBy(a);
                }
            }
        }
    }

    // for (auto x : actors) {
    //     cout << "actor " << x.second->getName() << "influenced " << x.second->getInfluenced().size() << "people " << '\n';
    //     cout << "actor " << x.second->getName() << "was influenced by " << x.second->getInfluencedBy().size() << "people " << '\n';
    // }

    cout << "Done parsing directed edges" << '\n';
}


Actor* AllData::getActorFromName(string name) {
    return actors[actor_name_to_id[name]];
}



map<Actor*, int> AllData::BFS_from_bacon() {
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

void AllData::BFS_to_bacon(string requested_actor) {
    // find actor
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
            Movie* common_movie = find_movie_in_common(chain[i], chain[i+1]);
            string common_movie_str = common_movie->getTitle() + ", (" + common_movie->getYear() + ")";
            cout << chain[i] << " and " << chain[i+1] << " starred together in " << common_movie_str << '\n';
        }
        cout << "The chain is " << chain.size() << " actors long\n";
    }
}

Movie* AllData::find_movie_in_common(string actor_name_1, string actor_name_2) {
    Actor* actor1 = getActorFromName(actor_name_1);
    Actor* actor2 = getActorFromName(actor_name_2);

    set<string> common_movies;
    for (auto x : actor1->getMovies()) {
        common_movies.insert(x->getTitle());
    }
    for (auto x : actor2->getMovies()) {
        if (common_movies.find(x->getTitle()) != common_movies.end()) {
            // we have the matching movie
            return x;
        }
    }
    return nullptr;
}

Movie* AllData::find_best_movie_in_common(string actor_name_1, string actor_name_2) {
    Actor* actor1 = getActorFromName(actor_name_1);
    Actor* actor2 = getActorFromName(actor_name_2);

    vector<Movie*> actor1_movies;
    for (auto x : actor1->getMovies()) {
        actor1_movies.push_back(x);
    }
    vector<Movie*> actor2_movies;
    for (auto x : actor2->getMovies()) {
        actor2_movies.push_back(x);
    }
    int best_rating = 0;
    Movie* ret = nullptr;
    for (auto x : actor2_movies) {
        for (auto y : actor1_movies) {
            if (x == y && stoi(x->getRating()) > best_rating) {
                ret = x;
                best_rating = stoi(x->getRating());
            }
        }
    }
    if (ret == nullptr) {
        cout << "kms";
    }
    return ret;
}

int AllData::minDistance(vector<int> dist, vector<bool> isFinal) {
    int min_val = INT_MAX;
    int min_index;
    for (int i = 0; i < dist.size(); i++) {
        if (!isFinal[i] && dist[i] < min_val) {
            min_val = dist[i];
            min_index = i;
        }
    }
    return min_index;
}

vector<Actor*> AllData::dijkstra(Actor* src, Actor* dest) {
    int num_actors = actors.size();
    int index = 0;
    map<Actor*, int> mapping;
    map<int, Actor*> reverse_mapping;
    for (auto x : actors) {
        mapping[x.second] = index;
        reverse_mapping[index] = x.second;
        index++;
    }

    vector<int> dist(num_actors, INT_MAX);
    vector<bool> isFinal(num_actors, false);
    vector<int> parent(num_actors);

    cout << "src node: " << mapping[src] << '\n';
    cout << "dst node: " << mapping[dest] << '\n';
    dist[mapping[src]] = 0;
    parent[mapping[src]] = -1; // mark it as root node
    for (int i = 0; i < num_actors - 1; i++) {
        // find the lowest distance node
        int u = minDistance(dist, isFinal);
        isFinal[u] = true;
        map<Actor*, int> edges = reverse_mapping[u]->getEdges();
        for (int j = 0; j < num_actors; j++) {
            int edgelength = edges[reverse_mapping[j]];
            // cout << "edge between " << u << " and " << j << " is " << edgelength << '\n';
            if (edgelength == 0) {
                continue; // if dist = 0 then there is no edge between actor u and actor j
            }
            // otherwise there is an edge. See if it reduces the distance.
            if (!isFinal[j] && dist[u] + edgelength < dist[j]) {
                dist[j] = dist[u] + edgelength;
                // cout << "new dist of " << j << " is " << dist[j] << '\n';
                parent[j] = u;
            }
        }
    }
    vector<int> path;
    int cur = mapping[dest];
    int var = 300;
    cout << "distance: " << dist[cur] << '\n';
    cout << "path:\n";
    if (dist[cur] == INT_MAX) {
        // then we know we never reached the node, its not connected!
        vector<Actor*> ret;
        ret.push_back(nullptr);
        return ret;
    }
    while (true) {
        if (var > 0) {
            cout << cur << '\n';
            var--;
        }
        path.push_back(cur);
        int par = parent[cur];
        if (par == -1) {
            // we've reached root, break
            break;
        }
        cur = par;
    }
    std::reverse(path.begin(), path.end()); // this is the path from root to dest, but its still made of ints. Need to translate back into pointers
    vector<Actor*> pointer_path;
    for (auto &x : path) {
        pointer_path.push_back(reverse_mapping[x]);
    }
    // this is the shortest path starting with the src and ending on dest
    return pointer_path;
}

void AllData::kosaraju() {
    Kosaraju k(actors);
    double dec = k.getLargestSCCSize() / (double) actors.size();
    cout << "The biggest 'clump' of actors takes up " << dec * 100 << " percent of the acting world with a size of " << k.getLargestSCCSize() << "!\n";
}