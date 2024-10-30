#ifndef __PIECES__HPP__
#define __PIECES__HPP__

#include <stdint.h>

namespace pieces {
	const uint8_t empty = 0;
	const uint8_t pawn = 1;
	const uint8_t knight = 2;
	const uint8_t bishop = 3;
	const uint8_t rook = 4;
	const uint8_t queen = 5;
	const uint8_t king = 6;

	const uint8_t white = 0;
	const uint8_t black = 8;
	
	const int pawn_value = 1;
	const int knight_value = 3;
	const int bishop_value = 3;
	const int rook_value = 5;
	const int queen_value = 9;
};

#endif 
