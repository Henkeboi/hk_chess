all: main 
CXX = clang-18 -std=c++23 -O0 -Wall -Wextra
CXXFLAGS = -lstdc++ -lgtest -L /usr/local/include -pthread -lm

ubuntu: CXX = clang-19 -std=c++23 -O3 -Wall -Wextra -Werror
ubuntu: main

debug: CXX = clang-18 -std=c++23 -g -O0 -Wall -Wextra -fsanitize=bounds -fsanitize=address -fsanitize=leak
debug: main

build/pieces.o: ../../pieces.cpp 
	$(CXX) -c -o build/pieces.o ../../pieces.cpp

build/move.o: ../../move.cpp 
	$(CXX) -c -o build/move.o ../../move.cpp

build/board.o: ../../board.cpp
	$(CXX) -c -o build/board.o ../../board.cpp 

build/zobrist.o: ../../zobrist.cpp
	$(CXX) -c -o build/zobrist.o ../../zobrist.cpp 

build/zobrist_test.o: zobrist_test.cpp
	$(CXX) -c -o build/zobrist_test.o zobrist_test.cpp

main: build/pieces.o build/move.o build/board.o build/zobrist.o build/zobrist_test.o main.cpp
	$(CXX) -o main main.cpp build/pieces.o build/move.o build/board.o build/zobrist.o build/zobrist_test.o $(CXXFLAGS)

clean:
	rm -f main build/main.o build/pieces.o build/board.o build/move.o build/zobrist.o build/zobrist_test.o

$(info $(shell mkdir -p build))
