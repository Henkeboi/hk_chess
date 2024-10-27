CXX = clang -std=c++23 -O3
CXXFLAGS = -lstdc++

all: main 

pieces.o: pieces.cpp
	$(CXX) -c -o pieces.o pieces.cpp

move.o: move.cpp
	$(CXX) -c -o move.o move.cpp

board.o: pieces.o move.o board.cpp
	$(CXX) -c -o board.o move.o pieces.o board.cpp

main: pieces.o move.o board.o main.cpp
	$(CXX) -o main main.cpp move.o board.o $(CXXFLAGS)

clean:
	rm -f main.o main board.o pieces.o move.o
