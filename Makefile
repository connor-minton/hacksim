CXX_FLAGS = --std=c++17 -g

all: hacksim Chips_test

clean:
	rm -f *.o hacksim Chips_test

hacksim: Main.o
	g++ $(CXX_FLAGS) Main.o -o hacksim

Main.o: Main.cpp Chips.h ShallowChips.h
	g++ $(CXX_FLAGS) -c Main.cpp -o Main.o

Chips_test: Chips_test.cpp Chips.h ShallowChips.h
	g++ $(CXX_FLAGS) Chips_test.cpp -o Chips_test