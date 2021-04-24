# I Target Goal(Leading Question)

In this project, we will analyze official IMDB data about movies and their associated actors + ratings. [Here](https://www.imdb.com/interfaces/) is where we are getting this data. We will first investigate just how sparse/dense the graph is. We will do this through the interesting phenomenon of the [Six degrees of Kevin Bacon](https://en.wikipedia.org/wiki/Six_Degrees_of_Kevin_Bacon). Is it true, or just a myth? We will represent actors as nodes and draw an edge between two actors if they have starred in a movie together. Then, using BFS we can allow the user to query our system for any actor, and the BFS will search for the shortest path to Kevin Bacon. Then we can automate our work, having the system query thousands of actors and see which of them take 6 steps or more to reach Kevin Bacon.

# II Dataset Acquisition and Processing
How to acquire: We get our data in tsv.gz file format. First use 7zip or winzip to unzip, then for the tsv we convert to a txt file using notepad or excel. Specifically we used three datasets, one with all the ratings of the shows (title.ratings.tsv.gz), one with all of the actors in the IMDB database (name.basics.tsv.gz), and one with all the movies/shows in the IMDB database (title.basics.tsv.gz) 

# III Graph Algorithms
## a) graph traversal
BFS is used to query the data for proximity to Kevin Bacon.

## b)  ???




# IV Basic Timeline
4.10 - mid-project check-in: Acquire data and implement algorithms
4.23 - 4.30 - Work on fine tuning the algorithms and making UI more friendly
5.1 - 5.12 - finalizing project, write report, and prepare for the final presentation.
