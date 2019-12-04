CC=g++
CFLAGS=-c -Wall
CXXSTD=-std=c++11
LDLIBS=-lm -lglut -lGLEW -lGL -lfreetype -L/usr/include/GL -lGLU -Wl,-rpath,
CXXFLAGS=-I/usr/include/freetype2
CXXUTILS=./common/shader_utils.o

BIN=../bin/

all: mde
debug: mde_debug

mde: main.o MDE.o text.o
	g++ -o mde main.o MDE.o text.o -L/usr/include/GL -lGL -lglut -lGLU -I../include -Wl,-rpath

mde_debug:main_d.o MDE_d.o text_d.o
	$(CC) -g -o $(BIN)mde_debug main_d.o MDE_d.o text_d.o $(LDLIBS) $(CXXUTILS)

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

MDE.o: MDE.cpp MDE.h
	g++ -std=c++11 -c MDE.cpp

text.o: text.cpp text.h
	g++ -c text.cpp 

main_d.o: main.cpp
	$(CC) $(CXXSTD) -g $(CFLAGS) main.cpp -o main_d.o $(CXXFLAGS)
	
MDE_d.o: MDE.cpp MDE.h
	$(CC) $(CXXSTD) -g $(CFLAGS)  MDE.cpp -o MDE_d.o

text_d.o: text.cpp text.h
	$(CC) $(CXXSTD) -g $(CFLAGS) text.cpp -o text_d.o $(CXXFLAGS)

clean:
	rm -rf *o mde
	rm -rf *o mde_debug

.PHONY: all debug clean
