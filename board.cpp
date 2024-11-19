#import "board.hpp"

#include <iostream>
#include "assert.h"


Board::Board() 
: _en_passant(move::Move(10, 10, 10, 10)), _can_white_castle_queen_side(true), _can_white_castle_king_side(true),
_can_black_castle_queen_side(true), _can_black_castle_king_side(true) {
	_board[0][0] = pieces::white | pieces::rook;
	_board[0][1] = pieces::white | pieces::knight;
	_board[0][2] = pieces::white | pieces::bishop;
	_board[0][3] = pieces::white | pieces::queen;
	_board[0][4] = pieces::white | pieces::king;
	_board[0][5] = pieces::white | pieces::bishop;
	_board[0][6] = pieces::white | pieces::knight;
	_board[0][7] = pieces::white | pieces::rook;

	for (auto j = 0; j < 8; ++j) {
			_board[1][j] = pieces::white | pieces::pawn;
	}

	for (auto i = 2; i < 6; ++i) {
		for (auto j = 0; j < 8; ++j) {
			_board[i][j] = pieces::empty;
		}
	}

	for (auto j = 0; j < 8; ++j) {
			_board[6][j] = pieces::black | pieces::pawn;
	}

	_board[7][0] = pieces::black | pieces::rook;
	_board[7][1] = pieces::black | pieces::knight;
	_board[7][2] = pieces::black | pieces::bishop;
	_board[7][3] = pieces::black | pieces::queen;
	_board[7][4] = pieces::black | pieces::king;
	_board[7][5] = pieces::black | pieces::bishop;
	_board[7][6] = pieces::black | pieces::knight;
	_board[7][7] = pieces::black | pieces::rook;
}

Board::Board(const Board& board, const move::Move& move)
: _board(board.get_raw_board()), _en_passant(board.get_en_passant()),
 _can_white_castle_queen_side(board.can_white_castle_queen_side()), _can_white_castle_king_side(can_white_castle_king_side()),
_can_black_castle_queen_side(can_black_castle_queen_side()), _can_black_castle_king_side(can_black_castle_king_side()) {
	assert(move.get_from_row() < 8);
	assert(move.get_from_col() < 8);
	assert(move.get_to_row() < 8);
	assert(move.get_to_col() < 8);
	assert(_board[move.get_from_row()][move.get_from_col()] != pieces::empty);

	if (move.get_to_row() == move.get_from_row()) {
		if (move.get_to_col() == move.get_from_col()) {
			assert(false);
		}
	}
		
	if (move.get_promotion_piece() == 0) {
		// En passant
		if (_en_passant.get_to_col() == move.get_to_col() && _en_passant.get_to_row() + 1 == move.get_to_row()) {
			if (_board[move.get_from_row()][move.get_from_col()] == (pieces::white | pieces::pawn)) {
				_board[_en_passant.get_to_row()][_en_passant.get_to_col()] = pieces::empty;
			}
		} else if (_en_passant.get_to_col() == move.get_to_col() && _en_passant.get_to_row() - 1 == move.get_to_row()) {
			if (_board[move.get_from_row()][move.get_from_col()] == (pieces::black | pieces::pawn)) {
				_board[_en_passant.get_to_row()][_en_passant.get_to_col()] = pieces::empty;
			}
		}	else if (move.get_from_row() == 1 && move.get_to_row() == 3) {
			if (_board[move.get_from_row()][move.get_from_col()] == (pieces::white | pieces::pawn)) {	
				_en_passant = move;
			}
		}	else if (move.get_from_row() == 6 && move.get_to_row() == 4) {
			if (_board[move.get_from_row()][move.get_from_col()] == (pieces::black | pieces::pawn)) {	
				_en_passant = move;
			}
		} else {
				_en_passant = move::Move{10, 10, 10, 10, 0};
		}
		// Castling rights
		if (move.get_from_row() == 0 && move.get_from_col() == 0) {
			_can_white_castle_queen_side = false;
		} else if (move.get_from_row() == 0 && move.get_from_col() == 7) {
			_can_white_castle_king_side = false;
		} else if (move.get_from_row() == 0 && move.get_from_col() == 4) {
			_can_white_castle_queen_side = false;
			_can_white_castle_king_side = false;
		} else if (move.get_from_row() == 7 && move.get_from_col() == 0) {
			_can_black_castle_queen_side = false;
		} else if (move.get_from_row() == 7 && move.get_from_col() == 7) {
			_can_black_castle_king_side = false;
		} else if (move.get_from_row() == 7 && move.get_from_col() == 4) {
			_can_black_castle_queen_side = false;
			_can_black_castle_king_side = false;
		}
		
		// Castling
		if (_board[move.get_from_row()][move.get_from_col()] == pieces::king) {
			if (move.get_from_col() > move.get_to_col()) { // Avoid negative numbers
				if (move.get_from_col() - move.get_to_col() == 2) { // Queen side castle
					_board[move.get_from_row()][3] = _board[move.get_from_row()][0];
					_board[move.get_from_row()][0] = pieces::empty;
				}
			} else if (move.get_to_col() - move.get_from_col() == 2) { // King side castle
					_board[move.get_from_row()][5] = _board[move.get_from_row()][7];
					_board[move.get_from_row()][7] = pieces::empty;
			}
		}
		_board[move.get_to_row()][move.get_to_col()] = _board[move.get_from_row()][move.get_from_col()];
		_board[move.get_from_row()][move.get_from_col()] = pieces::empty;
	} else {
		_board[move.get_to_row()][move.get_to_col()] = move.get_promotion_piece();
		_board[move.get_from_row()][move.get_from_col()] = pieces::empty;
		_en_passant = move::Move{10, 10, 10, 10, 0};
	}
}

