#ifndef MOVE_HPP_
#define MOVE_HPP_

#include "pieces.hpp"
#include <string>

namespace {
	const uint8_t col_bitmask = 0b00000111;
	const uint8_t row_bitmask = 0b00111000;
	const uint8_t is_en_passant_bitmask = 0b10000000;
	const uint8_t enables_en_passant_bitmask = 0b01000000;
}

class Move {
public:
	Move(uint8_t from_row, uint8_t from_col, uint8_t to_row, uint8_t to_col, uint8_t promotion_piece=pieces::empty, bool is_en_passant=false, bool enables_en_passant=false);
	Move(const Move& other);
	Move(const std::string move);
	Move& operator=(const Move& other);
	Move& operator=(Move& other);
	bool operator==(const Move& other) const;
	~Move() = default;
	void print() const;
	std::string get_move_as_string() const;
	[[nodiscard]] inline auto get_from_row() const { return (_from & row_bitmask) >> 3; }
	[[nodiscard]] inline auto get_from_col() const { return _from & col_bitmask; }
	[[nodiscard]] inline auto get_to_row() const { return (_to & row_bitmask) >> 3; }
	[[nodiscard]] inline auto get_to_col() const { return _to & col_bitmask; }
	[[nodiscard]] inline auto get_promotion_piece() const { return _promotion_piece; }
	[[nodiscard]] inline auto is_en_passant() const { return (_from & is_en_passant_bitmask) >> 7; }
	[[nodiscard]] inline auto enables_en_passant() const { return (_from & enables_en_passant_bitmask) >> 6; }
private:
	// _from: First bit represents that the move is en passant, second bit represents that the move enables en passant,
	// bits third to fifth gives the row and bits sixth to eight gives the col.
	uint8_t _from;
	// _to: Bits third to fifth gives the row and bits sixth to eight gives the col.
	uint8_t _to;
	uint8_t _promotion_piece;
};

#endif
