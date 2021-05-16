# THE IMDB DATASET

## Intro
For the final project of my CS225 course, I implemented three well known graph algorithms (BFS, Dijkstra, Kosaraju) and used them to analyze a large public dataset.
This is the result of that project

### Unzipping data

The data is not included because it is much too big for github (millions of lines long), but you can go here https://www.imdb.com/interfaces/ to download. Process is to unzip gz file (using 7zip or equivalent), then convert to txt. There are four files you need to download, one with all the ratings of the shows (title.ratings.tsv.gz), one with all of the actors in the IMDB database (name.basics.tsv.gz), one with all the movies/shows in the IMDB database (title.basics.tsv.gz), and one with all the director information: (title.crew.tsv.gz). These are the ratings.txt, actors.txt, movies.txt, and director.txt files respectively.

**Place all four txt files in a directory called data** (what the code will be expecting), or change paths listed in main.cpp to whatever suits you.

Also if you want to write your own data, make sure each line is following the format of the original data. To see examples, go to the tests folder where 
I have supplied a test set of the four text files. Make sure that each piece of data is separated by a tab character, and a newline at the end.

### Running
Make sure you have GCC compiler installed. You can see the version of your GCC compiler by running 
```
g++ --version
```
Once opened go to the src directory and run 
```
g++ -std=c++11 main.cpp movie.cpp actor.cpp alldata.cpp kosaraju.cpp
```
to compile. 

Then run 

```
./a.exe 
```

for tests go to the tests directory, run 
```
g++ -std=c++11 tests.cpp ../src/alldata.cpp ../src/movie.cpp ../src/actor.cpp ../src/kosaraju.cpp"
```
Again run the executable once it shows up



