#include "alpha_beta.hpp"
#include "eval.hpp"
#include <ranges>
#include <chrono>

namespace alpha_beta {

int alpha_beta(const Board& node, int depth, bool maximizing_player, move::Move& best_move, const int start_depth, std::stop_token& stop_token, int alpha, int beta) {
	//if (stop_token.stop_requested()) {
	//	return 0;
	//}

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
		for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
			const int new_evaluation = alpha_beta(boards[i], depth - 1, !maximizing_player, best_move, start_depth, stop_token, alpha, beta);
			if (evaluation < new_evaluation) {
				evaluation = new_evaluation;
				if (depth == start_depth) {
					best_move = moves[i];
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
		for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
			const int new_evaluation = alpha_beta(boards[i], depth - 1, !maximizing_player, best_move, start_depth, stop_token, alpha, beta);
			if (evaluation > new_evaluation) {
				evaluation = new_evaluation;
				if (depth == start_depth) {
					best_move = moves[i];
				}
			}
		}
		return evaluation;
	}
}

int alpha_beta_with_timeout(const Board& board, int depth, bool maximizing_player, move::Move& best_move, int timeout_ms) {
	//std::jthread alpha_beta_thread([&board, depth, maximizing_player, &best_move](std::stop_token stop_token) {
	std::stop_token stop_token;
	return alpha_beta(board, depth, maximizing_player, best_move, depth, stop_token);
	//});

	//std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
	//std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
	//alpha_beta_thread.request_stop();
	return 0;
}

}
