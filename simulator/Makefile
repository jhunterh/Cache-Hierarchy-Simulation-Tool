CC=gcc
CXX=g++

all:
	cmake . -Bbuild/ 
	cd build && make -j && make install

clean:
	rm -rf bin
	rm -rf build