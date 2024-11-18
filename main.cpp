#include <iostream>

#include "board.hpp"
#include "alpha_beta.hpp"
#include "visualization.hpp"
#include <vector>
#include <chrono>
#include <pthread.h>


int main() {
	Board board {};
	move::Move best_move{0, 0, 0, 0};
	int white_depth = 4;
	int black_depth = 4;
	bool maximizing_player = true;

	for (int i = 0; i < 20; ++i) {
		alpha_beta::alpha_beta(board, depth, maximizing_player, best_move, depth);
	}
	return 0;

	
	bool is_game_finished = false;
	while (!is_game_finished) {
		visualization::update_visualization(board);
		if (maximizing_player) {
			alpha_beta::alpha_beta_with_timeout(board, white_depth, maximizing_player, best_move, white_timeout_ms);
		} else {
			alpha_beta::alpha_beta_with_timeout(board, black_depth, maximizing_player, best_move, black_timeout_ms);
		}
		board = Board{board, best_move};
		is_game_finished = board.is_game_finished(!maximizing_player);
		maximizing_player = !maximizing_player;
	}

	std::cout << "Game finished\n";

  return 0;
}
