#include "zobrist.hpp"
#include <limits>
#include <map>
#include <cassert>
#include <iostream>

Zobrist::Zobrist(const Board& board, const bool white_to_move)
: _initial_zobrist_hash(0), _uniform_distribution(0, std::numeric_limits<uint64_t>::max()) {
	std::mt19937_64::result_type const seed = 0xa4535b34434526;
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

	assert(are_bit_strings_unique());

	_init_zobrist_hash(board, white_to_move);
}

uint64_t Zobrist::get_initial_zobrist_hash() const {
	return _initial_zobrist_hash;
}

uint64_t Zobrist::new_zobrist_hash(const Board& board_to_hash, const Board& prev_board, const move::Move& last_move, uint64_t prev_zobrist_hash) const {
	auto xor_hash_with_piece_value = [this, &prev_zobrist_hash](const Board& board, uint8_t row, uint8_t col) {
		switch (board.get_raw_board()[row][col]) {
			case (pieces::white | pieces::pawn):
				prev_zobrist_hash ^= _piece_positions_bits[_white_pawn_index][row][col];
				break;
			case (pieces::white | pieces::knight):
				prev_zobrist_hash ^= _piece_positions_bits[_white_knight_index][row][col];
				break;
			case (pieces::white | pieces::bishop):
				prev_zobrist_hash ^= _piece_positions_bits[_white_bishop_index][row][col];
				break;
			case (pieces::white | pieces::rook):
				prev_zobrist_hash ^= _piece_positions_bits[_white_rook_index][row][col];
				break;
			case (pieces::white | pieces::queen):
				prev_zobrist_hash ^= _piece_positions_bits[_white_queen_index][row][col];
				break;
			case (pieces::white | pieces::king):
				prev_zobrist_hash ^= _piece_positions_bits[_white_king_index][row][col];
				break;
			case (pieces::black | pieces::pawn):
				prev_zobrist_hash ^= _piece_positions_bits[_black_pawn_index][row][col];
				break;
			case (pieces::black | pieces::knight):
				prev_zobrist_hash ^= _piece_positions_bits[_black_knight_index][row][col];
				break;
			case (pieces::black | pieces::bishop):
				prev_zobrist_hash ^= _piece_positions_bits[_black_bishop_index][row][col];
				break;
			case (pieces::black | pieces::rook):
				prev_zobrist_hash ^= _piece_positions_bits[_black_rook_index][row][col];
				break;
			case (pieces::black | pieces::queen):
				prev_zobrist_hash ^= _piece_positions_bits[_black_queen_index][row][col];
				break;
			case (pieces::black | pieces::king):
				prev_zobrist_hash ^= _piece_positions_bits[_black_king_index][row][col];
				break;
		}
	};

	xor_hash_with_piece_value(prev_board, last_move.get_from_row(), last_move.get_from_col());
	xor_hash_with_piece_value(board_to_hash, last_move.get_to_row(), last_move.get_to_col());
	if (prev_board.get_raw_board()[last_move.get_to_row()][last_move.get_to_col()] != pieces::empty) {
		xor_hash_with_piece_value(prev_board, last_move.get_to_row(), last_move.get_to_col());
	}

	prev_zobrist_hash ^= _white_to_move_bits;
	if (board_to_hash.can_white_castle_queen_side() != prev_board.can_white_castle_queen_side()) 
		prev_zobrist_hash ^= _can_white_castle_queen_side_bits;
	if (board_to_hash.can_white_castle_king_side() != prev_board.can_white_castle_king_side()) 
		prev_zobrist_hash ^= _can_white_castle_king_side_bits;
	if (board_to_hash.can_black_castle_queen_side() != prev_board.can_black_castle_queen_side()) 
		prev_zobrist_hash ^= _can_black_castle_queen_side_bits;
	if (board_to_hash.can_black_castle_king_side() != prev_board.can_black_castle_king_side()) 
		prev_zobrist_hash = _can_black_castle_king_side_bits ^ prev_zobrist_hash;
	
	if (last_move.enables_en_passant() == true)
		prev_zobrist_hash ^= _en_passant_files_bits[last_move.get_from_col()];

	const move::Move prev_move = prev_board.get_last_move();
	if (prev_move.enables_en_passant() == true)
		prev_zobrist_hash ^= _en_passant_files_bits[prev_move.get_from_col()];

	return prev_zobrist_hash;
}

