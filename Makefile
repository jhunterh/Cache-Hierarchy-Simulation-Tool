.PHONY: clean all

DataGeneratorTool:
	$(MAKE) -C DataGenerator obj-intel64/DataGenerator.so PIN_ROOT=$(PIN_HOME) 

all: DataGeneratorTool

clean:
	rm DataGenerator/obj-intel64/*