I Target Goal(Leading Question)

In this project, we aim to learn to understand how the PageRank(PR) algorithm work using the web graph dataset rovided by google, after which we are going to 
expolre a smaller, possiblly manually processed datset to try to rank the most poplular TV shows/animes in the past year by using the PageRank algorithm.
If time allows, we would also like to make a dynamic graph of Most Popular TV Shows from the past decade in every single year. However, this might be very difficult so
we might end up not doing it.
Lead question(s): How does PageRank among websites work? How can we rank the popularity of TV shows and other modern day media using this algorithm?

II Dataset Acquisition and Processing
How to acquire: The initial data set will be aquired from the Stanford Large Dataset Collection, and, specifically, http://snap.stanford.edu/data/web-Google.html
The data set is in txt.gz format and will be downloaded, stored and maintained in the cs225 final project github directory of our team, as well as our local machines.
How to process: We will try to convert the file into readable txt first and use the readFile code provided by course staff or write some string splitting algorithm to 
process it. 

III Graph Algorithms
a) graph traversal
BFS/DFS will be used to traverse and (maybe) visualize the data.

b)  Floyd–Warshall algorithm
This algorithm helps determine the shortest path between 2 vertices in the graph and, in this case, websites. This will be helpful in implementing the PR algorithm.

c) PageRank Algorithm
We plan to try to implement at least a simple version of this algorithm, if not the full version. There are plenty of materials covering this algorithm online for us to 
draw reference from.

IV Basic Timeline
4.10 - mid-project check-in: Acquire data and implement PR algorithm. 
4.23 - 4.30 - Apply on web graphs of modern day media and try to get as much result as possible.
5.1 - 5.12 - finalizing project, write report, and prepare for the final presentation.