bool Zobrist::are_bit_strings_unique() const {
	std::map<uint64_t, int> bit_strings_counter;

	auto add_to_bit_strings_counter = [&bit_strings_counter](uint64_t bit_string) {
		if (bit_strings_counter.find(bit_string) == bit_strings_counter.end()) {
			bit_strings_counter[bit_string] = 1;
		} else {
			bit_strings_counter[bit_string] += 1;
		}
	};

	for (auto piece_index = 0; piece_index < 12; ++piece_index) {
		for (auto row = 0; row < 8; ++row) {
			for (auto col = 0; col < 8; ++col) {
				add_to_bit_strings_counter(_piece_positions_bits[piece_index][row][col]);
			}
		}
	}

	for (auto col = 0; col < 8; ++col) {
		add_to_bit_strings_counter(_en_passant_files_bits[col]);
	}
	
	add_to_bit_strings_counter(_white_to_move_bits);
	add_to_bit_strings_counter(_can_white_castle_queen_side_bits);
	add_to_bit_strings_counter(_can_white_castle_king_side_bits);
	add_to_bit_strings_counter(_can_black_castle_queen_side_bits);
	add_to_bit_strings_counter(_can_black_castle_king_side_bits);

	for (auto [bit_string_key, counter] : bit_strings_counter) {
		if (counter > 1) {
			return false;
		}		
	}
	return true;
}

void Zobrist::_init_zobrist_hash(const Board& board, const bool white_to_move)	{
	for (uint8_t row = 0; row < 8; ++row) {
		for (uint8_t col = 0; col < 8; ++col) {
			switch (board.get_raw_board()[row][col]) {
				case (pieces::white | pieces::pawn):
					_initial_zobrist_hash ^= _piece_positions_bits[_white_pawn_index][row][col];
					break;
				case (pieces::white | pieces::knight):
					_initial_zobrist_hash ^= _piece_positions_bits[_white_knight_index][row][col];
					break;
				case (pieces::white | pieces::bishop):
					_initial_zobrist_hash ^= _piece_positions_bits[_white_bishop_index][row][col];
					break;
				case (pieces::white | pieces::rook):
					_initial_zobrist_hash ^= _piece_positions_bits[_white_rook_index][row][col];
					break;
				case (pieces::white | pieces::queen):
					_initial_zobrist_hash ^= _piece_positions_bits[_white_queen_index][row][col];
					break;
				case (pieces::white | pieces::king):
					_initial_zobrist_hash ^= _piece_positions_bits[_white_king_index][row][col];
					break;
				case (pieces::black | pieces::pawn):
					_initial_zobrist_hash ^= _piece_positions_bits[_black_pawn_index][row][col];
					break;
				case (pieces::black | pieces::knight):
					_initial_zobrist_hash ^= _piece_positions_bits[_black_knight_index][row][col];
					break;
				case (pieces::black | pieces::bishop):
					_initial_zobrist_hash ^= _piece_positions_bits[_black_bishop_index][row][col];
					break;
				case (pieces::black | pieces::rook):
					_initial_zobrist_hash ^= _piece_positions_bits[_black_rook_index][row][col];
					break;
				case (pieces::black | pieces::queen):
					_initial_zobrist_hash ^= _piece_positions_bits[_black_queen_index][row][col];
					break;
				case (pieces::black | pieces::king):
					_initial_zobrist_hash ^= _piece_positions_bits[_black_king_index][row][col];
					break;
			}
		}
	}

	if (board.can_white_castle_queen_side()) 
		_initial_zobrist_hash ^= _can_white_castle_queen_side_bits;
	if (board.can_white_castle_king_side()) 
		_initial_zobrist_hash ^= _can_white_castle_king_side_bits;
	if (board.can_black_castle_queen_side()) 
		_initial_zobrist_hash ^= _can_black_castle_queen_side_bits;
	if (board.can_black_castle_king_side()) 
		_initial_zobrist_hash ^= _can_black_castle_king_side_bits;
	if (white_to_move) 
		_initial_zobrist_hash ^= _white_to_move_bits;

	const move::Move last_move = board.get_last_move();
	if (last_move.is_en_passant() == true)
			_initial_zobrist_hash ^= _en_passant_files_bits[last_move.get_to_col()];
}

std::mt19937_64::result_type Zobrist::_get_random_number() {
	std::mt19937_64::result_type random_number = _uniform_distribution(_rng);
	return random_number;
}
