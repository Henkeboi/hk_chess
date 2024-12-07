#include "search.hpp"
#include "eval.hpp"
#include "zobrist.hpp"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <limits.h>
#include "robin_map/robin_map.h"

namespace {
	SearchStatistics::SearchStatistics()
	: timeout_exceeded(false), evaluation(0), positions_evaluated(0), positions_generated(0) {

	}

	int alpha_beta(const Board& node, int depth, bool maximizing_player, move::Move& best_move, const int start_depth, SearchStatistics& search_statistics, tsl::robin_map<uint64_t, int>& transposition_table, tsl::robin_map<uint64_t, uint8_t> position_counter, uint64_t node_zobrist_hash, const Zobrist& zobrist_hasher, int alpha=INT_MIN, int beta=INT_MAX);

	std::optional<move::Move> get_random_move_if_not_checkmate(const Board& board, bool maximizing_player);
}

namespace search {

	Search::Search(const Board& board, bool maximizing_player) : _board(board), 
		_zobrist_hasher{board, maximizing_player}, _last_zobrist_hash(_zobrist_hasher.get_initial_zobrist_hash()), 
		_position_repeat_counter(tsl::robin_map<uint64_t, uint8_t>{}), _white_search_depth(5), _black_search_depth(5) {
	}
	
	move::Move Search::find_next_move(const Board& board, bool maximizing_player) {
		auto search_start = std::chrono::high_resolution_clock::now();
		move::Move best_move{0, 0, 0, 0};
		if (maximizing_player) {
			search(board, _white_search_depth, maximizing_player, best_move, _last_zobrist_hash, _zobrist_hasher, _position_repeat_counter);
		} else {
			search(board, _black_search_depth, maximizing_player, best_move, _last_zobrist_hash, _zobrist_hasher, _position_repeat_counter);
		}

		Board new_board = Board{board, best_move};
    _last_zobrist_hash = _zobrist_hasher.new_zobrist_hash(new_board, board, best_move, _last_zobrist_hash);
		auto search_stop = std::chrono::high_resolution_clock::now();
		auto search_execution_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(search_stop - search_start);
		if (search_execution_time_ms.count() < 1000) {
			if (maximizing_player) {
				_white_search_depth += 1;
			} else {
				_black_search_depth += 1;
			}
		} else if (search_execution_time_ms.count() > 3000) {
			if (maximizing_player) {
				if (_white_search_depth > 4000)
					_white_search_depth -= 1;
			} else {
				if (_black_search_depth > 4000)
					_black_search_depth -= 1;
			}
		}

		if (maximizing_player) {
			std::cout << "White search depth " << _white_search_depth << "\n";
			std::cout << "White execution time " << search_execution_time_ms.count() << "\n";
		} else {
				std::cout << "Black search depth " << _black_search_depth << "\n";
				std::cout << "Black execution time " << search_execution_time_ms.count() << "\n";
		}
		return best_move;
	}
	
	void search(const Board& board, int depth, bool maximizing_player, move::Move& best_move, uint64_t board_zobrist_hash, Zobrist& zobrist_hasher, tsl::robin_map<uint64_t, uint8_t> position_repeat_counter) {
		SearchStatistics search_statistics{};
		tsl::robin_map<uint64_t, int> transposition_table{};
		alpha_beta(board, depth, maximizing_player, best_move, depth, search_statistics, transposition_table, position_repeat_counter, board_zobrist_hash, zobrist_hasher);

		if (best_move.get_to_row() == best_move.get_from_row()) {
  		if (best_move.get_to_col() == best_move.get_from_col()) {
				std::optional<move::Move> random_move = get_random_move_if_not_checkmate(board, maximizing_player);
				if (random_move.has_value()) {
					std::cout << "Random move\n";
					best_move = random_move.value();
				} else {	
					exit(0);
				}
	  	}
		}

		if (board.get_raw_board()[best_move.get_from_row()][best_move.get_from_col()] == pieces::empty) {
			std::optional<move::Move> random_move = get_random_move_if_not_checkmate(board, maximizing_player);
			if (random_move.has_value()) {
				std::cout << "Random move\n";
				best_move = random_move.value();
			} else {	
				exit(0);
			}
		}

		std::cout << "Positions generated: " << search_statistics.positions_generated << "\n";
		std::cout << "Positions evaluated: " << search_statistics.positions_evaluated << "\n";
		std::cout << "Evaluation: " << search_statistics.evaluation << "\n";
	}
}

namespace {
	void increment_zobrist_hash_counter(tsl::robin_map<uint64_t, uint8_t>& position_counter, uint64_t zobrist_hash) {
		if (position_counter.find(zobrist_hash) == position_counter.end()) {
			position_counter[zobrist_hash] = 1;
		} else {
			position_counter[zobrist_hash] += 1;
		}
	}

