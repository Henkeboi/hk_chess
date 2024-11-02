#include "move.hpp"

namespace move {

Move::Move(uint8_t from_row, uint8_t from_col, uint8_t to_row, uint8_t to_col)
: _from(0), _to(0) {
	_from |= from_row << 3;
	_from |= from_col;
	_to |= to_row << 3;
	_to |= to_col; 
}



}
