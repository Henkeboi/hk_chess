CXX = clang -std=c++23 -O3
CXXFLAGS = -lstdc++

all: main 

pieces.o: pieces.cpp
	$(CXX) -c -o pieces.o pieces.cpp

move.o: move.cpp
	$(CXX) -c -o move.o move.cpp

board.o: board.cpp
	$(CXX) -c -o board.o board.cpp

alpha_beta.o: alpha_beta.cpp
	$(CXX) -c -o alpha_beta.o alpha_beta.cpp

eval.o: eval.cpp 
	$(CXX) -c -o eval.o eval.cpp

visualization.o: visualization.cpp 
	$(CXX) -c -o visualization.o visualization.cpp


main: eval.o pieces.o move.o alpha_beta.o board.o visualization.o main.cpp
	$(CXX) -o main main.cpp move.o board.o alpha_beta.o eval.o visualization.o $(CXXFLAGS)

clean:
	rm -f main.o main eval.o alpha_beta.o board.o pieces.o move.o visualization.o
