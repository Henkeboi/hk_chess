#include "eval.hpp"

static const std::array<std::array<int, 8>, 8> white_pawn_activity_values = {{
	{0,  0,  0,   0,   0,   0,  0,  0},
	{5,  10, 10, -20, -20, 10,  10, 5},
	{5, -5, -10,  0,   0, -10, -5,  5},
	{0,  0,  0,   20,  20, 0,   0,  0},
	{5,  5,  10,  25,  25, 10,  5,  5},
 	{10, 10, 20,  30,  30, 20,  10, 10},
	{50, 50, 50,  50,  50, 50,  50, 50},
	{0,  0,  0,   0,   0,  0,   0,  0}
}};

static const std::array<std::array<int, 8>, 8> black_pawn_activity_values = {{
	{0,  0,  0,   0,   0,  0,   0,  0},
	{50, 50, 50,  50,  50, 50,  50, 50},
 	{10, 10, 20,  30,  30, 20,  10, 10},
	{5,  5,  10,  25,  25, 10,  5,  5},
	{0,  0,  0,   20,  20, 0,   0,  0},
	{5, -5, -10,  0,   0, -10, -5,  5},
	{5,  10, 10, -20, -20, 10,  10, 5},
	{0,  0,  0,   0,   0,  0,   0,  0}
}};

static const std::array<std::array<int, 8>, 8> white_knight_activity_values = {{
	{-50, -40, -30, -30, -30, -30, -40, -50},
	{-40, -20,   0,   5,   5,   0, -20, -40},
	{-30,   5,  10,  15,  15,  10,   5, -30},
	{-30,   0,  15,  20,  20,  15,   0, -30},
	{-30,   5,  15,  20,  20,  15,   5, -30},
	{-30,   0,  10,  15,  15,  10,   0, -30},
	{-40, -20,   0,   0,   0,   0, -20, -40},
	{-50, -40, -30, -30, -30, -30, -40, -50},
}};

static const std::array<std::array<int, 8>, 8> black_knight_activity_values = {{
	{-50, -40, -30, -30, -30, -30, -40, -50},
	{-40, -20,   0,   0,   0,   0, -20, -40},
	{-30,   0,  10,  15,  15,  10,   0, -30},
	{-30,   5,  15,  20,  20,  15,   5, -30},
	{-30,   0,  15,  20,  20,  15,   0, -30},
	{-30,   5,  10,  15,  15,  10,   5, -30},
	{-40, -20,   0,   5,   5,   0, -20, -40},
	{-50, -40, -30, -30, -30, -30, -40, -50}
}};

static const std::array<std::array<int, 8>, 8> white_bishop_activity_values = {{
	{-20, -10, -10, -10, -10, -10, -10, -20},
	{-10,   5,   0,   0,   0,   0,   5, -10},
	{-10,  10,  10,  10,  10,  10,  10, -10},
	{-10,   0,  10,  10,  10,  10,   0, -10},
	{-10,   5,   5,  10,  10,   5,   5, -10},
	{-10,   0,   5,  10,  10,   5,   0, -10},
	{-10,   0,   0,   0,   0,   0,   0, -10},
	{-20, -10, -10, -10, -10, -10, -10, -20}
}};

static const std::array<std::array<int, 8>, 8> black_bishop_activity_values = {{
	{-20, -10, -10, -10, -10, -10, -10, -20},
	{-10,   0,   0,   0,   0,   0,   0, -10},
	{-10,   0,   5,  10,  10,   5,   0, -10},
	{-10,   5,   5,  10,  10,   5,   5, -10},
	{-10,   0,  10,  10,  10,  10,   0, -10},
	{-10,  10,  10,  10,  10,  10,  10, -10},
	{-10,   5,   0,   0,   0,   0,   5, -10},
	{-20, -10, -10, -10, -10, -10, -10, -20}
}};

static const std::array<std::array<int, 8>, 8> white_rook_activity_values = {{
 { 0,  0,  0,  5,  5,  0,  0,  0},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 { 5, 10, 10, 10, 10, 10, 10,  5},
 { 0,  0,  0,  0,  0,  0,  0,  0}
}};

static const std::array<std::array<int, 8>, 8> black_rook_activity_values = {{
 { 0,  0,  0,  0,  0,  0,  0,  0},
 { 5, 10, 10, 10, 10, 10, 10,  5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 { 0,  0,  0,  5,  5,  0,  0,  0}
}};








static const std::array<std::array<int, 8>, 8> white_king_activity_values = {{
	{ 20,  30,  10,   0,   0,  10,  30,  20},
	{ 20,  20,   0,   0,   0,   0,  20,  20},
	{-10, -20, -20, -20, -20, -20, -20, -10},
	{-20, -30, -30, -40, -40, -30, -30, -20},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30}
}};

static const std::array<std::array<int, 8>, 8> black_king_activity_values = {{
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-20, -30, -30, -40, -40, -30, -30, -20},
	{-10, -20, -20, -20, -20, -20, -20, -10},
	{ 20,  20,   0,   0,   0,   0,  20,  20},
	{ 20,  30,  10,   0,   0,  10,  30,  20}
}};


const int queen_side_castling_right_value = 20;
const int king_side_castling_right_value = 20;

namespace eval {

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
				eval -= black_knight_activity_values[row][col];
			} else if (piece == (pieces::black | pieces::bishop)) {
				eval -= pieces::bishop_value;
				eval -= black_bishop_activity_values[row][col];
			} else if (piece == (pieces::black | pieces::rook)) {
				eval -= pieces::rook_value;
				eval -= black_rook_activity_values[row][col];
			} else if (piece == (pieces::black | pieces::queen)) {
				eval -= pieces::queen_value;
			} else if (piece == (pieces::black | pieces::king)) {
				eval -= pieces::king_value; 
				eval -= black_king_activity_values[row][col]; 
			} else if (piece == pieces::pawn) {
				eval += pieces::pawn_value;
				eval += white_pawn_activity_values[row][col];
			} else if (piece == pieces::knight) {
				eval += pieces::knight_value;
				eval += white_knight_activity_values[row][col];
			} else if (piece == pieces::bishop) {
				eval += pieces::bishop_value;
				eval += white_bishop_activity_values[row][col];
			} else if (piece == pieces::rook) {
				eval += pieces::rook_value;
				eval += white_rook_activity_values[row][col];
			} else if (piece == pieces::queen) {
				eval += pieces::queen_value;
			} else if (piece == pieces::king) {
				eval += pieces::king_value;
				eval += white_king_activity_values[row][col];
			}
		}
	}
	
	if (board.can_white_castle_queen_side()) {
		eval += queen_side_castling_right_value;
	}
	if (board.can_white_castle_king_side()) {
		eval += king_side_castling_right_value;
	}
	if (board.can_black_castle_queen_side()) {
		eval -= queen_side_castling_right_value;
	}
	if (board.can_black_castle_king_side()) {
		eval -= king_side_castling_right_value;
	}

	return eval;
}

}
