#import "board.hpp"
#include "pieces.hpp"
#include <print>
#include <cstring>


#include <chrono>


Board::Board() 
: _en_passant(move::Move(0, 0, 0, 0)) {
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

	for (auto i = 2; i < 6; ++i) {
		for (auto j = 0; j < 8; ++j) {
			_board[i][j] = pieces::empty;
		}
	}
}

Board::Board(const Board& board, const move::Move& move)
: _board(board.get_board()), _en_passant(board.get_en_passant()) {
	if (_en_passant.get_to_col() == move.get_to_col() && _en_passant.get_to_row() + 1 == move.get_to_row()) {
		if (_board[move.get_from_row()][move.get_from_col()] == (pieces::white | pieces::pawn)) {
			_board[_en_passant.get_to_row()][_en_passant.get_to_col()] = pieces::empty;
		}
	}

	if (_en_passant.get_to_col() == move.get_to_col() && _en_passant.get_to_row() - 1 == move.get_to_row()) {
		if (_board[move.get_from_row()][move.get_from_col()] == (pieces::black | pieces::pawn)) {
			_board[_en_passant.get_to_row()][_en_passant.get_to_col()] = pieces::empty;
		}
	}

	if (move.get_from_row() == 1 && move.get_to_row() == 3) {
		if (_board[move.get_from_row()][move.get_from_col()] == (pieces::white | pieces::pawn)) {	
			_en_passant = move;
		}
	}	else if (move.get_from_row() == 6 && move.get_to_row() == 4) {
		if (_board[move.get_from_row()][move.get_from_col()] == (pieces::black | pieces::pawn)) {	
			_en_passant = move;
		}
	} 

	_board[move.get_to_row()][move.get_to_col()] = _board[move.get_from_row()][move.get_from_col()];
	_board[move.get_from_row()][move.get_from_col()] = pieces::empty;
}

void Board::print() const {
	for (auto i = 7; i > -1; --i) {
		for (auto j = 0; j < 8; ++j) {
			if (_board[i][j] >= pieces::black) {
				std::print("{:2} ", -(0b0111 & _board[i][j]));
			} else {	
				std::print("{:2} ", _board[i][j]);
			}
		}
		std::println();
	}
	std::println();
}

const std::array<std::array<uint8_t, 8>, 8>& Board::get_board() const {
	return _board;
}

const move::Move& Board::get_en_passant() const {
	return _en_passant;
}

void Board::get_white_moves(std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	for (uint8_t row = 0; row < 8; ++row) {
		for (uint8_t col = 0; col < 8; ++col) {
			switch (_board[row][col]) {
				case (pieces::white | pieces::pawn):
					_get_white_pawn_moves(row, col, moves, boards);
					break;
				case (pieces::white | pieces::knight):
					_get_white_knight_moves(row, col, moves, boards);
					break;
				case (pieces::white | pieces::bishop):
					_get_white_bishop_moves(row, col, moves, boards);
					break;
				case (pieces::white | pieces::rook):
					_get_white_rook_moves(row, col, moves, boards);
					break;
				case (pieces::white | pieces::queen):
					_get_white_rook_moves(row, col, moves, boards);
					_get_white_bishop_moves(row, col, moves, boards);
					break;
				case (pieces::white | pieces::king):
					_get_white_king_moves(row, col, moves, boards);
					break;
			}
		}
	}
}

void Board::get_black_moves(std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	for (uint8_t row = 0; row < 8; ++row) {
		for (uint8_t col = 0; col < 8; ++col) {
			switch (_board[row][col]) {
				case (pieces::black | pieces::pawn):
					_get_black_pawn_moves(row, col, moves, boards);
					break;
				case (pieces::black | pieces::knight):
					_get_black_knight_moves(row, col, moves, boards);
					break;
				case (pieces::black | pieces::bishop):
					_get_black_bishop_moves(row, col, moves, boards);
					break;
				case (pieces::black | pieces::rook):
					_get_black_rook_moves(row, col, moves, boards);
					break;
				case (pieces::black | pieces::queen):
					_get_black_rook_moves(row, col, moves, boards);
					_get_black_bishop_moves(row, col, moves, boards);
					break;
				case (pieces::black | pieces::king):
					_get_black_king_moves(row, col, moves, boards);
					break;
			}
		}
	}
}


