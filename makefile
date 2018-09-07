all: esv8

#~ esv8: main.o Entity.o glDisplay.o DataFile.o E.o
#~ 	g++ -o esv8 main.o Entity.o glFunctions.o DataFile.o E.o -L/usr/include/GL -lGL -lglut -lGLU -I../include ../lib/x64/libLeap.so -Wl,-rpath,../lib/x64
#~ esv8: main.o glFunctions.o DataFile.o E.o
#~ 	g++ -o esv8 main.o glFunctions.o DataFile.o E.o -L/usr/include/GL -lGL -lglut -lGLU -I../include ../lib/x64/libLeap.so -Wl,-rpath,../lib/x64
esv8: main.o E.o glFunctions.o
	g++ -o esv8 main.o E.o glFunctions.o -L/usr/include/GL -lGL -lglut -lGLU -I../include ../lib/x64/libLeap.so -Wl,-rpath,../lib/x64

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

#~ Entity.o: Entity.cpp Entity.h
#~ 	g++ -c Entity.cpp

#~ glFunctions.o: glFunctions.cpp glFunctions.h
#~ 	g++ -std=c++11 -c glFunctions.cpp

E.o: E.cpp E.h
	g++ -std=c++11 -c E.cpp

#~ DataFile.o: DataFile.cpp DataFile.h
#~ 	g++ -c DataFile.cpp

#~ E.o: E.cpp E.h
#~ 	g++ -c E.cpp

glFunctions.o: glFunctions.cpp glFunctions.h
	g++ -std=c++11 -c glFunctions.cpp

clean:
	rm -rf *o esv8
