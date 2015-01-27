GL = -lglut -lGLU -lGL

all: clean

2 : 2.cpp model.o
	g++ -o 2.exe 2.cpp model.o $(GL); ./2.exe

3 : 3.cpp model.o
	g++ -o 3.exe 3.cpp model.o $(GL); ./3.exe

model.o : model.cpp
	g++ -c model.cpp;

clean :
	rm -rf *.exe *.o
