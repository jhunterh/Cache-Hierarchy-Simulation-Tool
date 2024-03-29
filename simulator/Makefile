INCLUDES = -Iinclude -Iinclude/replacementpolicy -I../DataGenerator/include

REPL_SRCS = $(wildcard src/replacementpolicy/*.cpp)
REPL_OBJS = $(REPL_SRCS:src/replacementpolicy/%.cpp=obj/replacementpolicy/%.o)

SIM_SRCS = $(wildcard src/*.cpp)
SIM_OBJS = $(SIM_SRCS:src/%.cpp=obj/%.o)

obj/%.o: src/%.cpp
	mkdir -p obj
	g++ -std=c++17 -c -o $@ $(INCLUDES) $^

obj/replacementpolicy/%.o: src/replacementpolicy/%.cpp
	mkdir -p obj/replacementpolicy
	g++ -std=c++17 -c -o $@ $(INCLUDES) $^

lib/libsimulator.a: $(REPL_OBJS) $(SIM_OBJS)
	mkdir -p lib
	ar rcs $@ $^

all: lib/libsimulator.a

clean:
	rm -rf obj/*
	rm -rf lib/*