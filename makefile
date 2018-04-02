all: trim output bfs

trim: trim.o
	g++ -o trim trim.o

trim.o:	trim.cpp trim.h
	g++ -g -c trim.cpp

output: output.o
	g++ -o output output.o

output.o: output.cpp 
	g++ -g -c output.cpp

bfs: bfsNet.o network.o
	g++ -o bfs bfsNet.o network.o

bfsNet.o: bfsNet.cpp bfsNet.h timer.h
	g++ -g -c bfsNet.cpp

network.o: network.cpp network.h
	g++ -g -c network.cpp

clean:
	rm *.o trim output bfs comp*.*

