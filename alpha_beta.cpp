#include "alpha_beta.hpp"
#include "move.hpp"
#include "eval.hpp"
#include <ranges>

int alpha_beta::alpha_beta(const Board& node, int depth, bool maximizing_player, move::Move& best_move, const int start_depth, int alpha, int beta) {
	if (maximizing_player) {
		std::vector<move::Move> moves;
		std::vector<Board> boards;
		if (depth > 0) {
			node.get_white_moves(moves, boards, false);
		} else {
			node.get_white_moves(moves, boards, true);
		}

		if (node.black_in_checkmate(moves)) {
			return INT_MAX;
		}

		if (moves.size() == 0) {
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
			return INT_MIN;
		}

		if (moves.size() == 0) {
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
		}
		return evaluation;
	}
}
