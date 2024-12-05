#include "eval.hpp"
#include <utility>

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
 { 0,  0,  5,  10, 10, 5,  0,  0},
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
 { 0,  0,  5,  10, 10, 5,  0,  0}
}};

static const std::array<std::array<int, 8>, 8> white_queen_activity_values = {{
	{-20, -10, -10, -5, -5, -10, -10, -20},
	{-10,  0,   0,   0,  0,   0,   0, -10},
	{-10,  0,   5,   5,  5,   5,   0, -10},
	{ -5,  0,   5,   5,  5,   5,   0,  -5},
	{	0,   0,   5,   5,  5,   5,   0,  -5},
	{-10,  5,   5,   5,  5,   5,   0, -10},
	{-10,  0,   5,   0,  0,   0,   0, -10},
	{-20, -10, -10, -5, -5, -10, -10, -20}
}};

static const std::array<std::array<int, 8>, 8> black_queen_activity_values = {{
	{-20, -10, -10, -5, -5, -10, -10, -20},
	{-10,  0,   0,   0,  0,   0,   0, -10},
	{-10,  0,   5,   5,  5,   5,   0, -10},
	{ -5,  0,   5,   5,  5,   5,   0,  -5},
	{	0,   0,   5,   5,  5,   5,   0,  -5},
	{-10,  5,   5,   5,  5,   5,   0, -10},
	{-10,  0,   5,   0,  0,   0,   0, -10},
	{-20, -10, -10, -5, -5, -10, -10, -20}
}};

static const std::array<std::array<int, 8>, 8> white_king_activity_values_middle_game = {{
	{ 20,  30,  10,   0,   0,  10,  30,  20},
	{ 20,  20,   0,   0,   0,   0,  20,  20},
	{-10, -20, -20, -20, -20, -20, -20, -10},
	{-20, -30, -30, -40, -40, -30, -30, -20},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30}
}};

static const std::array<std::array<int, 8>, 8> black_king_activity_values_middle_game = {{
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-30, -40, -40, -50, -50, -40, -40, -30},
	{-20, -30, -30, -40, -40, -30, -30, -20},
	{-10, -20, -20, -20, -20, -20, -20, -10},
	{ 20,  20,   0,   0,   0,   0,  20,  20},
	{ 20,  30,  10,   0,   0,  10,  30,  20}
}};

static const std::array<std::array<int, 8>, 8> white_king_activity_values_end_game = {{
	{-50,-30,-30,-30,-30,-30,-30,-50},
	{-50,-40,-30,-20,-20,-30,-40,-50},
	{-30,-20,-10,  0,  0,-10,-20,-30},
	{-30,-10, 20, 30, 30, 20,-10,-30},
	{-30,-10, 30, 40, 40, 30,-10,-30},
	{-30,-10, 30, 40, 40, 30,-10,-30},
	{-30,-10, 20, 30, 30, 20,-10,-30},
	{-30,-30,  0,  0,  0,  0,-30,-30},
}};

static const std::array<std::array<int, 8>, 8> black_king_activity_values_end_game = {{
	{-50,-40,-30,-20,-20,-30,-40,-50},
	{-30,-20,-10,  0,  0,-10,-20,-30},
	{-30,-10, 20, 30, 30, 20,-10,-30},
	{-30,-10, 30, 40, 40, 30,-10,-30},
	{-30,-10, 30, 40, 40, 30,-10,-30},
	{-30,-10, 20, 30, 30, 20,-10,-30},
	{-30,-30,  0,  0,  0,  0,-30,-30},
	{-50,-30,-30,-30,-30,-30,-30,-50}
}};

#include <iostream>

namespace eval {
	std::pair<int, int> decide_game_stage(const Board& board);

	int eval(const Board& board) {
		auto [white_game_stage, black_game_stage] = decide_game_stage(board);

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
					eval -= black_queen_activity_values[row][col];
				} else if (piece == (pieces::black | pieces::king)) {
					eval -= pieces::king_value; 
					if (white_game_stage == 1) {
						eval -= black_king_activity_values_middle_game[row][col]; 
					} else {
						eval -= black_king_activity_values_end_game[row][col]; 
					}
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
					eval += white_queen_activity_values[row][col];
				} else if (piece == pieces::king) {
					eval += pieces::king_value;
					if (black_game_stage == 1) {
						eval += white_king_activity_values_middle_game[row][col];
					} else {
						eval += white_king_activity_values_end_game[row][col];
					}
				}
			}
		}
		return eval;
	}

	std::pair<int, int> decide_game_stage(const Board& board) { 
		int white_piece_value = 0;
		int black_piece_value = 0;

		for (int row = 0; row < 8; ++row) {
			for (int col = 0; col < 8; ++col) {
				pieces::piece piece = board.get_raw_board()[row][col];
				if (piece == (pieces::black | pieces::knight)) {
					black_piece_value += pieces::knight_value;
				} else if (piece == (pieces::black | pieces::bishop)) {
					black_piece_value += pieces::bishop_value;
				} else if (piece == (pieces::black | pieces::rook)) {
					black_piece_value += pieces::rook_value;
				} else if (piece == (pieces::black | pieces::queen)) {
					black_piece_value += pieces::queen_value;
				} else if (piece == pieces::knight) {
					white_piece_value += pieces::knight_value;
				} else if (piece == pieces::bishop) {
					white_piece_value += pieces::bishop_value;
				} else if (piece == pieces::rook) {
					white_piece_value += pieces::rook_value;
				} else if (piece == pieces::queen) {
					white_piece_value += pieces::queen_value;
				}
			}
		}
		int white_game_stage = 0;
		int black_game_stage = 0;

		if (white_piece_value < 10) {
			white_game_stage = 2;
		} else {
			white_game_stage = 1;
		}
		if (black_piece_value < 10) {
			black_game_stage = 2;
		} else {
			black_game_stage = 1;
		}
		return std::make_pair(white_game_stage, black_game_stage);
	}
}
