#import "board.hpp"

#include <iostream>
#include "assert.h"

Board::Board() 
: _last_move(Move(0, 0, 0, 0, 0, false, false)), _can_white_castle_queen_side(true), _can_white_castle_king_side(true),
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

Board::Board(std::string position) :
_last_move(Move(0, 0, 0, 0, 0, false, false)), _can_white_castle_queen_side(true), _can_white_castle_king_side(true),
_can_black_castle_queen_side(true), _can_black_castle_king_side(true) {
	// Fill board
	int position_index = 0;
	int piece_sign = 1;
	int ascii_minus_value = 45;
	int ascii_zero_value = 48;
	uint8_t row = 0;
	uint8_t col = 0;

	while (row < 8) {
		while (col < 8) {	
			if (position[position_index] == ascii_minus_value) {
				piece_sign = -1;
			} else {
				if (piece_sign == 1) {
					_board[row][col] = pieces::white | (uint8_t)(position[position_index] - ascii_zero_value);
				}	else {
					_board[row][col] = pieces::black | (uint8_t)(position[position_index] - ascii_zero_value);
				}
				piece_sign = 1;	
				++col;
			}
			++position_index;
		}
		col = 0;
		++row;
	}
	
	++position_index;
	if (position[position_index] == 0) {
		_can_white_castle_queen_side = false;
	} 
	++position_index;
	if (position[position_index] == 0) {
		_can_white_castle_king_side = false;
	}
	++position_index;
	if (position[position_index] == 0) {
		_can_black_castle_queen_side = false;
	} 
	++position_index;
	if (position[position_index] == 0) {
		_can_black_castle_king_side = false;
	} 
}

