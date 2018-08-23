all: esv8

esv8: main.o Entity.o glDisplay.o DataFile.o E.o
	g++ -o esv7 main.o Entity.o glDisplay.o DataFile.o E.o -L/usr/include/GL -lGL -lglut -lGLU -I../include ../lib/x64/libLeap.so -Wl,-rpath,../lib/x64

main.o: main.cpp
	g++ -c main.cpp

Entity.o: Entity.cpp Entity.h
	g++ -c Entity.cpp

glDisplay.o: glDisplay.cpp glDisplay.h
	g++ -c glDisplay.cpp

DataFile.o: DataFile.cpp DataFile.h
	g++ -c DataFile.cpp

E.o: E.cpp E.h
	g++ -c E.cpp

clean:
	rm -rf *o esv8
