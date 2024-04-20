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

matrixMultOpenMP: matrixMultOpenMP.cpp
	g++ -o $@ $^ -fopenmp

matrixMultOpenMPIneff: matrixMultOpenMPIneff.cpp
	g++ -o $@ $^ -fopenmp

matrixMultFork: matrixMultFork.cpp
	g++ -o $@ $^

matrixMultForkIneff: matrixMultForkIneff.cpp
	g++ -o $@ $^

all: forkTest simpleTest parallelSimpleTest matrixMultSingleIneff matrixMultSingle matrixMultOpenMP matrixMultOpenMPIneff matrixMultFork matrixMultForkIneff

clean:
	rm -f forkTest simpleTest parallelSimpleTest matrixMultSingleIneff matrixMultSingle matrixMultOpenMP matrixMultOpenMPIneff matrixMultFork matrixMultForkIneff