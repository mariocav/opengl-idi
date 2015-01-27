GL = -lglut -lGLU -lGL

all: clean

2 : 2.cpp
	g++ -o 2.exe 2.cpp $(GL); ./2.exe

3 : 3.cpp
	g++ -o 3.exe 3.cpp $(GL); ./3.exe

4 : 4.cpp
	g++ -o 4.exe 4.cpp $(GL); ./4.exe

5 : 5.cpp model.o
	g++ -o 5.exe 5.cpp model.o $(GL); ./5.exe

6_1 : 6_1.cpp model.o
	g++ -o 6_1.exe 6_1.cpp model.o $(GL); ./6_1.exe

6_2 : 6_2.cpp model.o
	g++ -o 6_2.exe 6_2.cpp model.o $(GL); ./6_2.exe

7 : 7.cpp model.o
	g++ -o 7.exe 7.cpp model.o $(GL); ./7.exe

8 : 8.cpp model.o
	g++ -o 8.exe 8.cpp model.o $(GL); ./8.exe

9_2 : 9_2.cpp model.o
	g++ -o 9_2.exe 9_2.cpp model.o $(GL); ./9_2.exe

10 : 10.cpp model.o
	g++ -o 10.exe 10.cpp model.o $(GL); ./10.exe

model.o : model.cpp
	g++ -c model.cpp;

clean :
	rm -rf *.exe *.o
