CXX = clang -std=c++23 -O3
CXXFLAGS = -lstdc++

all: main 

pieces.o: pieces.cpp
	$(CXX) -c -o pieces.o pieces.cpp

move.o: move.cpp
	$(CXX) -c -o move.o move.cpp

board.o: board.cpp
	$(CXX) -c -o board.o board.cpp

minmax.o: minmax.cpp
	$(CXX) -c -o minmax.o minmax.cpp

eval.o: eval.cpp 
	$(CXX) -c -o eval.o eval.cpp

main: eval.o pieces.o move.o minmax.o board.o main.cpp
	$(CXX) -o main main.cpp move.o board.o minmax.o eval.o $(CXXFLAGS)

clean:
	rm -f main.o main minmax.o board.o pieces.o move.o
