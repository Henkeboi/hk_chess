#include "zobrist.hpp"
#include <limits>
#include <iostream>


Zobrist::Zobrist(const Board& board, const bool white_to_move)
: _zobrist_hash(0), _uniform_distribution(0, std::numeric_limits<uint64_t>::max()) {
	std::mt19937_64::result_type const seed = 49;
	_rng.seed(seed);

	for (auto piece_index = 0; piece_index < 12; ++piece_index) {
		for (auto row = 0; row < 8; ++row) {
			for (auto col = 0; col < 8; ++col) {
				_piece_positions_bits[piece_index][row][col] = _get_random_number();
			}
		}
	}

	for (auto file_index = 0; file_index < 8; ++file_index) {
		_en_passant_files_bits[file_index] = _get_random_number();	
	}

	_can_white_castle_queen_side_bits = _get_random_number();
	_can_white_castle_king_side_bits = _get_random_number();
	_can_black_castle_queen_side_bits = _get_random_number();
	_can_black_castle_king_side_bits = _get_random_number();
	_white_to_move_bits = _get_random_number();

	_init_zobrist_hash(board, white_to_move);
}

uint64_t Zobrist::get_zobrist_hash() const {
	return _zobrist_hash;
}

uint64_t Zobrist::update_zobrist_hash(const Board& board_to_hash, const Board& previous_board, const move::Move& last_move) {
	switch (board_to_hash.get_raw_board()[last_move.get_to_row()][last_move.get_to_col()]) {
		case (pieces::white | pieces::pawn):
			_zobrist_hash ^= _piece_positions_bits[_white_pawn_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_white_pawn_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
		case (pieces::white | pieces::knight):
			_zobrist_hash ^= _piece_positions_bits[_white_knight_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_white_knight_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
		case (pieces::white | pieces::bishop):
			_zobrist_hash ^= _piece_positions_bits[_white_bishop_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_white_bishop_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
		case (pieces::white | pieces::rook):
			_zobrist_hash ^= _piece_positions_bits[_white_rook_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_white_rook_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
		case (pieces::white | pieces::queen):
			_zobrist_hash ^= _piece_positions_bits[_white_queen_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_white_queen_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
		case (pieces::white | pieces::king):
			_zobrist_hash ^= _piece_positions_bits[_white_king_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_white_king_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
		case (pieces::black | pieces::pawn):
			_zobrist_hash ^= _piece_positions_bits[_black_pawn_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_black_pawn_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
		case (pieces::black | pieces::knight):
			_zobrist_hash ^= _piece_positions_bits[_black_knight_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_black_knight_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
		case (pieces::black | pieces::bishop):
			_zobrist_hash ^= _piece_positions_bits[_black_bishop_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_black_bishop_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
		case (pieces::black | pieces::rook):
			_zobrist_hash ^= _piece_positions_bits[_black_rook_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_black_rook_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
		case (pieces::black | pieces::queen):
			_zobrist_hash ^= _piece_positions_bits[_black_queen_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_black_queen_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
		case (pieces::black | pieces::king):
			_zobrist_hash ^= _piece_positions_bits[_black_king_index][last_move.get_from_row()][last_move.get_from_col()];
			_zobrist_hash ^= _piece_positions_bits[_black_king_index][last_move.get_to_row()][last_move.get_to_col()];
			break;
	}

	_zobrist_hash ^= _white_to_move_bits;
	if (board_to_hash.can_white_castle_queen_side() != previous_board.can_white_castle_queen_side()) 
		_zobrist_hash ^= _can_white_castle_queen_side_bits;
	if (board_to_hash.can_white_castle_king_side() != previous_board.can_white_castle_king_side()) 
		_zobrist_hash ^= _can_white_castle_king_side_bits;
	if (board_to_hash.can_black_castle_queen_side() != previous_board.can_black_castle_queen_side()) 
		_zobrist_hash ^= _can_black_castle_queen_side_bits;
	if (board_to_hash.can_black_castle_king_side() != previous_board.can_black_castle_king_side()) 
		_zobrist_hash ^= _can_black_castle_king_side_bits;
	_zobrist_hash ^=_white_to_move_bits;

	const move::Move to_hash_en_passant = board_to_hash.get_en_passant();
	if (to_hash_en_passant.get_from_row() < 8 && to_hash_en_passant.get_from_col() < 8) {
		if (to_hash_en_passant.get_to_row() < 8 && to_hash_en_passant.get_to_col() < 8) {
			_zobrist_hash ^= _en_passant_files_bits[to_hash_en_passant.get_to_col()];
		}
	}

	const move::Move previous_en_passant = previous_board.get_en_passant();
	if (previous_en_passant.get_from_row() < 8 && previous_en_passant.get_from_col() < 8) {
		if (previous_en_passant.get_to_row() < 8 && previous_en_passant.get_to_col() < 8) {
			_zobrist_hash ^= _en_passant_files_bits[previous_en_passant.get_to_col()];
		}
	}

	return _zobrist_hash;
}

void Zobrist::_init_zobrist_hash(const Board& board, const bool white_to_move)	{
	for (uint8_t row = 0; row < 8; ++row) {
		for (uint8_t col = 0; col < 8; ++col) {
			switch (board.get_raw_board()[row][col]) {
				case (pieces::white | pieces::pawn):
					_zobrist_hash ^= _piece_positions_bits[_white_pawn_index][row][col];
					break;
				case (pieces::white | pieces::knight):
					_zobrist_hash ^= _piece_positions_bits[_white_knight_index][row][col];
					break;
				case (pieces::white | pieces::bishop):
					_zobrist_hash ^= _piece_positions_bits[_white_bishop_index][row][col];
					break;
				case (pieces::white | pieces::rook):
					_zobrist_hash ^= _piece_positions_bits[_white_rook_index][row][col];
					break;
				case (pieces::white | pieces::queen):
					_zobrist_hash ^= _piece_positions_bits[_white_queen_index][row][col];
					break;
				case (pieces::white | pieces::king):
					_zobrist_hash ^= _piece_positions_bits[_white_king_index][row][col];
					break;
				case (pieces::black | pieces::pawn):
					_zobrist_hash ^= _piece_positions_bits[_black_pawn_index][row][col];
					break;
				case (pieces::black | pieces::knight):
					_zobrist_hash ^= _piece_positions_bits[_black_knight_index][row][col];
					break;
				case (pieces::black | pieces::bishop):
					_zobrist_hash ^= _piece_positions_bits[_black_bishop_index][row][col];
					break;
				case (pieces::black | pieces::rook):
					_zobrist_hash ^= _piece_positions_bits[_black_rook_index][row][col];
					break;
				case (pieces::black | pieces::queen):
					_zobrist_hash ^= _piece_positions_bits[_black_queen_index][row][col];
					break;
				case (pieces::black | pieces::king):
					_zobrist_hash ^= _piece_positions_bits[_black_king_index][row][col];
					break;
			}
		}

		if (board.can_white_castle_queen_side()) 
			_zobrist_hash ^= _can_white_castle_queen_side_bits;
		if (board.can_white_castle_king_side()) 
			_zobrist_hash ^= _can_white_castle_king_side_bits;
		if (board.can_black_castle_queen_side()) 
			_zobrist_hash ^= _can_black_castle_queen_side_bits;
		if (board.can_black_castle_king_side()) 
			_zobrist_hash ^= _can_black_castle_king_side_bits;
		if (white_to_move) 
			_zobrist_hash ^= _white_to_move_bits;

		const move::Move en_passant = board.get_en_passant();
		if (en_passant.get_from_row() < 8 && en_passant.get_from_col() < 8) {
			if (en_passant.get_to_row() < 8 && en_passant.get_to_col() < 8) {
				_zobrist_hash ^= _en_passant_files_bits[en_passant.get_to_col()];
			}
		}
	}
}

std::mt19937_64::result_type Zobrist::_get_random_number() {
	std::mt19937_64::result_type random_number = _uniform_distribution(_rng);
	return random_number;
}
