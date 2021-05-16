#define CATCH_CONFIG_MAIN 
#include "./catch.hpp"
#include "../src/alldata.h"
#include <algorithm>

TEST_CASE("TEST ALL") {

    // 6 movies 7 actors
    AllData test_instance(6, 7, "./testmovies.txt", "./testratings.txt", "./testactors.txt", "./testdirectors.txt");

    map<string, Actor*> parsed_actors = test_instance.getActors();
    map<string, string> parsed_name_to_ID = test_instance.getActorNameToID();
    map<Actor*, int> distances_to_bacon = test_instance.getDistance();
    map<string, Movie*> parsed_string_to_movie = test_instance.getStringToMovie();
    SECTION("Test Initialization") {
    

    // test right entries in parsed actors
    REQUIRE(parsed_actors.size() == 7);
    REQUIRE(parsed_actors["nm0000003"]->getName() == "Brigitte Bardot");
    REQUIRE(parsed_actors["nm0000001"]->getName() == "Fred Astaire");

    // test parsed name to ID
    REQUIRE(parsed_name_to_ID.size() == parsed_actors.size());
    REQUIRE(parsed_name_to_ID["Brigitte Bardot"] == "nm0000003");
    REQUIRE(parsed_name_to_ID["Ingmar Bergman"] == "nm0000005");

    // test parsed movies
    REQUIRE(parsed_string_to_movie.size() == 6);
    REQUIRE(parsed_string_to_movie["tt0000001"]->getTitle() == "Carmencita");
    REQUIRE(parsed_string_to_movie["tt0000003"]->getYear() == "1898");
    REQUIRE(parsed_string_to_movie["tt0000002"]->getTitle() == "Le clown et ses chiens");
    REQUIRE(parsed_string_to_movie["tt0000002"]->getYear() == "1896");

    // test that right people are parsed into the right movies
    vector<string> people_in_movie1 = parsed_string_to_movie["tt0000001"]->getPeople();
    REQUIRE(people_in_movie1.size() == 3);
    REQUIRE(count(people_in_movie1.begin(), people_in_movie1.end(), "nm0000001") == 1);
    REQUIRE(count(people_in_movie1.begin(), people_in_movie1.end(), "nm0000002") == 1);

    // test that actors have the right movies parsed into their objects
    vector<Movie*> kevinbacon_movies = test_instance.getActorFromName("Kevin Bacon")->getMovies();
    REQUIRE(kevinbacon_movies.size() == 1);
    REQUIRE(find(kevinbacon_movies.begin(), kevinbacon_movies.end(), parsed_string_to_movie["tt0000002"]) != kevinbacon_movies.end());

    // test that actors have the right movie listed as "last movie"
    Movie* laurensLastMovie = test_instance.getActorFromName("Lauren Bacall")->getLastMovie();
    REQUIRE(laurensLastMovie->getTitle() == "Pauvre Pierrot"); // not movie 1, but movie 3


    // test director accuracy
    bool isKevinADirector = test_instance.getActorFromName("Kevin Bacon")->getIsDirector();
    REQUIRE(isKevinADirector == false);

    bool isFredADirector = test_instance.getActorFromName("Fred Astaire")->getIsDirector();
    REQUIRE(isFredADirector == true);

    // test getActorFromName returns the same pointer
    REQUIRE(test_instance.getActorFromName("Kevin Bacon") == parsed_actors[parsed_name_to_ID["Kevin Bacon"]]);

    // test adjacencies
    std::set<Actor*> test_adjacent = test_instance.getActorFromName("Kevin Bacon")->getAdjacent();
    REQUIRE(test_adjacent.size() == 3);
    REQUIRE(test_adjacent.count(test_instance.getActorFromName("Brigitte Bardot")) == 1); // Kevin is adjacent to Bridget 

    map<Actor*, int> weighted_edges = test_instance.getActorFromName("Kevin Bacon")->getEdges();

    // check weighted edges used by Dijkstra
    REQUIRE(weighted_edges[test_instance.getActorFromName("Brigitte Bardot")] == (int)(10 * (10-6.0)));
    REQUIRE(weighted_edges.size() == 3);


    // check influenced + influenced by edges
    std::set<Actor*> fredInfluences = test_instance.getActorFromName("Fred Astaire")->getInfluenced();
    std::set<Actor*> laurenInfluencedBy = test_instance.getActorFromName("Lauren Bacall")->getInfluencedBy();

    for (auto x : fredInfluences) {
        cout << x->getName() << '\n';
    }
    REQUIRE(fredInfluences.size() == 3);
    REQUIRE(laurenInfluencedBy.size() == 1);

    REQUIRE(fredInfluences.count(test_instance.getActorFromName("Lauren Bacall")) == 1);
    REQUIRE(laurenInfluencedBy.count(test_instance.getActorFromName("Fred Astaire")) == 1);
    }
    
    SECTION("Test BFS") {
        // check kevin bacon distance (should be 1 more than real dist, 0 if not reachable)
        REQUIRE(distances_to_bacon[test_instance.getActorFromName("Brigitte Bardot")] == 2);
        REQUIRE(distances_to_bacon[test_instance.getActorFromName("Lauren Bacall")] == 3); // kevin->mike->lauren

        // test movie in common methods
        Movie* common_movie = test_instance.find_movie_in_common("Ingmar Bergman", "Lauren Bacall");
        REQUIRE(common_movie->getTitle() == "Pauvre Pierrot");

        // test best movie in common method
        Movie* best_common_movie = test_instance.find_best_movie_in_common("Brigitte Bardot", "Ingmar Bergman");
        REQUIRE(best_common_movie->getTitle() == "Un bon bock"); // movie 4 rated higher than 5
    }

    SECTION("Test Dijkstra") {
        // small test case. Lauren Bacall (Actor 2) to Kevin Bacon (Actor 4), there are 2 paths. Either 2->6->4 or 2->5->3->4
        // 2->6 shares movie 1, weight 43. 6->4 shares movie 2, weight 40
        // edge 2->5 shares movie 3, weight is 35
        // edge 5->3 shares movie 4, weight 27
        // 3->4 shares movie 2, edge 40.
        // first path is 83, second is 102
        // so it should be 2->6->4 (actor 6 is Mike Pants)
        Actor* kevin = test_instance.getActorFromName("Kevin Bacon");
        Actor* lauren = test_instance.getActorFromName("Lauren Bacall");
        vector<Actor*> sol = test_instance.dijkstra(lauren, kevin);
        REQUIRE(sol.size() == 3);
        REQUIRE(sol[0]->getName() == "Lauren Bacall");
        REQUIRE(sol[1]->getName() == "Mike Pants");
        REQUIRE(sol[2]->getName() == "Kevin Bacon");

    }

    SECTION("Test Kosaraju") {
        Kosaraju test(parsed_actors);
        vector<int> components =  test.getComponent();
        // for (auto x : components) {
        //     cout << x << '\n';
        // }
        vector<vector<int>> allcomponents = test.getAllComponents();
        REQUIRE(allcomponents[3].size() == 3); // should have one SCC of size 3 (1, 6, 7) whose index is 3 in the traversal, and then other 4 are in their own 
        REQUIRE(components[6] == components[5]);
        REQUIRE(components[3] != components[4]); // both individual SCCs
    }
}