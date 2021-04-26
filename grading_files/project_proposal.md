# I Target Goal(Leading Question)

In this project, we will analyze official IMDB data about movies and their associated actors + ratings. [Here](https://www.imdb.com/interfaces/) is where we are getting this data. We will first investigate just how sparse/dense the graph is. We will do this through the interesting phenomenon of the [Six degrees of Kevin Bacon](https://en.wikipedia.org/wiki/Six_Degrees_of_Kevin_Bacon). Is it true, or just a myth? We will represent actors as nodes and draw an edge between two actors if they have starred in a movie together. Then, using BFS we can allow the user to query our system for any actor, and the BFS will search for the shortest path to Kevin Bacon. Then we can automate our work, having the system query thousands of actors and see which of them take 6 steps or more to reach Kevin Bacon.

# II Dataset Acquisition and Processing
How to acquire: We get our data in tsv.gz file format. First use 7zip or winzip to unzip, then for the tsv we convert to a txt file using notepad or excel. Specifically we used three datasets, one with all the ratings of the shows (title.ratings.tsv.gz), one with all of the actors in the IMDB database (name.basics.tsv.gz), and one with all the movies/shows in the IMDB database (title.basics.tsv.gz) 

# III Graph Algorithms
## a) graph traversal
BFS is used to query the data for proximity to Kevin Bacon. When the user wants to find the path to him, we call a BFS orginating from that node. Then we will trace the path
that the nodes appear until the end, and then retrace them for the full path.

## b) Algorithm covered in class: Dijkstra's Algorithm
We will create a weighted, undirected graph where actors A and B have a connected edge with weight equal to 10* (10 - the rating of their best movie together). 
The reason the formula is like this is because at first ratings are from 0-10, and since Dijkstra is a minimal distance algorithm we need to flip it. Then its 10* since
the ratings are doubles with 1 digit precision and we want them to be integer. What ends up happening is that highly rated movies have lower edge weights, and worse rated movies have higher weights. Actors that have not acted
together before will have no edge between them. Then we will use Dijkstra's algorithm to create a "recommendation" system for the user. They can query for two
actors, call them A and B. We will run Dijkstra's algorithm onto A, finding the shortest path to B. What this represents in this graph is the least amount of movies and
best rated movies. So we can capture the essence of what the user likes about these two actors by recommending similar actors whom they've worked with.

## c) Algorithm not covered in class: Kosaraju's algorithm for strongly connected components
Are actors and directors influenced by one another? We will seek to find the answer to this question using Kosaraju's Algorithm. In order to do this we must first create the graph. This graph will be directed, but unweighted. We will draw edges as follows: for an actor A, we will go through all their films. For every film that isn't their last, we will find the director of that film and draw a directed edge from the director to that actor. And a very similar process happens for the actors. 
The reason why we chose to have it be drawn if and only if the film they worked together in wasn't the last film is because the whole idea of "influence" is that you work together
with a person and then you go out and make something else with your past work in mind. So if it was their last film, then none of their other work really was influenced.
We should be left with a directed graph, and we will then seek to find the strongly connected components of this graph. 
This experiment will hopefully help us answer a few questions: How many actors know each other? Are these actors separated by language? 
Do they star in similar movie genres? Are they separated by generation/time, or does time transcend the acting world? Are modern actors influenced by old directors/actors? These are question we will look to answer based on the size + quantity of strongly connected components. How sparse/dense will the drawn graph be?




# IV Basic Timeline
4.10 - mid-project check-in: Acquire data and implement algorithms
4.23 - 4.30 - Work on fine tuning the algorithms and making UI more friendly
5.1 - 5.12 - finalizing project, write report, and prepare for the final presentation.
