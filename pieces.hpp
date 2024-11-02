#ifndef __PIECES__HPP__
#define __PIECES__HPP__

#include <stdint.h>

namespace pieces {
	typedef const uint8_t color;
	typedef const uint8_t piece;

	piece empty = 0;
	piece pawn = 1;
	piece knight = 2;
	piece bishop = 3;
	piece rook = 4;
	piece queen = 5;
	piece king = 6;

	color white = 0;
	color black = 8;
	
	const int pawn_value = 1;
	const int knight_value = 3;
	const int bishop_value = 3;
	const int rook_value = 5;
	const int queen_value = 9;
	const int king_value = 1000000;
};

#endif 
