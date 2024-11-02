#include "alpha_beta.hpp"
#include "move.hpp"
#include "eval.hpp"
#include <ranges>

int alpha_beta::alpha_beta(const Board& node, int depth, bool maximizing_player, move::Move& best_move, const int start_depth, int alpha, int beta, bool check_forcing_moves) {
	if (maximizing_player) {
		std::vector<move::Move> moves;
		std::vector<Board> boards;
		node.get_white_moves(moves, boards, false);

		if (node.black_in_checkmate(moves)) {
			return INT_MAX;
		}

		if (depth == 0) {
			return eval::eval(node);
		}

		int evaluation = INT_MIN;
		auto zipped = std::views::zip(moves, boards);
		for (auto [move, board] : zipped) {
			const int new_evaluation = alpha_beta(board, depth - 1, !maximizing_player, best_move, start_depth, alpha, beta);
			if (evaluation < new_evaluation) {
				evaluation = new_evaluation;
				if (depth == start_depth) {
					best_move = move;
				}
			} 
			if (evaluation > beta) {
				break;
			}
			if (evaluation > alpha) {
				alpha = evaluation;
			}
		}
		return evaluation;
	} else {
		std::vector<move::Move> moves;
		std::vector<Board> boards;
		node.get_black_moves(moves, boards, false);
		if (node.white_in_checkmate(moves)) {
			return INT_MIN;
		}

		if (depth == 0) {
			return eval::eval(node);
		}
	
		int evaluation = INT_MAX;
		auto zipped = std::views::zip(moves, boards);
		
		for (auto [move, board] : zipped) {
			const int new_evaluation = alpha_beta(board, depth - 1, !maximizing_player, best_move, start_depth, alpha, beta);
			if (evaluation > new_evaluation) {
				evaluation = new_evaluation;
				if (depth == start_depth) {
					best_move = move;
				}
			}
			if (evaluation < alpha) {
				break;
			}
			if (evaluation < beta) {
				beta = evaluation;
			}
		}
		return evaluation;
	}
}
