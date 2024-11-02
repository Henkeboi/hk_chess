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

	for (int i = 0; i < 20; ++i) {
		alpha_beta::alpha_beta(board, depth, maximizing_player, best_move, depth);
	}
	return 0;

	
	bool is_game_finished = false;
	while (!is_game_finished) {
		visualization::update_visualization(board);
		alpha_beta::alpha_beta(board, depth, maximizing_player, best_move, depth);
		board = Board{board, best_move, 0};
		is_game_finished = board.is_game_finished(!maximizing_player);
		maximizing_player = !maximizing_player;
	}

	std::println("Game finished");

  return 0;
}
