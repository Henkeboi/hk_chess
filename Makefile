all: main 
CXX = clang-18 -std=c++23 -O3 -Wall -Wextra
CXXFLAGS = -lstdc++

ubuntu: CXX = clang-19 -std=c++23 -O3 -Wall -Wextra -Werror
ubuntu: main

debug: CXX = clang-18 -std=c++23 -g -O0 -Wall -Wextra -fsanitize=bounds -fsanitize=address -fsanitize=leak
debug: main

build/pieces.o: pieces.cpp 
	$(CXX) -c -o build/pieces.o pieces.cpp

build/move.o: move.cpp 
	$(CXX) -c -o build/move.o move.cpp

build/board.o: board.cpp
	$(CXX) -c -o build/board.o board.cpp 

build/search.o: search.cpp
	$(CXX) -c -o build/search.o search.cpp

build/eval.o: eval.cpp 
	$(CXX) -c -o build/eval.o eval.cpp

build/zobrist.o: zobrist.cpp
	$(CXX) -c -o build/zobrist.o zobrist.cpp

build/visualization.o: visualization.cpp 
	$(CXX) -c -o build/visualization.o visualization.cpp

main: build/pieces.o build/move.o build/board.o build/eval.o build/zobrist.o build/search.o build/visualization.o main.cpp
	$(CXX) -o main main.cpp build/pieces.o build/move.o build/board.o build/search.o build/eval.o build/zobrist.o build/visualization.o $(CXXFLAGS)

clean:
	rm -f build/main.o main build/pieces.o build/eval.o build/search.o build/board.o build/move.o build/visualization.o build/zobrist.o

$(info $(shell mkdir -p build))
