all: esv8
debug: esv8_debug

esv8: main.o E.o glFunctions.o
	g++ -o esv8 main.o E.o glFunctions.o -L/usr/include/GL -lGL -lglut -lGLU -I../include ../lib/x64/libLeap.so -Wl,-rpath,../lib/x64

esv8_debug:main_d.o E_d.o glFunctions_d.o
	g++ -g -o esv8_debug main_d.o E_d.o glFunctions_d.o -L/usr/include/GL -lGL -lglut -lGLU -I../include ../lib/x64/libLeap.so -Wl,-rpath,../lib/x64

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

E.o: E.cpp E.h
	g++ -std=c++11 -c E.cpp

glFunctions.o: glFunctions.cpp glFunctions.h
	g++ -std=c++11 -c glFunctions.cpp

main_d.o: main.cpp
	g++ -std=c++11 -g -c -Wall main.cpp -o main_d.o

E_d.o: E.cpp E.h
	g++ -std=c++11 -g -c -Wall E.cpp -o E_d.o

glFunctions_d.o: glFunctions.cpp glFunctions.h
	g++ -std=c++11 -g -c -Wall glFunctions.cpp -o glFunctions_d.o

clean:
	rm -rf *o esv8
	rm -rf *o esv8_debug
