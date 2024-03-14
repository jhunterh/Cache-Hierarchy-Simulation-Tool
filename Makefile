.PHONY: clean all

DataGeneratorTool:
	make -c DataGenerator/makefile PIN_ROOT=$(PIN_HOME) obj-intel64/DataGenerator.so

all: DataGeneratorTool

clean:
	rm DataGenerator/obj-intel64/*