Board::Board(const Board& board, const Move& move)
: _board(board.get_raw_board()), _last_move(move),
	_can_white_castle_queen_side(board.can_white_castle_queen_side()),
	_can_white_castle_king_side(board.can_white_castle_king_side()),
	_can_black_castle_queen_side(board.can_black_castle_queen_side()),
	_can_black_castle_king_side(board.can_black_castle_king_side()) {
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

	if (move.get_promotion_piece() != 0) {
		assert(move.enables_en_passant() == false);
		assert(move.is_en_passant() == false);
	}
	assert(move.enables_en_passant() == false || move.is_en_passant() == false);
	
	// Update castling rights
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

	// Move rook if castling
	if (_board[move.get_from_row()][move.get_from_col()] == (pieces::king | pieces::white)
			|| _board[move.get_from_row()][move.get_from_col()] == (pieces::king | pieces::black)) {
		if (move.get_from_col() > move.get_to_col()) { // Avoid negative numbers (uint)
			if (move.get_from_col() - move.get_to_col() == 2) { // Queen side castle
				_board[move.get_from_row()][3] = _board[move.get_from_row()][0];
				_board[move.get_from_row()][0] = pieces::empty;
			}
		} else if (move.get_to_col() - move.get_from_col() == 2) { // King side castle
				_board[move.get_from_row()][5] = _board[move.get_from_row()][7];
				_board[move.get_from_row()][7] = pieces::empty;
		}
	}

	if (move.get_promotion_piece() != 0) { 
		_board[move.get_to_row()][move.get_to_col()] = move.get_promotion_piece();
		_board[move.get_from_row()][move.get_from_col()] = pieces::empty;
	} else if (move.is_en_passant() == true) { 
		_board[board.get_last_move().get_to_row()][board.get_last_move().get_to_col()] = pieces::empty; 
		_board[move.get_to_row()][move.get_to_col()] = _board[move.get_from_row()][move.get_from_col()];
		_board[move.get_from_row()][move.get_from_col()] = pieces::empty;
	} else {
		_board[move.get_to_row()][move.get_to_col()] = _board[move.get_from_row()][move.get_from_col()];
		_board[move.get_from_row()][move.get_from_col()] = pieces::empty;
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

[[nodiscard]] const std::array<std::array<uint8_t, 8>, 8>& Board::get_raw_board() const {
	return _board;
}

[[nodiscard]] const Move& Board::get_last_move() const {
	return _last_move;
}

[[nodiscard]] bool Board::can_white_castle_queen_side() const {
	return _can_white_castle_queen_side;
}

[[nodiscard]] bool Board::can_white_castle_king_side() const {
	return _can_white_castle_king_side;
}

[[nodiscard]] bool Board::can_black_castle_queen_side() const {
	return _can_black_castle_queen_side;
}

[[nodiscard]] bool Board::can_black_castle_king_side() const {
	return _can_black_castle_king_side;
}

[[nodiscard]] bool Board::operator==(const Board& rhs) const {
	for (uint8_t row = 0; row < 8; ++row) {
		for (uint8_t col = 0; col < 8; ++col) {
			if (_board[row][col] != rhs.get_raw_board()[row][col])	
				return false;
		}
	}
	
	if (can_white_castle_queen_side() != rhs.can_white_castle_queen_side()) 
		return false;		
	if (can_white_castle_king_side() != rhs.can_white_castle_king_side()) 
		return false;		
	if (can_black_castle_queen_side() != rhs.can_black_castle_queen_side()) 
		return false;		
	if (can_black_castle_king_side() != rhs.can_black_castle_king_side()) 
		return false;		
	
	if (_last_move.enables_en_passant() != rhs.get_last_move().enables_en_passant()) {
		return false;
	}

	return true;
}

void Board::get_white_moves(std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle) const {
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

void Board::get_black_moves(std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle) const {
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

[[nodiscard]] std::pair<bool, Move> Board::white_in_checkmate(const std::vector<Move>& moves) const {
	for (const auto& move : moves) {
		if (_board[move.get_to_row()][move.get_to_col()] == (pieces::white | pieces::king)) {
			return std::make_pair(true, move);
		}
	}
	return std::make_pair(false, Move{0, 0, 0, 0});
}

[[nodiscard]] std::pair<bool, Move> Board::black_in_checkmate(const std::vector<Move>& moves) const {
	for (const auto& move : moves) {
		if (_board[move.get_to_row()][move.get_to_col()] == (pieces::black | pieces::king)) {
			return std::make_pair(true, move);
		}
	}
	return std::make_pair(false, Move{0, 0, 0, 0});
}

[[nodiscard]] bool Board::is_checkmate(bool white_to_move) const {
	auto can_get_captured = [&](pieces::piece piece) {
		std::vector<Move> moves;
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
		return can_get_captured(pieces::king | pieces::white);
	}
}

[[nodiscard]] inline bool Board::_square_has_white_piece(const uint8_t row, const uint8_t col) const {
	assert(row < 8);
	assert(col < 8);
	return ((_board[row][col] & pieces::black) == 0) && ((_board[row][col] | pieces::empty) != 0);
}

[[nodiscard]] inline bool Board::_square_has_black_piece(const uint8_t row, const uint8_t col) const {
	assert(row < 8);
	assert(col < 8);
	return ((_board[row][col] & pieces::black) != 0);
}

[[nodiscard]] inline bool Board::_square_is_empty(const uint8_t row, const uint8_t col) const {
	assert(row < 8);
	assert(col < 8);
	return (_board[row][col] == pieces::empty);
}

inline void Board::_get_white_pawn_moves(uint8_t row, uint8_t col, std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	// Promotion
	if (row == 6) {
		if (!only_captures) {
			if (_square_is_empty(7, col)) {
				moves.emplace_back(Move{row, col, 7, col, static_cast<pieces::piece>(pieces::white | pieces::knight)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(Move{row, col, 7, col, static_cast<pieces::piece>(pieces::white | pieces::bishop)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(Move{row, col, 7, col, static_cast<pieces::piece>(pieces::white | pieces::rook)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(Move{row, col, 7, col, static_cast<pieces::piece>(pieces::white | pieces::queen)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col > 0 && _square_has_black_piece(7, col - 1)) {
			moves.emplace_back(Move{row, col, 7, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::white | pieces::knight)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 7, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::white | pieces::bishop)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 7, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::white | pieces::rook)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 7, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::white | pieces::queen)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col < 7 && _square_has_black_piece(row + 1, col + 1)) {
			moves.emplace_back(Move{row, col, 7, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::white | pieces::knight)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 7, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::white | pieces::knight)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 7, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::white | pieces::rook)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 7, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::white | pieces::queen)});
			boards.push_back(Board{*this, moves.back()});
		}
	} else {
		if (!only_captures) {
			// 1 step forward
			if (row < 6 && _square_is_empty(row + 1, col)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), col});
				boards.push_back(Board{*this, moves.back()});
				// 2 steps forward
				if (row == 1 && _square_is_empty(row + 2, col)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 2), col, pieces::empty, false, true});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
		// left take	
		if (col > 0 && _square_has_black_piece(row + 1, col - 1)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		}
		// right take
		if (col < 7 && _square_has_black_piece(row + 1, col + 1)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		}
		
		// en passant	
		if (_last_move.enables_en_passant()) {
			if (row == 4 && _last_move.get_to_row() == 4) {
				if (col == _last_move.get_to_col() - 1 && col > 0) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1), pieces::empty, true, false});
					boards.push_back(Board{*this, moves.back()});
				} else if (col == _last_move.get_to_col() + 1 && col < 7) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1), pieces::empty, true, false});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
	}
}