inline void Board::_get_white_pawn_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	// 1 step forward
	if (_board[row + 1][col] == pieces::empty) {
		moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), col});
		boards.emplace_back(Board{*this, moves.back()});
		// 2 steps forward
		if (row == 1 && _board[row + 2][col] == pieces::empty) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), col});
			boards.emplace_back(Board{*this, moves.back()});
		}
	}
	// left take	
	if (col > 0 && (_board[row + 1][col - 1] & pieces::black) != 0) {
		moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
		boards.emplace_back(Board{*this, moves.back()});
	}
	// right take
	if (col < 7 && (_board[row + 1][col + 1] & pieces::black) != 0) {
		moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
		boards.emplace_back(Board{*this, moves.back()});
	}
	
	// en passant	
	if (row == 4 && _en_passant.get_to_row() == 4) {
		if (col == _en_passant.get_to_col() - 1) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
			boards.emplace_back(Board{*this, moves.back()});
		} else if (col == _en_passant.get_to_col() + 1) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
			boards.emplace_back(Board{*this, moves.back()});
		}  
	}
}

inline void Board::_get_black_pawn_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	// 1 step forward
	if (_board[row - 1][col] == pieces::empty) {
		moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), col});
		boards.emplace_back(Board{*this, moves.back()});
		// 2 steps forward
		if (row == 6 && _board[row - 2][col] == pieces::empty) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), col});
			boards.emplace_back(Board{*this, moves.back()});
		}
	}
	// left take	
	if (col > 0 && (_board[row - 1][col - 1] & pieces::black) == 0 && _board[row - 1][col -1] != pieces::empty) {
		moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
		boards.emplace_back(Board{*this, moves.back()});
	}
	// right take
	if (col < 7 && (_board[row - 1][col + 1] & pieces::black) == 0 && _board[row - 1][col + 1] != pieces::empty) {
		moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
		boards.emplace_back(Board{*this, moves.back()});
	}
	
	// en passant	
	if (row == 3 && _en_passant.get_to_row() == 3) {
		if (col == _en_passant.get_to_col() - 1) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
			boards.emplace_back(Board{*this, moves.back()});
		} else if (col == _en_passant.get_to_col() + 1) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
			boards.emplace_back(Board{*this, moves.back()});
		}  
	}
}


