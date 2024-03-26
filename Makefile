.PHONY: clean all BuildTests RunTests

DataGeneratorTool:
	$(MAKE) -C DataGenerator obj-intel64/DataGenerator.so PIN_ROOT=$(PIN_HOME)

BuildTests:
	$(MAKE) -C DataGenerator/test all

RunTests:
	$(MAKE) -C DataGenerator/test RunTests

all: DataGeneratorTool BuildTests RunTests

clean:
	rm -f DataGenerator/obj-intel64/*
	$(MAKE) -C DataGenerator/test clean