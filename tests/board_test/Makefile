all: main 
CXX = clang-18 -std=c++23 -O3 -Wall -Wextra
CXXFLAGS = -lstdc++ -pthread -lgtest -L /usr/local/include

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

build/board_test.o: ../../board_test.cpp
	$(CXX) -c -o build/board_test.o ../../board_test.cpp

main: build/pieces.o build/move.o build/board.o main.cpp
	$(CXX) -o main main.cpp build/pieces.o build/move.o build/board.o  $(CXXFLAGS)

clean:
	rm -f main build/main.o build/pieces.o build/board.o build/move.o build/test_board.o

$(info $(shell mkdir -p build))
