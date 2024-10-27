#include "move.hpp"

namespace move {

Move::Move(uint8_t from_row, uint8_t from_col, uint8_t to_row, uint8_t to_col)
: _from(0), _to(0) {
	_from |= from_row << 3;
	_from |= from_col;
	_to |= to_row << 3;
	_to |= to_col; 
}

Move::Move(const Move& move)
: _from(0), _to(0) {
	_from |= move.get_from_row() << 3;
	_from |= move.get_from_col();
	_to |= move.get_to_row() << 3;
	_to |= move.get_to_col(); 
}

}
