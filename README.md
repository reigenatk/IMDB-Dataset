**IMDB Databank**

src contains all relevant code

run with "g++ -std=c++11 main.cpp movie.cpp actor.cpp alldata.cpp kosaraju.cpp" while in src directory

then run ./a.out or whatever executable shows up

for tests, run "g++ -std=c++11 tests.cpp ../src/alldata.cpp ../src/movie.cpp ../src/actor.cpp ../src/kosaraju.cpp" while in tests directory

proposal, dev log and results file in grading files directory

data is not included because it is much too big for github, but you can go here https://www.imdb.com/interfaces/ to download. Which ones specifically is listed in more detail inside our project proposal. Process is to unzip gz file, then convert to txt.
Place all three txt files in a directory called data (what the code will be expecting), or change paths listed in main.cpp to whatever suits you.
