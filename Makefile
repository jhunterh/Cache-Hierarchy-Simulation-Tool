.PHONY: clean all Simulator SimulatorExample BuildTests RunTests

INCLUDES = -Isimulator/include -Isimulator/include/replacementpolicy -IDataGenerator/include
LIBS = -Lsimulator/lib -lsimulator -lstdc++fs 

DataGeneratorTool:
	$(MAKE) -C DataGenerator obj-intel64/DataGenerator.so PIN_ROOT=$(PIN_HOME)

Simulator:
	$(MAKE) -C simulator all

SimulatorExample: Simulator
	g++ -std=c++17 -o $@ $(INCLUDES) simulator-example/src/simulation.cpp $(LIBS)

BuildTests:
	$(MAKE) -C DataGenerator/test all

RunTests:
	$(MAKE) -C DataGenerator/test RunTests

all: DataGeneratorTool Simulator BuildTests RunTests

clean:
	rm -f DataGenerator/obj-intel64/*
	$(MAKE) -C simulator clean
	$(MAKE) -C DataGenerator/test clean