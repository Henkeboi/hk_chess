#include "search.hpp"
#include "eval.hpp"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <thread>
#include <limits>
#include <set>
#include "assert.h"

namespace {
	SearchStatistics::SearchStatistics()
	: timeout_exceeded(false), positions_evaluated(0), positions_generated(0), positions_pruned(0) {
	}
	typedef struct TranspositionData {
		int evaluation;
		int depth;
		std::vector<Move> variation;
		bool operator<(const TranspositionData& rhs) const {
			return this->evaluation < rhs.evaluation;
		}
	} transposition_data;


	std::pair<int, Move> alpha_beta(const Board& node, int depth, bool maximizing_player, Zobrist& zobrist_hasher,
		tsl::robin_map<uint64_t, transposition_data>& transposition_table, tsl::robin_map<uint64_t, uint8_t>& position_repeat_tracker,
		uint64_t node_hash, Move& best_move, const int start_depth,
		SearchStatistics& search_statistics, int alpha=INT_MIN, int beta=INT_MAX);
	bool is_threefold_repetition(tsl::robin_map<uint64_t, uint8_t>& position_repeat_tracker, uint64_t zobrist_hash);
	void decrement_position_repeat_tracker(tsl::robin_map<uint64_t, uint8_t>& position_repeat_tracker, uint64_t zobrist_hash);
	std::tuple<Move, bool, bool> get_random_move(const Board& board, bool maximizing_player);
	bool stop_search(tsl::robin_map<uint64_t, transposition_data>& transposition_table, uint64_t zobrist_hash, int depth);
}

namespace search {
	std::tuple<Move, bool, bool> search(const Board board, int depth, bool maximizing_player, Zobrist zobrist_hasher,
	tsl::robin_map<uint64_t, uint8_t> position_repeat_tracker, uint64_t zobrist_hash, Move& best_move) {
		bool white_in_checkmate = false;
		bool black_in_checkmate = false;
		std::tie(best_move, white_in_checkmate, black_in_checkmate) = get_random_move(board, maximizing_player);
		if (white_in_checkmate || black_in_checkmate) {
			return std::make_tuple(best_move, white_in_checkmate, black_in_checkmate);
		}
	
		SearchStatistics search_statistics{};
		tsl::robin_map<uint64_t, transposition_data> transposition_table{};
		auto search_start_time = std::chrono::high_resolution_clock::now();
		int evaluation = 0;
		Move test_move{0, 0, 0, 0};
		std::tie(evaluation, test_move) = alpha_beta(board, depth, maximizing_player, zobrist_hasher, transposition_table,
										position_repeat_tracker, zobrist_hash, best_move,
										depth, search_statistics);

		//std::set<transposition_data> sorted_table;
		//for (const auto& pair : transposition_table) {
		//	sorted_table.insert(pair.second);
		//}
		
		//transposition_data position_data;
		//position_data.evaluation = evaluation;
		//for (auto& move : sorted_table.lower_bound(position_data)->variation) {
		//	std::cout << test_move.get_move_as_string() << "\n";
		//}

		auto search_end_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> search_interval = search_end_time - search_start_time;
		std::cout << "K-NPS: " <<  static_cast<int>(static_cast<double>(search_statistics.positions_evaluated) / search_interval.count()) << "\n";
		std::cout << "Evaluation: " << evaluation << "\n";
		std::cout << "Positions pruned: " << search_statistics.positions_pruned << "\n";

		if ((evaluation < -20000 && maximizing_player) || (evaluation > 20000 && !maximizing_player)
			|| (board.get_raw_board()[best_move.get_from_row()][best_move.get_from_col()] == pieces::empty)) {
			std::cout << "Random move\n";
			std::tie(best_move, white_in_checkmate, black_in_checkmate) = get_random_move(board,  maximizing_player);
		}
	
		return std::make_tuple(best_move, white_in_checkmate, black_in_checkmate);
	}
}

namespace {
std::pair<int, Move> alpha_beta(const Board& node, int depth, bool maximizing_player, Zobrist& zobrist_hasher, 
tsl::robin_map<uint64_t, transposition_data>& transposition_table, tsl::robin_map<uint64_t, uint8_t>& position_repeat_tracker,
uint64_t node_hash, Move& best_move, const int start_depth, SearchStatistics& search_statistics, int alpha, int beta) {
	Move local_best_move{0, 0, 0, 0};
 	std::vector<Move> moves;
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
			search_statistics.positions_evaluated += 1;
			return std::make_pair(0, Move{0, 0, 0, 0}); // Stalemate
		}

