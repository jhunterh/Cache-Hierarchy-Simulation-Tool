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

all: DatafileControllerTest

clean:
	rm obj/*.o DatafileControllerTest