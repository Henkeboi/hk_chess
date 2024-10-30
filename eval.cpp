#include "eval.hpp"
#include "pieces.hpp"

#include <print>
#include <random>
#include <iostream>
#include <limits>


int eval::eval(const Board& board) {
	int eval = 0;
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			uint8_t piece = board.get_raw_board()[row][col];
			if (piece == (pieces::black | pieces::pawn)) {
				eval -= pieces::pawn_value;
			} else if (piece == (pieces::black | pieces::knight)) {
				eval -= pieces::knight_value;
			} else if (piece == (pieces::black | pieces::bishop)) {
				eval -= pieces::bishop_value;
			} else if (piece == (pieces::black | pieces::rook)) {
				eval -= pieces::rook_value;
			} else if (piece == (pieces::black | pieces::queen)) {
				eval -= pieces::queen_value;
			} else if (piece == (pieces::black | pieces::king)) {
				eval -= 100000; 
			} else if (piece == pieces::pawn) {
				eval += pieces::pawn_value;
			} else if (piece == pieces::knight) {
				eval += pieces::knight_value;
			} else if (piece == pieces::bishop) {
				eval += pieces::bishop_value;
			} else if (piece == pieces::rook) {
				eval += pieces::rook_value;
			} else if (piece == pieces::queen) {
				eval += pieces::queen_value;
			} else if (piece == pieces::king) {
				eval += 100000;
			}
		}
	}

	//std::vector<move::Move> white_moves;
	//std::vector<Board> white_boards;
	//board.get_white_moves(white_moves, white_boards);

	//std::vector<move::Move> black_moves;
	//std::vector<Board> black_boards;
	//board.get_white_moves(black_moves, black_boards);

	//eval += white_moves.size();
	//eval -= black_moves.size();
	//if (eval != 0) {
	//	std::cout << eval << "\n";
	//}
	return 100 * eval;
}
