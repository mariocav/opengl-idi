GL = -lglut -lGLU -lGL

all: clean

1 : 1.cpp
	g++ -o 1.exe 1.cpp $(GL); ./1.exe

2_1 : 2_1.cpp
	g++ -o 2_1.exe 2_1.cpp $(GL); ./2_1.exe

2_2 : 2_2.cpp
	g++ -o 2_2.exe 2_2.cpp $(GL); ./2_2.exe

2_3 : 2_3.cpp
	g++ -o 2_3.exe 2_3.cpp $(GL); ./2_3.exe

utils.o : utils.cpp
	g++ -c utils.cpp -lglut -lGLU -lGL;

2_4_triangle : 2_4_triangle.cpp utils.o
	g++ -o 2_4_triangle.exe utils.o 2_4_triangle.cpp $(GL); ./2_4_triangle.exe

2_4_tetera : 2_4_tetera.cpp utils.o
	g++ -o 2_4_tetera.exe utils.o 2_4_tetera.cpp $(GL); ./2_4_tetera.exe

3 : 3.cpp utils.o
	g++ -o 3.exe utils.o 3.cpp $(GL); ./3.exe

model.o : model.cpp
	g++ -c model.cpp;

4 : 4.cpp model.o utils.o
	g++ -o 4.exe utils.o model.o 4.cpp $(GL);

5 : 5.cpp model.o utils.o
	g++ -o 5.exe utils.o model.o 5.cpp $(GL);

clean :
	rm -rf *.exe *.o
