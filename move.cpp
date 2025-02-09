#include "move.hpp"
#include <cassert>
#include <iostream>

namespace move {

Move::Move(uint8_t from_row, uint8_t from_col, uint8_t to_row, uint8_t to_col, uint8_t promotion_piece, bool is_en_passant, bool enables_en_passant)
: _from(0), _to(0), _promotion_piece(promotion_piece) {
	assert(from_row < 8);
	assert(from_col < 8);
	assert(to_row < 8);
	assert(to_col < 8);

	if (is_en_passant) 
		_from |= is_en_passant_bitmask;
	if (enables_en_passant) 
		_from |= enables_en_passant_bitmask;
	
	_from |= from_row << 3;
	_from |= from_col;
	_to |= to_row << 3;
	_to |= to_col; 
}

Move::Move(const Move& other)
: _from(other._from), _to(other._to), _promotion_piece(other.get_promotion_piece()) {
}

Move::Move(const std::string move) :
	_from(0), _to(0), _promotion_piece(0) {
	if (move.size() == 5) {
		if (move[4] == 'k') {
			_promotion_piece = pieces::black | pieces::knight;
		} else if (move[4] == 'b') {
			_promotion_piece = pieces::black | pieces::bishop;
		} else if (move[4] == 'r') {
			_promotion_piece = pieces::black | pieces::rook;
		} else if (move[4] == 'q') {
			_promotion_piece = pieces::black | pieces::queen;
		}
	}
	_from |= uint8_t(move[1] - 49) << 3;
	_from |= uint8_t(move[0] - 97);
	_to |= uint8_t(move[3] - 49) << 3;
	_to |= uint8_t(move[2] - 97); 
}

void Move::print() const {
	std::cout << static_cast<int>(get_from_row()) << ", " << static_cast<int>(get_from_col()) << ", " << static_cast<int>(get_to_row()) << ", " << static_cast<int>(get_to_col()) << "\n";
}

std::string Move::get_move_as_string() const {
	std::string move_as_string = "";

	move_as_string += char(97 + this->get_from_col());
	move_as_string += char(49 + this->get_from_row());
	move_as_string += char(97 + this->get_to_col());
	move_as_string += char(49 + this->get_to_row());

	return move_as_string;
}

move::Move& Move::operator=(const Move& other) {
	_from = other._from;
	_to = other._to;
	_promotion_piece = other._promotion_piece;
	return *this;
}

move::Move& Move::operator=(Move& other) {
	_from = other._from;
	_to = other._to;
	_promotion_piece = other.get_promotion_piece();
	return *this;
}

bool Move::operator==(const Move& other) const {
	if (_to != other._to)
		return false;
	if (_from != other._from)
		return false;
	if (get_promotion_piece() != other.get_promotion_piece()) 
		return false;
	return true;
}

}
