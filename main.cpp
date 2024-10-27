#include <print>

#include "board.hpp"
#include "minmax.hpp"
#include <vector>
#include <chrono>

int main() {
	auto t1 = std::chrono::high_resolution_clock::now();
	Board board;
	std::println("{}", minmax::minmax(board, 6, true));
	
	auto t2 = std::chrono::high_resolution_clock::now();
	std::println("{}", std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());

  return 0;
}