		bool is_checkmate;
		if (maximizing_player) {
			std::tie(is_checkmate, local_best_move) = node.black_in_checkmate(moves);
			if (is_checkmate) {
				search_statistics.positions_evaluated += 1;
				return std::make_pair(INT_MAX, local_best_move);
			}
		} else {
			std::tie(is_checkmate, local_best_move) = node.white_in_checkmate(moves);
			if (is_checkmate) {
				search_statistics.positions_evaluated += 1;
				return std::make_pair(INT_MIN, local_best_move);
			}
		}
	} else if (moves.size() == 0) {
		search_statistics.positions_evaluated += 1;
		return std::make_pair(eval::eval(node), Move{0, 0, 0, 0}); // Todo
	}

	if (maximizing_player) {
		int eval = INT_MIN;
		for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
			search_statistics.positions_generated += 1;	
			uint64_t zobrist_hash = zobrist_hasher.new_zobrist_hash(boards[i], node, moves[i], node_hash);
			int new_eval = 0;
			if (!is_threefold_repetition(position_repeat_tracker, zobrist_hash)) {
				bool position_already_evaluated = stop_search(transposition_table, zobrist_hash, start_depth - depth);
				if (position_already_evaluated) {
					search_statistics.positions_pruned += 1;
					new_eval = transposition_table[zobrist_hash].evaluation;
				} else {	
					std::tie(new_eval, local_best_move) = alpha_beta(boards[i], depth - 1, !maximizing_player, zobrist_hasher,
										transposition_table, position_repeat_tracker, zobrist_hash, best_move,
										start_depth, search_statistics, alpha, beta);
					transposition_data position_data;
					position_data.evaluation = new_eval;
					position_data.depth = start_depth - depth;
					position_data.variation.push_back(local_best_move);
					transposition_table[zobrist_hash] = position_data;
				}
			}
			decrement_position_repeat_tracker(position_repeat_tracker, zobrist_hash);
			if (eval < new_eval) {
				eval = new_eval;
				local_best_move = moves[i];
				if (depth == start_depth) {
					best_move = moves[i];
				}
				if (eval > beta) {
					search_statistics.positions_pruned += 1;
					break;
				}
				alpha = std::max(alpha, eval);
			} 
		}
		return std::make_pair(eval, local_best_move);
	} else {
		int eval = INT_MAX;
		for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
			search_statistics.positions_generated += 1;	
			uint64_t zobrist_hash = zobrist_hasher.new_zobrist_hash(boards[i], node, moves[i], node_hash);
			int new_eval = 0;
			if (!is_threefold_repetition(position_repeat_tracker, zobrist_hash)) {
				bool position_already_evaluated = stop_search(transposition_table, zobrist_hash, start_depth - depth);
				if (position_already_evaluated) {
					search_statistics.positions_pruned += 1;
					new_eval = transposition_table[zobrist_hash].evaluation;
				} else {
					std::tie(new_eval, local_best_move) = alpha_beta(boards[i], depth - 1, !maximizing_player, zobrist_hasher,
										transposition_table, position_repeat_tracker, zobrist_hash, best_move,
										start_depth, search_statistics, alpha, beta);
					transposition_data position_data;
					position_data.evaluation = new_eval;
					position_data.depth = start_depth - depth;
					position_data.variation.push_back(local_best_move);
					transposition_table[zobrist_hash] = position_data;
				}
			}
			decrement_position_repeat_tracker(position_repeat_tracker, zobrist_hash);
			if (eval > new_eval) {
				eval = new_eval;
				local_best_move = moves[i];
				if (depth == start_depth) {
					best_move = moves[i];
				}
				if (eval < alpha) {
					search_statistics.positions_pruned += 1;
					break;
				}
				beta = std::min(beta, eval);
			}
		}
		return std::make_pair(eval, local_best_move);
	}
}

bool stop_search(tsl::robin_map<uint64_t, transposition_data>& transposition_table, uint64_t zobrist_hash, int depth) {
	if (transposition_table.find(zobrist_hash) == transposition_table.end()) 
		return false;
	if (transposition_table[zobrist_hash].depth <= depth)
		return true;
	return false;
}

void decrement_position_repeat_tracker(tsl::robin_map<uint64_t, uint8_t>& position_repeat_tracker, uint64_t zobrist_hash) {
	if (position_repeat_tracker[zobrist_hash] == 1) {
		position_repeat_tracker.erase(position_repeat_tracker.find(zobrist_hash));
	} else {
		position_repeat_tracker[zobrist_hash] -= 1;
	}
}

bool is_threefold_repetition(tsl::robin_map<uint64_t, uint8_t>& position_repeat_tracker, uint64_t zobrist_hash) {
	if (position_repeat_tracker.find(zobrist_hash) == position_repeat_tracker.end()) {
		position_repeat_tracker[zobrist_hash] = 1;
	} else {
		position_repeat_tracker[zobrist_hash] += 1;
		if (position_repeat_tracker[zobrist_hash] == 3) {
			return true;
		}
	}
	return false;
}

std::tuple<Move, bool, bool> get_random_move(const Board& board, bool maximizing_player) {
	bool white_in_checkmate = false;
	bool black_in_checkmate = false;
	if (maximizing_player) {
		std::vector<Move> moves;
		std::vector<Board> boards;
		board.get_white_moves(moves, boards, false);
		for (size_t i = 0; i < boards.size(); ++i) {
			std::vector<Move> black_moves;
			std::vector<Board> black_boards;
			boards[i].get_black_moves(black_moves, black_boards, false);
			Move checkmating_move{0, 0, 0, 0};
			std::tie(white_in_checkmate, checkmating_move) = boards[i].white_in_checkmate(black_moves);
			if (!white_in_checkmate) {
				return std::make_tuple(moves[i], white_in_checkmate, black_in_checkmate);
			}
		}
	}	else {
		std::vector<Move> moves;
		std::vector<Board> boards;
		board.get_black_moves(moves, boards, false);
		for (size_t i = 0; i < boards.size(); ++i) {
			std::vector<Move> white_moves;
			std::vector<Board> white_boards;
			boards[i].get_white_moves(white_moves, white_boards, false);
			Move checkmating_move{0, 0, 0, 0};
			std::tie(black_in_checkmate, checkmating_move) = boards[i].black_in_checkmate(white_moves);
			if (!black_in_checkmate) {
				return std::make_tuple(moves[i], white_in_checkmate, black_in_checkmate);
			}
		}
	}
	if (maximizing_player) {
		white_in_checkmate = true;
		return std::make_tuple(Move{0, 0, 0, 0}, white_in_checkmate, black_in_checkmate);
	} else {
		black_in_checkmate = true;
		return std::make_tuple(Move{0, 0, 0, 0}, white_in_checkmate, black_in_checkmate);
	}
}
}