# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

all: clean link

link: compile
	g++ -o bin/simulation bin/main.o bin/Settlement.o bin/Facility.o bin/SelectionPolicy.o bin/Plan.o bin/Simulation.o bin/Action.o bin/Auxiliary.o

compile: src/Action.cpp src/Auxiliary.cpp src/Facility.cpp src/main.cpp src/Plan.cpp src/SelectionPolicy.cpp src/Settlement.cpp src/Simulation.cpp
	@echo "compiling source code"
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/Settlement.o src/Settlement.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/Facility.o src/Facility.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/Plan.o src/Plan.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/Simulation.o src/Simulation.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -I./include -c -o bin/Auxiliary.o src/Auxiliary.cpp

clean:
	@echo "cleaning bin directory"
	rm -f bin/*

