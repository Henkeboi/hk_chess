#ifndef __MOVE__HPP__
#define __MOVE__HPP__

#include "pieces.hpp"
#include <stdint.h>

namespace move {
	class Move {
	public:
		Move(uint8_t from_row, uint8_t from_col, uint8_t to_row, uint8_t to_col, uint8_t promotion_piece=pieces::empty);
		Move(const Move& other);
		Move& operator=(const Move& other);
		Move& operator=(Move& other);
		inline uint8_t get_from_row() const { return _from >> 3; }
		inline uint8_t get_from_col() const { return _from & 0b000111; }
		inline uint8_t get_to_row() const { return _to >> 3; }
		inline uint8_t get_to_col() const { return _to & 0b000111; }
		inline uint8_t get_promotion_piece() const { return _promotion_piece; }
	private:
		uint8_t _from;
		uint8_t _to;
		uint8_t _promotion_piece;
	};
}

#endif
