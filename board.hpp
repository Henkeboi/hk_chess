#ifndef __BOARD__HPP__
#define __BOARD__HPP__


#include "move.hpp"

#include <stdint.h>
#include <vector>
#include <array>

class Board {
public:
	Board();
	Board(const Board& board, const move::Move& move);
	void print() const;
	void get_white_moves(std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	void get_black_moves(std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	bool white_in_checkmate(std::vector<move::Move>& moves) const;
	bool black_in_checkmate(std::vector<move::Move>& moves) const;
	const std::array<std::array<uint8_t, 8>, 8>& get_board() const;
	const move::Move& get_en_passant() const;
private:
	inline void _get_white_pawn_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_white_knight_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_white_bishop_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_white_rook_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_white_king_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_black_pawn_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_black_knight_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_black_bishop_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_black_rook_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_black_king_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;

		std::array<std::array<uint8_t, 8>, 8> _board;

	move::Move _en_passant;
};

#endif 
