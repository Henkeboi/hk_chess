#ifndef PIECES_HPP_
#define PIECES_HPP_

#include <cstdint>

namespace pieces {
	using color = const uint8_t;
	using piece = const uint8_t;

	piece empty = 0;
	piece pawn = 1;
	piece knight = 2;
	piece bishop = 3;
	piece rook = 4;
	piece queen = 5;
	piece king = 6;

	color white = 0;
	color black = 8;
	
	const int pawn_value = 100;
	const int knight_value = 320;
	const int bishop_value = 330;
	const int rook_value = 500;
	const int queen_value = 900;
	const int king_value = 2000;
};

#endif 
