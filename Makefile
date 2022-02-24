CXX_FLAGS = --std=c++17 -g

all: hacksim test dasm

test: Chips_test FileUtils_test Computer_test

clean:
	rm -f *.o hacksim Chips_test FileUtils_test Computer_test

hacksim: Main.o
	g++ $(CXX_FLAGS) Main.o -o hacksim

Main.o: Main.cpp Chips.h ShallowChips.h
	g++ $(CXX_FLAGS) -c Main.cpp -o Main.o

Chips_test: Chips_test.cpp Chips.h ShallowChips.h FileUtils.h Test.h
	g++ $(CXX_FLAGS) Chips_test.cpp -o Chips_test

FileUtils_test: FileUtils_test.cpp FileUtils.h Test.h
	g++ $(CXX_FLAGS) FileUtils_test.cpp -o FileUtils_test

Computer_test: Computer_test.cpp Chips.h FileUtils.h Test.h
	g++ $(CXX_FLAGS) Computer_test.cpp -o Computer_test

FileUtils.h: Exceptions.h Bits.h

Chips.h: Bits.h ShallowChips.h FileUtils.h

dasm: FileUtils.h Exceptions.h
	g++ $(CXX_FLAGS) dasm.cpp -o dasm