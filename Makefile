all: main 
CXX = clang-18 -std=c++23 -O3 -Wall -Wextra
CXXFLAGS = -lstdc++

ubuntu: CXX = clang-19 -std=c++23 -O3 -Wall -Wextra -Werror
ubuntu: main

debug: CXX = clang-18 -std=c++23 -g -O0 -Wall -Wextra -fsanitize=bounds -fsanitize=address -fsanitize=leak
debug: main

pieces.o: pieces.cpp 
	$(CXX) -c -o build/pieces.o pieces.cpp

move.o: move.cpp 
	$(CXX) -c -o build/move.o move.cpp

board.o: board.cpp
	$(CXX) -c -o build/board.o board.cpp 

search.o: search.cpp
	$(CXX) -c -o build/search.o search.cpp

eval.o: eval.cpp 
	$(CXX) -c -o build/eval.o eval.cpp

zobrist.o: zobrist.cpp
	$(CXX) -c -o build/zobrist.o zobrist.cpp

visualization.o: visualization.cpp 
	$(CXX) -c -o build/visualization.o visualization.cpp

main: pieces.o eval.o zobrist.o move.o search.o board.o visualization.o main.cpp
	$(CXX) -o main main.cpp build/pieces.o build/move.o build/board.o build/search.o build/eval.o build/zobrist.o build/visualization.o $(CXXFLAGS)

clean:
	rm -f build/main.o main build/pieces.o build/eval.o build/search.o build/board.o build/move.o build/visualization.o

$(info $(shell mkdir -p build))