inline void Board::_get_white_knight_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	if (row + 2 < 8) {
		if (col + 1 < 8) {
			if (_board[row + 2][col + 1] == pieces::empty || (_board[row + 2][col + 1] & pieces::black)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col + 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (_board[row + 2][col - 1] == pieces::empty || (_board[row + 2][col - 1] & pieces::black)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col - 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}
	if (row + 1 < 8) {
		if (col + 2 < 8) {
			if (_board[row + 1][col + 2] == pieces::empty || (_board[row + 1][col + 2] & pieces::black)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 2)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if (_board[row + 1][col - 2] == pieces::empty || (_board[row + 1][col - 2] & pieces::black)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 2)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 1 > -1) {
		if (col + 2 < 8) {
			if (_board[row - 1][col + 2] == pieces::empty || (_board[row - 1][col + 2] & pieces::black)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 2)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if (_board[row - 1][col - 2] == pieces::empty || (_board[row - 1][col - 2] & pieces::black)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 2)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 2 > -1) {
		if (col + 1 < 8) {
			if (_board[row - 2][col + 1] == pieces::empty || (_board[row - 2][col + 1] & pieces::black)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col + 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if (_board[row - 2][col - 1] == pieces::empty || (_board[row - 2][col - 1] & pieces::black)) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col - 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}
}

inline void Board::_get_black_knight_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	if (row + 2 < 8) {
		if (col + 1 < 8) {
			if ((_board[row + 2][col + 1] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col + 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if ((_board[row + 2][col - 1] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 2), static_cast<uint8_t>(col - 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}
	if (row + 1 < 8) {
		if (col + 2 < 8) {
			if ((_board[row + 1][col + 2] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 2)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if ((_board[row + 1][col - 2] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 2)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 1 > -1) {
		if (col + 2 < 8) {
			if ((_board[row - 1][col + 2] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 2)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 2 > -1) {
			if ((_board[row - 1][col - 2] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 2)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}	
	if (row - 2 > -1) {
		if (col + 1 < 8) {
			if ((_board[row - 2][col + 1] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col + 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if ((_board[row - 2][col - 1] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 2), static_cast<uint8_t>(col - 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}
}

inline void Board::_get_white_bishop_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row < 7 && col < 7) {
			++row;
			++col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if (_board[row][col] & pieces::black) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row < 7 && col > 0) {
			++row;
			--col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if (_board[row][col] & pieces::black) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row > 0 && col < 7) {
			--row;
			++col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if (_board[row][col] & pieces::black) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row > 0 && col > 0) {
			--row;
			--col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if (_board[row][col] & pieces::black) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_black_bishop_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row < 7 && col < 7) {
			++row;
			++col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if ((_board[row][col] & pieces::black) == 0) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row < 7 && col > 0) {
			++row;
			--col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if ((_board[row][col] & pieces::black) == 0) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row > 0 && col < 7) {
			--row;
			++col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if ((_board[row][col] & pieces::black) == 0) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		auto old_col = col;
		while (row > 0 && col > 0) {
			--row;
			--col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if ((_board[row][col] & pieces::black) == 0) {
				moves.emplace_back(move::Move{old_row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_white_rook_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		while (row < 7) {
			++row;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if (_board[row][col] & pieces::black) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		while (row > 0) {
			--row;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if (_board[row][col] & pieces::black) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_col = col;
		while (col < 7) {
			++col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if (_board[row][col] & pieces::black) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_col = col;
		while (col > 0) {
			--col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if (_board[row][col] & pieces::black) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_black_rook_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		while (row < 7) {
			++row;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if ((_board[row][col] & pieces::black) == 0) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_row = row;
		while (row > 0) {
			--row;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if ((_board[row][col] & pieces::black) == 0) {
				moves.emplace_back(move::Move{old_row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_col = col;
		while (col < 7) {
			++col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if ((_board[row][col] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);

	[this, &moves, &boards](uint8_t row, uint8_t col) -> void {
		auto old_col = col;
		while (col > 0) {
			--col;
			if (_board[row][col] == pieces::empty) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
			} else if ((_board[row][col] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, old_col, static_cast<uint8_t>(row), static_cast<uint8_t>(col)});
				boards.emplace_back(Board{*this, moves.back()});
				return;
			}	else {
				return;
			}
		}
	}(row, col);
}

inline void Board::_get_white_king_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	if (row + 1 < 8) {
		if ((_board[row + 1][col] | pieces::black) != 0 && _board[row + 1][col] == pieces::empty) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col)});
			boards.emplace_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if ((_board[row + 1][col + 1] | pieces::black) != 0 && _board[row + 1][col + 1] == pieces::empty) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if ((_board[row + 1][col - 1] | pieces::black) != 0 && _board[row + 1][col - 1] == pieces::empty) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}

	if (col + 1 < 8) {
		if ((_board[row][col + 1] | pieces::black) != 0 && _board[row][col + 1] == pieces::empty) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col + 1)});
			boards.emplace_back(Board{*this, moves.back()});
		}
	}

	if (col - 1 > -1) {
		if ((_board[row][col - 1] | pieces::black) != 0 && _board[row][col - 1] == pieces::empty) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col - 1)});
			boards.emplace_back(Board{*this, moves.back()});
		}
	}

	if (row - 1 > -1) {
		if ((_board[row - 1][col] | pieces::black) != 0 && _board[row - 1][col] == pieces::empty) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col)});
			boards.emplace_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if ((_board[row - 1][col + 1] | pieces::black) != 0 && _board[row - 1][col + 1] == pieces::empty) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if ((_board[row - 1][col - 1] | pieces::black) != 0 && _board[row - 1][col - 1] == pieces::empty) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}
}	

inline void Board::_get_black_king_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const {
	if (row + 1 < 8) {
		if ((_board[row + 1][col] & pieces::black) == 0) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col)});
			boards.emplace_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if ((_board[row + 1][col + 1] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col + 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if ((_board[row + 1][col - 1] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row + 1), static_cast<uint8_t>(col - 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}

	if (col + 1 < 8) {
		if ((_board[row][col + 1] & pieces::black) == 0) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col + 1)});
			boards.emplace_back(Board{*this, moves.back()});
		}
	}

	if (col - 1 > -1) {
		if ((_board[row][col - 1] & pieces::black) == 0) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row), static_cast<uint8_t>(col - 1)});
			boards.emplace_back(Board{*this, moves.back()});
		}
	}

	if (row - 1 > -1) {
		if ((_board[row - 1][col] & pieces::black) == 0) {
			moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col)});
			boards.emplace_back(Board{*this, moves.back()});
		}
		if (col + 1 < 8) {
			if ((_board[row - 1][col + 1] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col + 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
		if (col - 1 > -1) {
			if ((_board[row - 1][col - 1] & pieces::black) == 0) {
				moves.emplace_back(move::Move{row, col, static_cast<uint8_t>(row - 1), static_cast<uint8_t>(col - 1)});
				boards.emplace_back(Board{*this, moves.back()});
			}
		}
	}
}

