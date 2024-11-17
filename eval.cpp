#include "eval.hpp"

#include <print>

namespace eval {

static const std::array<std::array<int, 8>, 8> white_pawn_activity_values = {{
	{0,  0,  0,   0,    0,   0,  0,  0},
	{5,  10, 10,  -20,  -20, 10, 10, 5},
	{5,  -5, -10, 0,    0,  -10, -5, 5},
	{0,  0,  0,   20,   20, 0,   0,  0},
	{5,  5,  10,  25,   25, 10,  5,  5},
 	{10, 10, 20,  30,   30, 20,  10, 10},
	{50, 50, 50,  50,   50, 50,  50, 50},
	{0,  0,  0,   0,    0,  0,   0,  0}
}};

static const std::array<std::array<int, 8>, 8> black_pawn_activity_values = {{
	{-0,  -0,  -0,   -0,   -0,  -0,  0,  0},
	{-5,  -10, -10,  20,   20,  -10, 10, 5},
	{-5,  5,   10,   -0,   -0,  10, -5, 5},
	{-0,  -0,  -0,   -20,  -20, -0,  0,  0},
	{-5,  -5,  -10,  -25,  -25, -10, 5,  5},
 	{-10, -10, -20,  -30,  -30, -20, 10, 10},
	{-50, -50, -50,  -50,  -50, -50, 50, 50},
	{-0,  -0,  -0,   -0,   -0,  -0,  0,  0}
}};

int eval(const Board& board) {
	int eval = 0;
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			pieces::piece piece = board.get_raw_board()[row][col];
			if (piece == (pieces::black | pieces::pawn)) {
				eval -= pieces::pawn_value;
				eval -= black_pawn_activity_values[row][col];
			} else if (piece == (pieces::black | pieces::knight)) {
				eval -= pieces::knight_value;
			} else if (piece == (pieces::black | pieces::bishop)) {
				eval -= pieces::bishop_value;
			} else if (piece == (pieces::black | pieces::rook)) {
				eval -= pieces::rook_value;
			} else if (piece == (pieces::black | pieces::queen)) {
				eval -= pieces::queen_value;
			} else if (piece == (pieces::black | pieces::king)) {
				eval -= pieces::king_value; 
			} else if (piece == pieces::pawn) {
				eval += pieces::pawn_value;
				eval += white_pawn_activity_values[row][col];
			} else if (piece == pieces::knight) {
				eval += pieces::knight_value;
			} else if (piece == pieces::bishop) {
				eval += pieces::bishop_value;
			} else if (piece == pieces::rook) {
				eval += pieces::rook_value;
			} else if (piece == pieces::queen) {
				eval += pieces::queen_value;
			} else if (piece == pieces::king) {
				eval += pieces::king_value;
			}
		}
	}
	return eval;
}
}
