CXX_FLAGS = --std=c++17 -g

all: hacksim

clean:
	rm -f *.o hacksim

hacksim: Main.o
	g++ $(CXX_FLAGS) Main.o -o hacksim

Main.o: Main.cpp
	g++ $(CXX_FLAGS) -c Main.cpp -o Main.o