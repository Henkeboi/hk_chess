#include "search.hpp"
#include "eval.hpp"
#include <chrono>
#include <algorithm>
#include <iostream>
#include <thread>
#include <limits>


namespace {
	SearchStatistics::SearchStatistics()
	: timeout_exceeded(false), positions_evaluated(0), positions_generated(0) {
	}

	int alpha_beta(const Board& node, int depth, bool maximizing_player, move::Move& best_move, const int start_depth, SearchStatistics& search_statistics, int alpha=INT_MIN, int beta=INT_MAX);
}

namespace search {

	void search(const Board& board, int depth, bool maximizing_player, std::map<uint64_t, uint8_t> position_repeat_counter, move::Move& best_move) {
		SearchStatistics search_statistics{};
		int evaluation = alpha_beta(board, depth, maximizing_player, best_move, depth, search_statistics);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		std::cout << "Positions generated: " << search_statistics.positions_generated << "\n";
		std::cout << "Positions evaluated: " << search_statistics.positions_evaluated << "\n";
		std::cout << "Evaluation: " << evaluation << "\n";
	}
}

namespace {
	int alpha_beta(const Board& node, int depth, bool maximizing_player, move::Move& best_move, const int start_depth, SearchStatistics& search_statistics, int alpha, int beta) {
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
				search_statistics.positions_evaluated += 1;
				return eval::eval(node);
			}
			
			int evaluation = INT_MIN;
			for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
				search_statistics.positions_generated += 1;	
				const int new_evaluation = alpha_beta(boards[i], depth - 1, !maximizing_player, best_move, start_depth, search_statistics, alpha, beta);
				if (evaluation < new_evaluation) {
					evaluation = new_evaluation;
					if (depth == start_depth) {
						best_move = moves[i];
					}
					if (evaluation > beta) {
						break;
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
				search_statistics.positions_evaluated += 1;
				return eval::eval(node);
			}

			int evaluation = INT_MAX;
			for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
				search_statistics.positions_generated += 1;	
				const int new_evaluation = alpha_beta(boards[i], depth - 1, !maximizing_player, best_move, start_depth, search_statistics, alpha, beta);
				if (evaluation > new_evaluation) {
					evaluation = new_evaluation;
					if (depth == start_depth) {
						best_move = moves[i];
					}
					if (evaluation < alpha) {
						break;
					}
					beta = std::min(beta, evaluation);
				}
			}
			return evaluation;
		}
	}
}
