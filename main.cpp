#include <print>

#include "board.hpp"
#include <chrono>

int main() {
	auto t1 = std::chrono::high_resolution_clock::now();
	Board board;

	for (int i = 0; i < 2000000; ++i) {
	//for (int i = 0; i < 1; ++i) {
		board.get_white_moves();
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms = t2 - t1;
	std::println("{}", ms);

  return 0;
}

