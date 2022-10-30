run: project.o 
	g++ -o run project.o   
project.o: project.cpp   
	g++ -c -Wall -pedantic -g -std=c++17 project.cpp
clean: 
	rm project.o  run
