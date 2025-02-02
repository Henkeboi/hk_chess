#include "search.hpp"
#include "eval.hpp"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <thread>
#include <limits>
#include "assert.h"

namespace {
	SearchStatistics::SearchStatistics()
	: timeout_exceeded(false), positions_evaluated(0), positions_generated(0) {
	}

	int alpha_beta(const Board& node, int depth, bool maximizing_player, Zobrist& zobrist_hasher, std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t node_hash, move::Move& best_move, const int start_depth, SearchStatistics& search_statistics, int alpha=INT_MIN, int beta=INT_MAX);
	bool is_threefold_repetition(std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash);
	void decrement_position_repeat_counter(std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash);
	move::Move get_random_move(const Board& board, bool maximizing_player);
}

namespace search {
	void search(const Board board, int depth, bool maximizing_player, Zobrist zobrist_hasher, std::map<uint64_t, uint8_t> position_repeat_counter, uint64_t zobrist_hash, move::Move& best_move) {
		SearchStatistics search_statistics{};
		int evaluation = alpha_beta(board, depth, maximizing_player, zobrist_hasher, position_repeat_counter, zobrist_hash, best_move, depth, search_statistics);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		//std::cout << "Positions generated: " << search_statistics.positions_generated << "\n";
		//std::cout << "Positions evaluated: " << search_statistics.positions_evaluated << "\n";
		//std::cout << "Evaluation: " << evaluation << "\n";

		if ((evaluation < -20000 && maximizing_player) || (evaluation > 20000 && !maximizing_player)) {
			best_move = get_random_move(board,  maximizing_player);
		}
	}
}

namespace {
	int alpha_beta(const Board& node, int depth, bool maximizing_player, Zobrist& zobrist_hasher, std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t node_hash, move::Move& best_move, const int start_depth, SearchStatistics& search_statistics, int alpha, int beta) {
 	std::vector<move::Move> moves;
 	std::vector<Board> boards;

	bool checking_forced = false;
 	if (maximizing_player) {
 		if (depth > 0) {
 			node.get_white_moves(moves, boards, false);
 		} else {
 			node.get_white_moves(moves, boards, true);
			checking_forced = true;
 		}
	} else {
		if (depth > 0) {
			node.get_black_moves(moves, boards, false);
		} else {
			node.get_black_moves(moves, boards, true);
			checking_forced = true;
		}
	}

	if (!checking_forced) {
		if (moves.size() == 0) {
			return 0; // Stalemate
		}

		if (maximizing_player) {
			if (node.black_in_checkmate(moves)) {
				search_statistics.positions_evaluated += 1;
				return INT_MAX;
			}
		} else {
			if (node.white_in_checkmate(moves)) {
				search_statistics.positions_evaluated += 1;
				return INT_MIN;
			}
		}
	} else if (moves.size() == 0) {
		search_statistics.positions_evaluated += 1;
		return eval::eval(node);
	}

	if (maximizing_player) {
		int eval = INT_MIN;
		for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
			search_statistics.positions_generated += 1;	
			uint64_t zobrist_hash = zobrist_hasher.new_zobrist_hash(boards[i], node, moves[i], node_hash);
			int new_eval = 0;
			if (!is_threefold_repetition(position_repeat_counter, zobrist_hash)) {
				new_eval = alpha_beta(boards[i], depth - 1, !maximizing_player, zobrist_hasher, position_repeat_counter, zobrist_hash, best_move, start_depth, search_statistics, alpha, beta);
			}
			decrement_position_repeat_counter(position_repeat_counter, zobrist_hash);
			if (eval < new_eval) {
				eval = new_eval;
				if (depth == start_depth) {
					best_move = moves[i];
				}
				if (eval > beta) {
					break;
				}
				alpha = std::max(alpha, eval);
			} 
		}
		return eval;
	} else {
		int eval = INT_MAX;
		for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
			search_statistics.positions_generated += 1;	
			uint64_t zobrist_hash = zobrist_hasher.new_zobrist_hash(boards[i], node, moves[i], node_hash);
			int new_eval = 0;
			if (!is_threefold_repetition(position_repeat_counter, zobrist_hash)) {
				new_eval = alpha_beta(boards[i], depth - 1, !maximizing_player, zobrist_hasher, position_repeat_counter, zobrist_hash, best_move, start_depth, search_statistics, alpha, beta);
			}
			decrement_position_repeat_counter(position_repeat_counter, zobrist_hash);
			if (eval > new_eval) {
				eval = new_eval;
				if (depth == start_depth) {
					best_move = moves[i];
				}
				if (eval < alpha) {
					break;
				}
				beta = std::min(beta, eval);
			}
		}
		return eval;
	}
}

void decrement_position_repeat_counter(std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash) {
	if (position_repeat_counter[zobrist_hash] == 1) {
		position_repeat_counter.erase(position_repeat_counter.find(zobrist_hash));
	} else {
		position_repeat_counter[zobrist_hash] -= 1;
	}
}

bool is_threefold_repetition(std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash) {
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

move::Move get_random_move(const Board& board, bool maximizing_player) {
	if (maximizing_player) {
		std::vector<move::Move> moves;
		std::vector<Board> boards;
		board.get_white_moves(moves, boards, false);
		for (size_t i = 0; i < boards.size(); ++i) {
			std::vector<move::Move> black_moves;
			std::vector<Board> black_boards;
			boards[i].get_black_moves(black_moves, black_boards, false);
			if (!boards[i].white_in_checkmate(black_moves)) {
				return moves[i];
			}
		}
	}	else {
		std::vector<move::Move> moves;
		std::vector<Board> boards;
		board.get_black_moves(moves, boards, false);
		for (size_t i = 0; i < boards.size(); ++i) {
			std::vector<move::Move> white_moves;
			std::vector<Board> white_boards;
			boards[i].get_white_moves(white_moves, white_boards, false);
			if (!boards[i].black_in_checkmate(white_moves)) {
				return moves[i];
			}
		}
	}
	if (maximizing_player) {
		std::cout << "White in checkmate\n";
		exit(0);
	} else {
		std::cout << "Black in checkmate\n";
		exit(0);
	}
}
}