	void decrement_zobrist_hash_counter(tsl::robin_map<uint64_t, uint8_t>& position_counter, uint64_t zobrist_hash) {
		position_counter[zobrist_hash] -= 1;
		if (position_counter[zobrist_hash] == 0) {
			auto erase_iterator = position_counter[zobrist_hash];
			position_counter.erase(erase_iterator);
		}
	}

	int alpha_beta(const Board& node, int depth, bool maximizing_player, move::Move& best_move, const int start_depth, SearchStatistics& search_statistics, tsl::robin_map<uint64_t, int>& transposition_table, tsl::robin_map<uint64_t, uint8_t> position_counter, uint64_t node_hash, const Zobrist& zobrist_hasher, int alpha, int beta) {
		if (maximizing_player) {
			std::vector<move::Move> moves;
			std::vector<Board> boards;
			if (depth > 0) {
				node.get_white_moves(moves, boards, false);
			} else {
				node.get_white_moves(moves, boards, true);
			}

			if (node.black_in_checkmate(moves)) {
				search_statistics.positions_evaluated += 1;
				return INT_MAX;
			}

			if (moves.size() == 0) {
				//if (depth < 0) {
					search_statistics.positions_evaluated += 1;
					return eval::eval(node);
				/*} else {
					return 0; // Stalemate
				}*/
			}
			
			int evaluation = INT_MIN;
			for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
				search_statistics.positions_generated += 1;	
				int new_evaluation = 0;
				const uint64_t new_zobrist_hash = zobrist_hasher.new_zobrist_hash(boards[i], node, moves[i], node_hash);
				increment_zobrist_hash_counter(position_counter, new_zobrist_hash);
				if (position_counter[new_zobrist_hash] == 3) {
					return 0;
				}
				if (transposition_table.find(new_zobrist_hash) == transposition_table.end()) {
					new_evaluation = alpha_beta(boards[i], depth - 1, !maximizing_player, best_move, start_depth, search_statistics, transposition_table, position_counter, new_zobrist_hash, zobrist_hasher, alpha, beta);
				} else {
					new_evaluation = transposition_table[new_zobrist_hash];
				}
				decrement_zobrist_hash_counter(position_counter, new_zobrist_hash);
				if (evaluation < new_evaluation) {
					evaluation = new_evaluation;
					if (depth == start_depth) {
						best_move = moves[i];
						search_statistics.evaluation = evaluation;
					}
					if (evaluation > beta) {
						break;
					}
					if (transposition_table.find(new_zobrist_hash) == transposition_table.end()) {
						transposition_table[new_zobrist_hash] = new_evaluation;
					} 
					alpha = std::max(alpha, evaluation);
				} 
			}
			return evaluation;
		} else {
			std::vector<move::Move> moves;
			std::vector<Board> boards;
			if (depth > 0) {
				node.get_black_moves(moves, boards, false);
			} else {
				node.get_black_moves(moves, boards, true);
			}

			if (node.white_in_checkmate(moves)) {
				search_statistics.positions_evaluated += 1;
				return INT_MIN;
			}

			if (moves.size() == 0) {
				//if (depth < 0) {
				search_statistics.positions_evaluated += 1;
				return eval::eval(node);
				/*} else {
					return 0; // Stalemate
				}*/
			}

			int evaluation = INT_MAX;
			for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
				search_statistics.positions_generated += 1;	
				int new_evaluation = 0;
				const uint64_t new_zobrist_hash = zobrist_hasher.new_zobrist_hash(boards[i], node, moves[i], node_hash);
				increment_zobrist_hash_counter(position_counter, new_zobrist_hash);
				if (position_counter[new_zobrist_hash] == 3) {
					return 0;
				}
				if (transposition_table.find(new_zobrist_hash) == transposition_table.end()) {
					new_evaluation = alpha_beta(boards[i], depth - 1, !maximizing_player, best_move, start_depth, search_statistics, transposition_table, position_counter, new_zobrist_hash, zobrist_hasher, alpha, beta);
				} else {
					new_evaluation = transposition_table[new_zobrist_hash];
				}
				decrement_zobrist_hash_counter(position_counter, new_zobrist_hash);
				if (evaluation > new_evaluation) {
					evaluation = new_evaluation;
					if (depth == start_depth) {
						best_move = moves[i];
						search_statistics.evaluation = evaluation;
					}
					if (evaluation < alpha) {
						break;
					}
					if (transposition_table.find(new_zobrist_hash) == transposition_table.end()) {
						transposition_table[new_zobrist_hash] = new_evaluation;
					} 
					beta = std::min(beta, evaluation);
				}
			}
			return evaluation;
		}
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
