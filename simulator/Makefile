INCLUDES = -Iinclude -Iinclude/replacementpolicy -I../DataGenerator/include
LIBS = -lstdc++fs

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

all: $(REPL_OBJS) $(SIM_OBJS)

clean:
	rm -rf obj/*