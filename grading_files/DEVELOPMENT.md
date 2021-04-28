Week of (4/18 to 4/24):

We implemented file IO using getline and stringstream for parsing certain patterns (for example commas and tabs which was the way the data was laid out in the txt file).
We created the major node classes for this project- instead of having one generic Node class, we made two- Movie, and Actor. They each have their own associated data
and connections. We made it so that Actor includes the Movie file, but the Movie does not know which actors act in the movie. This is consistent with the way the data was laid
out to us in the IMDB data, as each actor has his notable movies listed but the movies themselves do not list the actors.

Next, we connected the edges by using data structures like sets, maps and vectors to make the process very straightforward. Our first graph will be an undirected, unweighted
graph where each node is an Actor an edge is drawn if two actors have acted together in the past. Then, we implemented BFS to meet the first deliverable of our project-
proximity to Kevin Bacon. The BFS works by starting at the node which the user specifies through user input in the terminal. Then it will go out as BFS does until it has
reached everything. Also, it makes sure to mark which node visited it so that we can trace our steps back. Finally, we just find Kevin Bacon and trace the path back to the
root node in order to find the relevant information.

The next steps will be to implement our next algorithms which are going to involve the rating system somehow.

Week of (4/25 to 5/1):

We implemented a fully working version of Dijkstra's algorithm, meeting algorithm 2 of our project proposal.

Implemented Kosaraju's SCC algorithm by using 2 DFS and a custom class for testing

Wrote lots of tests to ensure the algorithms are working properly, adding the corresponding txt to the tests folder.

Clean up overall command line text flow 
