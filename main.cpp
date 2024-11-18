#include <iostream>

#include "board.hpp"
#include "alpha_beta.hpp"
#include "visualization.hpp"
#include <vector>
#include <chrono>
#include <thread>
#include <sys/resource.h>

int main() {
	Board board {};
	move::Move best_move{0, 0, 0, 0};
	int white_depth = 5;
	int black_depth = 5;
	int white_timeout_ms = 2000;
	int black_timeout_ms = 2000;
	bool maximizing_player = true;

	//for (int i = 0; i < 20; ++i) {
	//	alpha_beta::alpha_beta_with_timeout(board, 5, maximizing_player, best_move, 1000);
	//}
	//return 0;
	
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
