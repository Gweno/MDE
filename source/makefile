CC=g++
CFLAGS=-c -Wall
CXXSTD=-std=c++17
LDLIBS=-lm -lglut -lGLEW -lGL -lfreetype -L/usr/include/GL -lGLU -lstdc++fs -Wl,-rpath, 
CXXFLAGS=-no-pie
CXXFLAGS2=-I/usr/include/freetype2
CXXUTILS=./common/shader_utils.o

BIN=../bin/

all: mde
debug: mde_debug

mde: main.o MDE.o text.o utils.o
#~ 	$(CC) $(CXXSTD) -o $(BIN)mde main_d.o MDE_d.o text_d.o $(LDLIBS) $(CXXUTILS)
	$(CC) $(CXXSTD) -o $(BIN)mde main.o MDE.o text.o utils.o $(LDLIBS) $(CXXUTILS) $(CXXFLAGS)

mde_debug:main_d.o MDE_d.o text_d.o utils_d.o
	$(CC) $(CXXSTD) -g -o $(BIN)mde_debug main_d.o MDE_d.o text_d.o utils_d.o $(LDLIBS) $(CXXUTILS) $(CXXFLAGS)
#~ 	$(CC) $(CXXSTD) -g -o $(BIN)mde_debug main_d.o MDE_d.o text_d.o $(LDLIBS) $(CXXUTILS) -lstdc++fs

main.o: main.cpp
	$(CC) $(CXXSTD) -c $(CFLAGS) main.cpp -o main.o $(CXXFLAGS2)

MDE.o: MDE.cpp MDE.h
	$(CC) $(CXXSTD) -c $(CFLAGS)  MDE.cpp -o MDE.o

text.o: text.cpp text.h
	$(CC) $(CXXSTD) -c $(CFLAGS) text.cpp -o text.o $(CXXFLAGS2)

utils.o: utils.cpp utils.h
	$(CC) $(CXXSTD) -c $(CFLAGS) utils.cpp -o utils.o $(CXXFLAGS2)

main_d.o: main.cpp
	$(CC) $(CXXSTD) -g $(CFLAGS) main.cpp -o main_d.o $(CXXFLAGS2)
	
MDE_d.o: MDE.cpp MDE.h
	$(CC) $(CXXSTD) -g $(CFLAGS)  MDE.cpp -o MDE_d.o

text_d.o: text.cpp text.h
	$(CC) $(CXXSTD) -g $(CFLAGS) text.cpp -o text_d.o $(CXXFLAGS2)

utils_d.o: utils.cpp utils.h
	$(CC) $(CXXSTD) -g $(CFLAGS) utils.cpp -o utils_d.o $(CXXFLAGS2)

clean:
	rm -rf *o mde
	rm -rf *o mde_debug

.PHONY: all debug clean
