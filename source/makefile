CC=g++
CFLAGS=-c -Wall
CXXSTD=-std=c++17
LDLIBS=-lm -lglut -lGLEW -lGL -lfreetype -L/usr/include/GL -lGLU -lstdc++fs -Wl,-rpath, 
CXXFLAGS=-I/usr/include/freetype2
CXXUTILS=./common/shader_utils.o

BIN=../bin/

all: mde
debug: mde_debug

mde: main.o MDE.o text.o
	$(CC) $(CXXSTD) -o $(BIN)mde main_d.o MDE_d.o text_d.o $(LDLIBS) $(CXXUTILS)

mde_debug:main_d.o MDE_d.o text_d.o
	$(CC) $(CXXSTD) -g -o $(BIN)mde_debug main_d.o MDE_d.o text_d.o $(LDLIBS) $(CXXUTILS)
#~ 	$(CC) $(CXXSTD) -g -o $(BIN)mde_debug main_d.o MDE_d.o text_d.o $(LDLIBS) $(CXXUTILS) -lstdc++fs

main.o: main.cpp
	$(CC) $(CXXSTD) -g $(CFLAGS) main.cpp -o main_d.o $(CXXFLAGS)

MDE.o: MDE.cpp MDE.h
	$(CC) $(CXXSTD) -c $(CFLAGS)  MDE.cpp -o MDE_d.o

text.o: text.cpp text.h
	$(CC) $(CXXSTD) -c $(CFLAGS) text.cpp -o text_d.o $(CXXFLAGS)

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
