#include "search.hpp"
#include "eval.hpp"
#include "zobrist.hpp"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <limits.h>
#include <optional>

namespace {
	SearchStatistics::SearchStatistics()
	: timeout_exceeded(false), positions_evaluated(0), positions_generated(0) {
	}

	int alpha_beta(const Board& node, int depth, bool maximizing_player, Zobrist zobrist_hasher, std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t node_hash, move::Move& best_move, const int start_depth, SearchStatistics& search_statistics, int alpha=INT_MIN, int beta=INT_MAX);
	bool is_threefold_repetition(std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash);
	void decrement_position_repeat_counter(std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash);
}

namespace search {
	void search(const Board board, int depth, bool maximizing_player, Zobrist zobrist_hasher, std::map<uint64_t, uint8_t> position_repeat_counter, uint64_t zobrist_hash, move::Move& best_move) {
		SearchStatistics search_statistics{};
		int evaluation = alpha_beta(board, depth, maximizing_player, zobrist_hasher, position_repeat_counter, zobrist_hash, best_move, depth, search_statistics);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		std::cout << "Positions generated: " << search_statistics.positions_generated << "\n";
		std::cout << "Positions evaluated: " << search_statistics.positions_evaluated << "\n";
		std::cout << "Evaluation: " << evaluation << "\n";
	}
}

namespace {
	int alpha_beta(const Board& node, int depth, bool maximizing_player, Zobrist zobrist_hasher, std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t node_hash, move::Move& best_move, const int start_depth, SearchStatistics& search_statistics, int alpha, int beta) {
 	std::vector<move::Move> moves;
 	std::vector<Board> boards;

 	if (maximizing_player) {
 		if (depth > 0) {
 			node.get_white_moves(moves, boards, false);
 		} else {
 			node.get_white_moves(moves, boards, true);
 		}
	} else {
		if (depth > 0) {
			node.get_black_moves(moves, boards, false);
		} else {
			node.get_black_moves(moves, boards, true);
		}
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

	if (moves.size() == 0) {
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

	std::optional<move::Move> get_random_move_if_not_checkmate(const Board& board, bool maximizing_player) {
		std::vector<move::Move> moves;
		std::vector<Board> boards;
		if (maximizing_player) {
			board.get_white_moves(moves, boards, false);
			std::vector<move::Move> checkmating_moves;
			std::vector<Board> checkmating_boards;
			for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
				boards[i].get_black_moves(checkmating_moves, checkmating_boards, false);
				if (!boards[i].white_in_checkmate(checkmating_moves)) {
					return moves[i];
				}
				checkmating_moves.clear();
				checkmating_boards.clear();
				std::cout << "White in checkmate\n";
			}
		} else {
			board.get_black_moves(moves, boards, false);
			std::vector<move::Move> checkmating_moves;
			std::vector<Board> checkmating_boards;
			for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
				boards[i].get_white_moves(checkmating_moves, checkmating_boards, false);
				if (!boards[i].black_in_checkmate(checkmating_moves)) {
					return moves[i];
				}
				checkmating_moves.clear();
				checkmating_boards.clear();
			}
			std::cout << "Black in checkmate\n";
		}
		std::cout << "No move\n";
		return std::nullopt;	
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

}