inline void Board::_get_black_pawn_moves(uint8_t row, uint8_t col, std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	// Promotion
	if (row == 1) {
		if (!only_captures) {
			if (_square_is_empty(0, col)) {
				moves.emplace_back(Move{row, col, 0, col, static_cast<pieces::piece>(pieces::black | pieces::knight)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(Move{row, col, 0, col, static_cast<pieces::piece>(pieces::black | pieces::bishop)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(Move{row, col, 0, col, static_cast<pieces::piece>(pieces::black | pieces::rook)});
				boards.push_back(Board{*this, moves.back()});
				moves.emplace_back(Move{row, col, 0, col, static_cast<pieces::piece>(pieces::black | pieces::queen)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col > 0 && _square_has_white_piece(0, col - 1)) {
			moves.emplace_back(Move{row, col, 0, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::black | pieces::knight)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 0, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::black | pieces::bishop)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 0, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::black | pieces::rook)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 0, static_cast<uint8_t>(col - 1), static_cast<pieces::piece>(pieces::black | pieces::queen)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col < 7 && _square_has_white_piece(0, col + 1)) {
			moves.emplace_back(Move{row, col, 0, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::black | pieces::knight)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 0, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::black | pieces::bishop)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 0, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::black | pieces::rook)});
			boards.push_back(Board{*this, moves.back()});
			moves.emplace_back(Move{row, col, 0, static_cast<uint8_t>(col + 1), static_cast<pieces::piece>(pieces::black | pieces::queen)});
			boards.push_back(Board{*this, moves.back()});
		}
	} else {
		if (!only_captures) {
			// 1 step forward
			if (row > 1 && _square_is_empty(row - 1, col)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), col});
				boards.push_back(Board{*this, moves.back()});
				// 2 steps forward
				if (row == 6 && _square_is_empty(row - 2, col)) {
					// seg
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 2), col, pieces::empty, false, true});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
		// left take	
		if (col > 0 && _square_has_white_piece(row - 1, col - 1)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		}
		// right take
		if (col < 7 && _square_has_white_piece(row - 1, col + 1)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		}
		
		// en passant	
		if (_last_move.enables_en_passant()) {	
			if (row == 3 && _last_move.get_to_row() == 3) {
				if (col == _last_move.get_to_col() + 1 && col > 0) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1), pieces::empty, true, false});
					boards.push_back(Board{*this, moves.back()});
				} else if (col == _last_move.get_to_col() - 1 && col < 7) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1), pieces::empty, true, false});
					boards.push_back(Board{*this, moves.back()});
				}  
			}
		}	
	}
}

