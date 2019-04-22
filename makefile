CC=g++
CFLAGS=-c -Wall
CXXSTD=-std=c++11
LDLIBS=-lm -lglut -lGLEW -lGL -lfreetype -L/usr/include/GL -lGLU -Wl,-rpath,../lib/x64
CXXFLAGS=-I/usr/include/freetype2
CXXUTILS=./common/shader_utils.o

# CLEAP =  -I../include ../lib/x64/libLeap.so

all: esv8
debug: esv8_debug

esv8: main.o E.o glFunctions.o text.o
	g++ -o esv8 main.o E.o glFunctions.o text.o -L/usr/include/GL -lGL -lglut -lGLU -I../include ../lib/x64/libLeap.so -Wl,-rpath,../lib/x64

esv8_debug:main_d.o E_d.o glFunctions_d.o text_d.o
	$(CC) -g -o esv8_debug main_d.o E_d.o glFunctions_d.o text_d.o $(LDLIBS) $(CXXUTILS)

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

E.o: E.cpp E.h
	g++ -std=c++11 -c E.cpp

glFunctions.o: glFunctions.cpp glFunctions.h
	g++ -std=c++11 -c glFunctions.cpp

text.o: text.cpp text.h
	g++ -c text.cpp 

main_d.o: main.cpp
	$(CC) $(CXXSTD) -g $(CFLAGS) main.cpp -o main_d.o $(CXXFLAGS)
	
E_d.o: E.cpp E.h
	$(CC) $(CXXSTD) -g $(CFLAGS)  E.cpp -o E_d.o

glFunctions_d.o: glFunctions.cpp glFunctions.h
	$(CC) $(CXXSTD) -g $(CFLAGS)  glFunctions.cpp -o glFunctions_d.o

text_d.o: text.cpp text.h
	$(CC) $(CXXSTD) -g $(CFLAGS) text.cpp -o text_d.o $(CXXFLAGS)

clean:
	rm -rf *o esv8
	rm -rf *o esv8_debug
.PHONY: all clean