void Board::print() const {
	for (auto i = 7; i > -1; --i) {
		for (auto j = 0; j < 8; ++j) {
			if (_board[i][j] >= pieces::black) {
				std::cout << "-" << static_cast<int>(0b0111 & _board[i][j]) << " ";
			} else {	
				std::cout << " " << static_cast<int>(_board[i][j]) << " ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

const std::array<std::array<uint8_t, 8>, 8>& Board::get_raw_board() const {
	return _board;
}

const move::Move& Board::get_en_passant() const {
	return _en_passant;
}

bool Board::can_white_castle_queen_side() const {
	return _can_white_castle_queen_side;
}

bool Board::can_white_castle_king_side() const {
	return _can_white_castle_king_side;
}

bool Board::can_black_castle_queen_side() const {
	return _can_black_castle_queen_side;
}

bool Board::can_black_castle_king_side() const {
	return _can_black_castle_king_side;
}

void Board::get_white_moves(std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle) const {
for (uint8_t row = 0; row < 8; ++row) {
		for (uint8_t col = 0; col < 8; ++col) {
			switch (_board[row][col]) {
				case (pieces::white | pieces::pawn):
					_get_white_pawn_moves(row, col, moves, boards, only_captures);
					break;
				case (pieces::white | pieces::knight):
					_get_white_knight_moves(row, col, moves, boards, only_captures);
					break;
				case (pieces::white | pieces::bishop):
					_get_white_bishop_moves(row, col, moves, boards, only_captures);
					break;
				case (pieces::white | pieces::rook):
					_get_white_rook_moves(row, col, moves, boards, only_captures);
					break;
				case (pieces::white | pieces::queen):
					_get_white_rook_moves(row, col, moves, boards, only_captures);
					_get_white_bishop_moves(row, col, moves, boards, only_captures);
					break;
				case (pieces::white | pieces::king):
					_get_white_king_moves(row, col, moves, boards, only_captures, check_castle);
					break;
			}
		}
	}
}

void Board::get_black_moves(std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle) const {
for (uint8_t row = 0; row < 8; ++row) {
		for (uint8_t col = 0; col < 8; ++col) {
			switch (_board[row][col]) {
				case (pieces::black | pieces::pawn):
					_get_black_pawn_moves(row, col, moves, boards, only_captures);
					break;
				case (pieces::black | pieces::knight):
					_get_black_knight_moves(row, col, moves, boards, only_captures);
					break;
				case (pieces::black | pieces::bishop):
					_get_black_bishop_moves(row, col, moves, boards, only_captures);
					break;
				case (pieces::black | pieces::rook):
					_get_black_rook_moves(row, col, moves, boards, only_captures);
					break;
				case (pieces::black | pieces::queen):
					_get_black_rook_moves(row, col, moves, boards, only_captures);
					_get_black_bishop_moves(row, col, moves, boards, only_captures);
					break;
				case (pieces::black | pieces::king):
					_get_black_king_moves(row, col, moves, boards, only_captures, check_castle);
					break;
			}
		}
	}
}

bool Board::white_in_checkmate(const std::vector<move::Move>& moves) const {
	for (const auto& move : moves) {
		if (_board[move.get_to_row()][move.get_to_col()] == (pieces::white | pieces::king)) {
			return true;
		}
	}
	return false;
}

bool Board::black_in_checkmate(const std::vector<move::Move>& moves) const {
	for (const auto& move : moves) {
		if (_board[move.get_to_row()][move.get_to_col()] == (pieces::black | pieces::king)) {
			return true;
		}
	}
	return false;
}

bool Board::is_game_finished(bool white_to_move) const {
	auto can_get_captured = [&](pieces::piece piece) {
		std::vector<move::Move> moves;
		std::vector<Board> boards;
		if ((piece & pieces::black) == 0) {
			get_black_moves(moves, boards, true);
		} else {
			get_white_moves(moves, boards, true);
		}
		for (const auto& move : moves) {
			if (_board[move.get_to_row()][move.get_to_col()] == piece) {
				return true;
			}	
		}
		return false;
	};

	if (white_to_move) {
		return can_get_captured((pieces::king | pieces::black));
	} else {
		return can_get_captured(pieces::king);
	}
}

inline bool Board::_square_has_white_piece(const uint8_t row, const uint8_t col) const {
	assert(row < 8);
	assert(col < 8);
	return ((_board[row][col] & pieces::black) == 0) && ((_board[row][col] | pieces::empty) != 0);
}

inline bool Board::_square_has_black_piece(const uint8_t row, const uint8_t col) const {
	assert(row < 8);
	assert(col < 8);
	return ((_board[row][col] & pieces::black) != 0);
}

inline bool Board::_square_is_empty(const uint8_t row, const uint8_t col) const {
	assert(row < 8);
	assert(col < 8);
	return (_board[row][col] == pieces::empty);
}

inline void Board::_get_white_pawn_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	// Promotion
	if (row == 6) {
		if (!only_captures) {
			if (_square_is_empty(7, col)) {
				moves.emplace_back(move::Move{row, col, 7, col, static_cast<pieces::piece>(pieces::white | pieces::knight)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(move::Move{row, col, 7, col, static_cast<pieces::piece>(pieces::white | pieces::bishop)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(move::Move{row, col, 7, col, static_cast<pieces::piece>(pieces::white | pieces::rook)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(move::Move{row, col, 7, col, static_cast<pieces::piece>(pieces::white | pieces::queen)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col > 0 && _square_has_black_piece(7, col - 1)) {
			moves.emplace_back(move::Move{row, col, 7, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::white | pieces::knight)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 7, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::white | pieces::bishop)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 7, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::white | pieces::rook)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 7, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::white | pieces::queen)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col < 7 && _square_has_black_piece(row + 1, col + 1)) {
			moves.emplace_back(move::Move{row, col, 7, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::white | pieces::knight)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 7, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::white | pieces::knight)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 7, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::white | pieces::rook)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 7, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::white | pieces::queen)});
			boards.push_back(Board{*this, moves.back()});
		}
	} else {
		if (!only_captures) {
			// 1 step forward
			if (row < 6 && _square_is_empty(row + 1, col)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), col});
				boards.push_back(Board{*this, moves.back()});
				// 2 steps forward
				if (row == 1 && _square_is_empty(row + 2, col)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), col});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
		// left take	
		if (col > 0 && _square_has_black_piece(row + 1, col - 1)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		}
		// right take
		if (col < 7 && _square_has_black_piece(row + 1, col + 1)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		}
		
		// en passant	
		if (row == 4 && _en_passant.get_to_row() == 4) {
			if (col == _en_passant.get_to_col() - 1 && col > 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (col == _en_passant.get_to_col() + 1 && col < 7) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}  
		}
	}
}

inline void Board::_get_black_pawn_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	// Promotion
	if (row == 1) {
		if (!only_captures) {
			if (_square_is_empty(0, col)) {
				moves.emplace_back(move::Move{row, col, 0, col, static_cast<pieces::piece>(pieces::black | pieces::knight)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(move::Move{row, col, 0, col, static_cast<pieces::piece>(pieces::black | pieces::bishop)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(move::Move{row, col, 0, col, static_cast<pieces::piece>(pieces::black | pieces::rook)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(move::Move{row, col, 0, col, static_cast<pieces::piece>(pieces::black | pieces::queen)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col > 0 && _square_has_white_piece(0, col - 1)) {
			moves.emplace_back(move::Move{row, col, 0, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::black | pieces::knight)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 0, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::black | pieces::bishop)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 0, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::black | pieces::rook)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 0, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::black | pieces::queen)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col < 7 && _square_has_white_piece(0, col + 1)) {
			moves.emplace_back(move::Move{row, col, 0, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::black | pieces::knight)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 0, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::black | pieces::bishop)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 0, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::black | pieces::rook)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(move::Move{row, col, 0, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::black | pieces::queen)});
			boards.push_back(Board{*this, moves.back()});
		}
	} else {
		if (!only_captures) {
			// 1 step forward
			if (row > 1 && _square_is_empty(row - 1, col)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), col});
				boards.push_back(Board{*this, moves.back()});
				// 2 steps forward
				if (row == 6 && _square_is_empty(row - 2, col)) {
					// seg
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), col});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
		// left take	
		if (col > 0 && _square_has_white_piece(row - 1, col - 1)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		}
		// right take
		if (col < 7 && _square_has_white_piece(row - 1, col + 1)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		}
		
		// en passant	
		if (row == 3 && _en_passant.get_to_row() == 3) {
			if (col == _en_passant.get_to_col() + 1 && col > 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (col == _en_passant.get_to_col() - 1 && col < 7) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}  
		}
	}
}

inline void Board::_get_white_knight_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	if (row + 2 < 8) {
		if (col + 1 < 8) {
			if (only_captures) {
				if (_square_has_black_piece(row + 2, col + 1)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col + 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row + 2, col + 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (only_captures) {
				if (_square_has_black_piece(row + 2, col - 1)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col - 1)});
					boards.push_back(Board{*this, moves.back()});
				}	
			} else if (!_square_has_white_piece(row + 2, col - 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
	if (row + 1 < 8) {
		if (col + 2 < 8) {
			if (only_captures) {
				if (_square_has_black_piece(row + 1, col + 2)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row + 1, col + 2)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if (only_captures) {
				if (_square_has_black_piece(row + 1, col - 2)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row + 1, col - 2)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 1 > -1) {
		if (col + 2 < 8) {
			if (only_captures) {
				if (_square_has_black_piece(row - 1, col + 2)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (_square_has_white_piece(row - 1, col + 2)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if (only_captures) {
 				if (_square_has_black_piece(row - 1, col - 2)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row - 1, col - 2)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 2 > -1) {
		if (col + 1 < 8) {
			if (only_captures) {
				if (_square_has_black_piece(row - 2, col + 1)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col + 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row - 2, col + 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (only_captures) {
				if (_square_has_black_piece(row - 2, col - 1)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col - 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row - 2, col - 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
}

inline void Board::_get_black_knight_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	if (row + 2 < 8) {
		if (col + 1 < 8) {
			if (only_captures) {	
				if (_square_has_white_piece(row + 2, col + 1)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col + 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row + 2, col + 1)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col + 1)});
					boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (only_captures) {
				if (_square_has_white_piece(row + 2, col - 1)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col - 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row + 2, col - 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
	if (row + 1 < 8) {
		if (col + 2 < 8) {
			if (only_captures) {
				if (_square_has_white_piece(row + 1, col + 2)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row + 1, col + 2)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if (only_captures) {
				if (_square_has_white_piece(row + 1, col - 2)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row + 1, col - 2)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 1 > -1) {
		if (col + 2 < 8) {
			if (only_captures) {
				if (_square_has_white_piece(row - 1, col + 2)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 2)});
					boards.push_back(Board{*this, moves.back()});
				} 
			} else if (!_square_has_black_piece(row - 1, col + 2)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if (only_captures) {
				if (_square_has_white_piece(row - 1, col - 2)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row - 1, col - 2)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 2 > -1) {
		if (col + 1 < 8) {
			if (only_captures) {
				if (_square_has_white_piece(row - 2, col + 1)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col + 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row - 2, col + 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (only_captures) {
				if (_square_has_white_piece(row - 2, col - 1)) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col - 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row - 2, col - 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
}

inline void Board::_get_white_bishop_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row < 7 && col < 7) {
			++row;
			++col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row,  col)) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row < 7 && col > 0) {
			++row;
			--col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row > 0 && col < 7) {
			--row;
			++col;
			if (_square_is_empty(row, col) && !only_captures) { 
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row > 0 && col > 0) {
			--row;
			--col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_black_bishop_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row < 7 && col < 7) {
			++row;
			++col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) { 
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row < 7 && col > 0) {
			++row;
			--col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row > 0 && col < 7) {
			--row;
			++col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row > 0 && col > 0) {
			--row;
			--col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_white_rook_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		while (row < 7) {
			++row;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		while (row > 0) {
			--row;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_col = col;
		while (col < 7) {
			++col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_col = col;
		while (col > 0) {
			--col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_black_rook_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		while (row < 7) {
			++row;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		while (row > 0) {
			--row;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_col = col;
		while (col < 7) {
			++col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_col = col;
		while (col > 0) {
			--col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_white_king_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle) const {
	if (row + 1 < 8) {
		if (_square_is_empty(row + 1, col) && !only_captures) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_black_piece(row + 1, col)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if (_square_is_empty(row + 1, col + 1) && !only_captures) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row + 1, col + 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (_square_is_empty(row + 1, col - 1) && !only_captures) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row + 1, col - 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}

	if (col + 1 < 8) {
		if (_square_is_empty(row, col + 1) && !only_captures) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_black_piece(row, col + 1)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		}
	}

	if (col - 1 > -1) {
		if (_square_is_empty(row, col - 1) && !only_captures) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_black_piece(row, col - 1)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		}
	}

	if (row - 1 > -1) {
		if (_square_is_empty(row - 1, col) && !only_captures) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_black_piece(row - 1, col)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if (_square_is_empty(row - 1, col + 1) && !only_captures) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row - 1, col + 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (_square_is_empty(row - 1, col - 1) && !only_captures) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row - 1, col - 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
	if (check_castle) {
		// Queen side castle
		if (!only_captures && _can_white_castle_queen_side) {
			if (_square_is_empty(0, 1) && _square_is_empty(0, 2) && _square_is_empty(0, 3)) {
				std::vector<move::Move> next_moves;
				std::vector<Board> next_boards;
				get_black_moves(next_moves, next_boards, false, false);
				bool castling_square_attacked = false;
				for (const auto& move : next_moves) {
					if (move.get_to_row() == 0) {
						if (move.get_to_col() == 2 || move.get_to_col() == 3 || move.get_to_col() == 4) {
							castling_square_attacked = true;
							break;
						}
					}
				}
				if (!castling_square_attacked) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(0), static_cast<uint8_t>(2)});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
		// King side castle
		if (!only_captures && _can_white_castle_king_side) {
			if (_square_is_empty(0, 5) && _square_is_empty(0, 6)) {
				std::vector<move::Move> next_moves;
				std::vector<Board> next_boards;
				get_black_moves(next_moves, next_boards, false, false);
				bool castling_square_attacked = false;
				for (const auto& move : next_moves) {
					if (move.get_to_row() == 0) {
						if (move.get_to_col() == 4 || move.get_to_col() == 5 || move.get_to_col() == 6) {
							castling_square_attacked = true;
							break;
						}
					}
				}
				if (!castling_square_attacked) {
					moves.emplace_back(move::Move{row, col, 0, static_cast<uint8_t>(6)});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
	}
}	

inline void Board::_get_black_king_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle) const {
	if (row + 1 < 8) {
		if (_square_is_empty(row + 1, col) && !only_captures) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_white_piece(row + 1, col)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if (_square_is_empty(row + 1, col + 1) && !only_captures) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row + 1, col + 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (_square_is_empty(row + 1, col - 1) && !only_captures) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row + 1, col - 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}

	if (col + 1 < 8) {
		if (_square_is_empty(row, col + 1) && !only_captures) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_white_piece(row, col + 1)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		}
	}

	if (col - 1 > -1) {
		if (_square_is_empty(row, col - 1) && !only_captures) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_white_piece(row, col - 1)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		}
	}

	if (row - 1 > -1) {
		if (_square_is_empty(row - 1, col) && !only_captures) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_white_piece(row - 1, col)) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if (_square_is_empty(row - 1, col + 1) && !only_captures) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row - 1, col + 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (_square_is_empty(row - 1, col - 1) && !only_captures) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row - 1, col - 1)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
	if (check_castle) {
		// Queen side castle
		if (!only_captures && _can_black_castle_queen_side) {
			if (_square_is_empty(7, 1) && _square_is_empty(7, 2) && _square_is_empty(7, 3)) {
				std::vector<move::Move> next_moves;
				std::vector<Board> next_boards;
				get_white_moves(next_moves, next_boards, false, false);
				bool castling_square_attacked = false;
				for (const auto& move : next_moves) {
					if (move.get_to_row() == 7) {
						if (move.get_to_col() == 2 || move.get_to_col() == 3 || move.get_to_col() == 4) {
							castling_square_attacked = true;
							break;
						}
					}
				}
				if (!castling_square_attacked) {
					moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(7), static_cast<uint8_t>(2)});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
		// King side castle
		if (!only_captures && _can_black_castle_king_side) {
			if (_square_is_empty(7, 5) && _square_is_empty(7, 6)) {
				std::vector<move::Move> next_moves;
				std::vector<Board> next_boards;
				get_white_moves(next_moves, next_boards, false, false);
				bool castling_square_attacked = false;
				for (const auto& move : next_moves) {
					if (move.get_to_row() == 7) {
						if (move.get_to_col() == 4 || move.get_to_col() == 5 || move.get_to_col() == 6) {
							castling_square_attacked = true;
							break;
						}
					}
				}
				if (!castling_square_attacked) {
					moves.emplace_back(move::Move{row, col, 7, static_cast<uint8_t>(6)});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
	}
}
