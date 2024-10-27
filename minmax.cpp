#include "minmax.hpp"
#include "move.hpp"
#include <limits>
#include <ranges>
#include <vector>
#include <print>
#include "eval.hpp"

float minmax::minmax(const Board& node, int depth, bool maximizing_player) {
	if (depth == 0) {
		return eval::eval(node.get_board());
	}
	if (maximizing_player) {
		std::vector<move::Move> moves;
		std::vector<Board> boards;
		node.get_black_moves(moves, boards);
		if (node.white_in_checkmate(moves)) {
			return -std::numeric_limits<float>::max();
		}

		float evaluation = -std::numeric_limits<float>::max();
		auto zipped = std::views::zip(moves, boards);
		for (auto [move, board] : zipped) {
			evaluation = std::max(evaluation, minmax(board, depth - 1, false));
		}
		return evaluation;
	} else {
		std::vector<move::Move> moves;
		std::vector<Board> boards;
		node.get_white_moves(moves, boards);
		if (node.black_in_checkmate(moves)) {
			return std::numeric_limits<float>::max();
		}
	
		float evaluation = std::numeric_limits<float>::max();
		auto zipped = std::views::zip(moves, boards);
		for (auto [move, board] : zipped) {
			evaluation = std::min(evaluation, minmax(board, depth - 1, true));
		}
		return evaluation;
	}
}
