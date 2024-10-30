#include <iostream>
#include <print>

#include "board.hpp"
#include "alpha_beta.hpp"
#include "visualization.hpp"
#include <vector>
#include <chrono>
#include <pthread.h>


int main() {
	Board board {};
	move::Move best_move{0, 0, 0, 0};
	int depth = 5;
	bool maximizing_player = true;
	
	for (int i = 0; i < 1000; ++i) {
		visualization::update_visualization(board);
		alpha_beta::alpha_beta(board, depth, INT_MIN, INT_MAX, maximizing_player, best_move, depth);
		board = Board{board, best_move};
		maximizing_player = !maximizing_player;
	}
  return 0;
}
