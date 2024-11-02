#ifndef __MOVE__HPP__
#define __MOVE__HPP__

#include <stdint.h>

namespace move {

	class Move {
	public:
		Move(uint8_t from_row, uint8_t from_col, uint8_t to_row, uint8_t to_col);
		inline uint8_t get_from_row() const { return _from >> 3; }
		inline uint8_t get_from_col() const { return _from & 0b000111; }
		inline uint8_t get_to_row() const { return _to >> 3; }
		inline uint8_t get_to_col() const { return _to & 0b000111; }
	private:
		uint8_t _from;
		uint8_t _to;
	};
}

#endif
