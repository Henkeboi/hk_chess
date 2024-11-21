#ifndef __ZOBRIST__HPP__
#define __ZOBRIST__HPP__

#include <random>
#include "board.hpp"

class Zobrist {
	public:
	Zobrist(const Board& board, const bool white_to_move);
	uint64_t get_initial_zobrist_hash() const;
	uint64_t new_zobrist_hash(const Board& board_to_hash, const Board& previous_board, const move::Move& last_move, uint64_t previous_zobrist_hash) const;
	private:
	std::mt19937_64::result_type _get_random_number();
	void _init_zobrist_hash(const Board& board, const bool white_to_move);
	std::array<std::array<std::array<uint64_t, 8>, 8>,12> _piece_positions_bits;
	std::array<uint64_t, 8> _en_passant_files_bits;
	uint64_t _can_white_castle_queen_side_bits;
	uint64_t _can_white_castle_king_side_bits;
	uint64_t _can_black_castle_queen_side_bits;
	uint64_t _can_black_castle_king_side_bits;
	uint64_t _white_to_move_bits;
	uint64_t _initial_zobrist_hash;
	std::uniform_int_distribution<std::mt19937_64::result_type> _uniform_distribution;
	std::mt19937_64 _rng;
	const uint8_t _white_pawn_index = 0;
	const uint8_t _white_knight_index = 1;
	const uint8_t _white_bishop_index = 2;
	const uint8_t _white_rook_index = 3;
	const uint8_t _white_queen_index = 4;
	const uint8_t _white_king_index = 5;
	const uint8_t _black_pawn_index = 6;
	const uint8_t _black_knight_index = 7;
	const uint8_t _black_bishop_index = 8;
	const uint8_t _black_rook_index = 9;
	const uint8_t _black_queen_index = 10;
	const uint8_t _black_king_index = 11;
};

#endif // __ZOBRIST__HPP__
