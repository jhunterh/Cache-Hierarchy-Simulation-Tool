.PHONY: clean all Simulator SimulatorExample BuildTests RunTests

INCLUDES = -Isimulator/include -Isimulator/include/replacementpolicy
LIBS = -Lsimulator/bin -lsimulator -lstdc++fs 

DataGeneratorTool:
	$(MAKE) -C DataGenerator obj-intel64/DataGenerator.so PIN_ROOT=$(PIN_HOME)

Simulator:
	$(MAKE) -C simulator all

SimulatorMain: Simulator
	g++ -std=c++17 -o $@ $(INCLUDES) simulator-main/src/simulation.cpp $(LIBS)

BuildTests:
	$(MAKE) -C DataGenerator/test all

RunTests:
	$(MAKE) -C DataGenerator/test RunTests
	simulator/bin/test_simulator

BenchTests:
	$(MAKE) -C DataGenerator/tests all

all: DataGeneratorTool Simulator SimulatorMain BenchTests BuildTests RunTests

clean:
	rm -f DataGenerator/obj-intel64/*
	$(MAKE) -C simulator clean
	$(MAKE) -C DataGenerator/test clean
	$(MAKE) -C DataGenerator/tests clean