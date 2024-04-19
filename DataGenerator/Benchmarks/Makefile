.PHONY: all clean

forkTest: forkTest.cpp
	g++ -o $@ $^

simpleTest: simpleTest.cpp
	g++ -o $@ $^

parallelSimpleTest: parallelSimpleTest.cpp
	g++ -o $@ $^ -fopenmp

all: forkTest simpleTest parallelSimpleTest

clean:
	rm -f forkTest simpleTest parallelSimpleTest