inline void Board::_get_white_knight_moves(uint8_t row, uint8_t col, std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	if (row + 2 < 8) {
		if (col + 1 < 8) {
			if (only_captures) {
				if (_square_has_black_piece(row + 2, col + 1)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col + 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row + 2, col + 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (only_captures) {
				if (_square_has_black_piece(row + 2, col - 1)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col - 1)});
					boards.push_back(Board{*this, moves.back()});
				}	
			} else if (!_square_has_white_piece(row + 2, col - 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
	if (row + 1 < 8) {
		if (col + 2 < 8) {
			if (only_captures) {
				if (_square_has_black_piece(row + 1, col + 2)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row + 1, col + 2)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if (only_captures) {
				if (_square_has_black_piece(row + 1, col - 2)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row + 1, col - 2)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 1 > -1) {
		if (col + 2 < 8) {
			if (only_captures) {
				if (_square_has_black_piece(row - 1, col + 2)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row - 1, col + 2)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if (only_captures) {
 				if (_square_has_black_piece(row - 1, col - 2)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row - 1, col - 2)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 2 > -1) {
		if (col + 1 < 8) {
			if (only_captures) {
				if (_square_has_black_piece(row - 2, col + 1)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col + 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row - 2, col + 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (only_captures) {
				if (_square_has_black_piece(row - 2, col - 1)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col - 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_white_piece(row - 2, col - 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
}

inline void Board::_get_black_knight_moves(uint8_t row, uint8_t col, std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	if (row + 2 < 8) {
		if (col + 1 < 8) {
			if (only_captures) {	
				if (_square_has_white_piece(row + 2, col + 1)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col + 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row + 2, col + 1)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col + 1)});
					boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (only_captures) {
				if (_square_has_white_piece(row + 2, col - 1)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col - 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row + 2, col - 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
	if (row + 1 < 8) {
		if (col + 2 < 8) {
			if (only_captures) {
				if (_square_has_white_piece(row + 1, col + 2)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row + 1, col + 2)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if (only_captures) {
				if (_square_has_white_piece(row + 1, col - 2)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row + 1, col - 2)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 1 > -1) {
		if (col + 2 < 8) {
			if (only_captures) {
				if (_square_has_white_piece(row - 1, col + 2)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 2)});
					boards.push_back(Board{*this, moves.back()});
				} 
			} else if (!_square_has_black_piece(row - 1, col + 2)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if (only_captures) {
				if (_square_has_white_piece(row - 1, col - 2)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 2)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row - 1, col - 2)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 2)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 2 > -1) {
		if (col + 1 < 8) {
			if (only_captures) {
				if (_square_has_white_piece(row - 2, col + 1)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col + 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row - 2, col + 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (only_captures) {
				if (_square_has_white_piece(row - 2, col - 1)) {
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col - 1)});
					boards.push_back(Board{*this, moves.back()});
				}
			} else if (!_square_has_black_piece(row - 2, col - 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
}

inline void Board::_get_white_bishop_moves(uint8_t row, uint8_t col, std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row < 7 && col < 7) {
			++row;
			++col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row,  col)) {
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_black_bishop_moves(uint8_t row, uint8_t col, std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row < 7 && col < 7) {
			++row;
			++col;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) { 
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_white_rook_moves(uint8_t row, uint8_t col, std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		while (row < 7) {
			++row;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row, col)) {
				moves.emplace_back(Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_black_rook_moves(uint8_t row, uint8_t col, std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures) const {
	[this, &moves, &boards, &only_captures](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		while (row < 7) {
			++row;
			if (_square_is_empty(row, col) && !only_captures) {
				moves.emplace_back(Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
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
				moves.emplace_back(Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row, col)) {
				moves.emplace_back(Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.push_back(Board{*this, moves.back()});
				return;
			} else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_white_king_moves(uint8_t row, uint8_t col, std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle) const {
	if (row + 1 < 8) {
		if (_square_is_empty(row + 1, col) && !only_captures) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_black_piece(row + 1, col)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if (_square_is_empty(row + 1, col + 1) && !only_captures) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row + 1, col + 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (_square_is_empty(row + 1, col - 1) && !only_captures) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row + 1, col - 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}

	if (col + 1 < 8) {
		if (_square_is_empty(row, col + 1) && !only_captures) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_black_piece(row, col + 1)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		}
	}

	if (col - 1 > -1) {
		if (_square_is_empty(row, col - 1) && !only_captures) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_black_piece(row, col - 1)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		}
	}

	if (row - 1 > -1) {
		if (_square_is_empty(row - 1, col) && !only_captures) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_black_piece(row - 1, col)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if (_square_is_empty(row - 1, col + 1) && !only_captures) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row - 1, col + 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (_square_is_empty(row - 1, col - 1) && !only_captures) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_black_piece(row - 1, col - 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
	if (check_castle) {
		// Queen side castle
		if (!only_captures && _can_white_castle_queen_side) {
			if (_square_is_empty(0, 1) && _square_is_empty(0, 2) && _square_is_empty(0, 3)) {
				std::vector<Move> next_moves;
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
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(0), static_cast<uint8_t>(2)});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
		// King side castle
		if (!only_captures && _can_white_castle_king_side) {
			if (_square_is_empty(0, 5) && _square_is_empty(0, 6)) {
				std::vector<Move> next_moves;
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
					moves.emplace_back(Move{row, col, 0, static_cast<uint8_t>(6)});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
	}
}	

inline void Board::_get_black_king_moves(uint8_t row, uint8_t col, std::vector<Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle) const {
	if (row + 1 < 8) {
		if (_square_is_empty(row + 1, col) && !only_captures) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_white_piece(row + 1, col)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if (_square_is_empty(row + 1, col + 1) && !only_captures) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row + 1, col + 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (_square_is_empty(row + 1, col - 1) && !only_captures) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row + 1, col - 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}

	if (col + 1 < 8) {
		if (_square_is_empty(row, col + 1) && !only_captures) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_white_piece(row, col + 1)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col + 1)});
			boards.push_back(Board{*this, moves.back()});
		}
	}

	if (col - 1 > -1) {
		if (_square_is_empty(row, col - 1) && !only_captures) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_white_piece(row, col - 1)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col - 1)});
			boards.push_back(Board{*this, moves.back()});
		}
	}

	if (row - 1 > -1) {
		if (_square_is_empty(row - 1, col) && !only_captures) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		} else if (_square_has_white_piece(row - 1, col)) {
			moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col)});
			boards.push_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if (_square_is_empty(row - 1, col + 1) && !only_captures) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row - 1, col + 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (_square_is_empty(row - 1, col - 1) && !only_captures) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			} else if (_square_has_white_piece(row - 1, col - 1)) {
				moves.emplace_back(Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
				boards.push_back(Board{*this, moves.back()});
			}
		}
	}
	if (check_castle) {
		// Queen side castle
		if (!only_captures && _can_black_castle_queen_side) {
			if (_square_is_empty(7, 1) && _square_is_empty(7, 2) && _square_is_empty(7, 3)) {
				std::vector<Move> next_moves;
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
					moves.emplace_back(Move{row, col, static_cast<uint8_t>(7), static_cast<uint8_t>(2)});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
		// King side castle
		if (!only_captures && _can_black_castle_king_side) {
			if (_square_is_empty(7, 5) && _square_is_empty(7, 6)) {
				std::vector<Move> next_moves;
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
					moves.emplace_back(Move{row, col, 7, static_cast<uint8_t>(6)});
					boards.push_back(Board{*this, moves.back()});
				}
			}
		}
	}
}
