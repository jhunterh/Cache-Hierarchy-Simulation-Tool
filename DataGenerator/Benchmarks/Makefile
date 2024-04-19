.PHONY: all clean

forkTest: forkTest.cpp
	g++ -o $@ $^

simpleTest: simpleTest.cpp
	g++ -o $@ $^

parallelSimpleTest: parallelSimpleTest.cpp
	g++ -o $@ $^ -fopenmp

matrixMultSingleIneff: matrixMultSingleIneff.cpp
	g++ -o $@ $^

matrixMultSingle: matrixMultSingle.cpp
	g++ -o $@ $^

all: forkTest simpleTest parallelSimpleTest matrixMultSingleIneff matrixMultSingle

clean:
	rm -f forkTest simpleTest parallelSimpleTest