CXX = clang-19 -std=c++23 -O3 -Wall -Wextra -g 
CXXFLAGS = -lstdc++a -fexperimental-library

all: main 

pieces.o: pieces.cpp 
	$(CXX) -c -o build/pieces.o pieces.cpp

move.o: move.cpp 
	$(CXX) -c -o build/move.o move.cpp

board.o: board.cpp
	$(CXX) -c -o build/board.o board.cpp 

alpha_beta.o: alpha_beta.cpp
	$(CXX) -c -o build/alpha_beta.o alpha_beta.cpp

eval.o: eval.cpp 
	$(CXX) -c -o build/eval.o eval.cpp

visualization.o: visualization.cpp 
	$(CXX) -c -o build/visualization.o visualization.cpp


main: pieces.o eval.o move.o alpha_beta.o board.o visualization.o main.cpp
	$(CXX) -o main main.cpp build/pieces.o build/move.o build/board.o build/alpha_beta.o build/eval.o build/visualization.o $(CXXFLAGS)

clean:
	rm -f build/main.o main build/pieces.o build/eval.o build/alpha_beta.o build/board.o build/move.o build/visualization.o


$(info $(shell mkdir -p build))
