.PHONY: all clean

INCLUDE = -I../include -Iinclude

obj/DatafileController.o:
	mkdir -p obj
	g++ -c $(INCLUDE) ../src/DatafileController.cpp -o $@

obj/DatafileControllerTest.o:
	mkdir -p obj
	g++ -c $(INCLUDE) src/DatafileControllerTest.cpp -o $@

DatafileControllerTest: obj/DatafileControllerTest.o obj/DatafileController.o
	g++ $(INCLUDE) $^ -o $@ 

RunTests:
	mkdir -p data
	./DatafileControllerTest

all: DatafileControllerTest

clean:
	rm -f obj/*.o DatafileControllerTest