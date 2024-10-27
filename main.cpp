#include <print>

#include "board.hpp"
#include "move.hpp"
#include <vector>
#include <chrono>

int main() {
	auto t1 = std::chrono::high_resolution_clock::now();
	Board board;
	std::vector<move::Move> moves;
	std::vector<Board> boards;

	for (int i = 0; i < 4000000; ++i) {
	//for (int i = 0; i < 1; ++i) {
		board.get_black_moves(moves, boards);
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms = t2 - t1;
	std::println("{}", ms);

  return 0;
}

