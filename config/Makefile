.PHONY: clean all

INCLUDES = -Iinclude -Iinclude/nlohmann

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=obj/%.o)

obj/%.o: src/%.cpp
	mkdir -p obj
	g++ -c -o $@ $(INCLUDES) $^

ConfigParserTest: $(OBJS)
	g++ -o $@ $(INCLUDES) $^

all: ConfigParserTest

clean:
	rm -rf obj/*