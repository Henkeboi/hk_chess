#include <iostream>

#include "board.hpp"
#include "search.hpp"
#include "visualization.hpp"
#include "zobrist.hpp"
#include <vector>
#include "robin_map/robin_map.h"

bool check_if_threefold_repetition(tsl::robin_map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash);

int main() {
	Board board{};
	move::Move best_move{0, 0, 0, 0};
	int white_depth = 4;
	int black_depth = 4;
	bool maximizing_player = true;

	//for (int i = 0; i < 20; ++i) {
	//	search::search(board, 6, maximizing_player, best_move);
	//}
	//return 0;
	
	bool is_game_finished = false;
	Zobrist zobrist_hasher{board, maximizing_player};
	uint64_t zobrist_hash = zobrist_hasher.get_initial_zobrist_hash();
	tsl::robin_map<uint64_t, uint8_t> position_repeat_counter{};
	visualization::update_visualization(board);
	search::Search move_search{board, maximizing_player};

	while (!is_game_finished) {
		move::Move best_move = move_search.find_next_move(board, maximizing_player);
		board = Board{board, best_move};
		visualization::update_visualization(board);

		is_game_finished = board.is_game_finished(!maximizing_player);
		maximizing_player = !maximizing_player;
	}
	visualization::update_visualization(board);

	return 0;

	while (!is_game_finished) {
		if (maximizing_player) {
			search::search(board, white_depth, maximizing_player, best_move, zobrist_hash, zobrist_hasher, position_repeat_counter);
		} else {
			search::search(board, black_depth, maximizing_player, best_move, zobrist_hash, zobrist_hasher, position_repeat_counter);
		}
		Board prev_board = board;
		board = Board{board, best_move};
		visualization::update_visualization(board);
		zobrist_hash = zobrist_hasher.new_zobrist_hash(board, prev_board, best_move, zobrist_hash);
		if (check_if_threefold_repetition(position_repeat_counter, zobrist_hash)) {
			std::cout << "Threefold repetition\n";
			break;
		}
		is_game_finished = board.is_game_finished(!maximizing_player);
		maximizing_player = !maximizing_player;
	}
	std::cout << "Game over\n";
  return 0;
}

bool check_if_threefold_repetition(tsl::robin_map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash) {
	if (position_repeat_counter.find(zobrist_hash) == position_repeat_counter.end()) {
		position_repeat_counter[zobrist_hash] = 1;
	} else {
		position_repeat_counter[zobrist_hash] += 1;
		if (position_repeat_counter[zobrist_hash] == 3) {
			return true;
		}
	}
	return false;
}
