#include "move.hpp"
#include <iostream>

namespace move {

Move::Move(uint8_t from_row, uint8_t from_col, uint8_t to_row, uint8_t to_col, uint8_t promotion_piece)
: _from(0), _to(0), _promotion_piece(promotion_piece) {
	_from |= from_row << 3;
	_from |= from_col;
	_to |= to_row << 3;
	_to |= to_col; 
}

Move::Move(const Move& other)
: _from(0), _to(0), _promotion_piece(other.get_promotion_piece()) {
	_from |= other.get_from_row() << 3;
	_from |= other.get_from_col();
	_to |= other.get_to_row() << 3;
	_to |= other.get_to_col(); 
}

void Move::print() const {
	std::cout << static_cast<int>(get_from_row()) << ", " << static_cast<int>(get_from_col()) << ", " << static_cast<int>(get_to_row()) << ", " << static_cast<int>(get_to_col()) << "\n";
}

move::Move& Move::operator=(const Move& other) {
	_from = 0;
	_to = 0;
	_from |= other.get_from_row() << 3;
	_from |= other.get_from_col();
	_to |= other.get_to_row() << 3;
	_to |= other.get_to_col(); 
	_promotion_piece = other.get_promotion_piece();
	return *this;
}

move::Move& Move::operator=(Move& other) {
	_from = 0;
	_to = 0;
	_from |= other.get_from_row() << 3;
	_from |= other.get_from_col();
	_to |= other.get_to_row() << 3;
	_to |= other.get_to_col(); 
	_promotion_piece = other.get_promotion_piece();
	return *this;
}

}
