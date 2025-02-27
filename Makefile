all: main 
CXX = clang -std=c++23 -O3 -Wall -Wextra
CXXFLAGS = -lstdc++ -lm -lcurl

ubuntu: CXX = clang-19 -std=c++23 -O3 -Wall -Wextra -Werror
ubuntu: main

debug: CXX = clang-18 -std=c++23 -g -O0 -Wall -Wextra -fsanitize=bounds -fsanitize=address -fsanitize=leak
debug: main

build/robin_map.o: robin_map/robin_map.cpp 
	$(CXX) -c -o build/robin_map.o robin_map/robin_map.cpp

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

build/chess_server.o: chess_server.cpp
	$(CXX) -c -o build/chess_server.o chess_server.cpp

main: build/robin_map.o build/pieces.o build/move.o build/board.o build/eval.o build/zobrist.o build/search.o build/chess_server.o main.cpp
	$(CXX) -o main main.cpp build/robin_map.o build/pieces.o build/move.o build/board.o build/search.o build/eval.o build/zobrist.o build/chess_server.o $(CXXFLAGS)

clean:
	rm -f build/main.o main build/robin_map.o build/pieces.o build/eval.o build/search.o build/board.o build/move.o build/zobrist.o build/chess_server.o

$(info $(shell mkdir -p build))
