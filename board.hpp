#ifndef __BOARD__HPP__
#define __BOARD__HPP__


#include "move.hpp"
#include "pieces.hpp"

#include <vector>
#include <array>

class Board {
public:
	Board();
	Board(const Board& board, const move::Move& move = move::Move{0, 0, 0, 0}, const pieces::piece& promotion_piece = 0);
	void print() const;
	void get_white_moves(std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	void get_black_moves(std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	bool white_in_checkmate(const std::vector<move::Move>& moves) const;
	bool black_in_checkmate(const std::vector<move::Move>& moves) const;
	bool is_game_finished(bool white_to_move) const;
	const std::array<std::array<uint8_t, 8>, 8>& get_raw_board() const;
	const move::Move& get_en_passant() const;
private:
	inline bool _square_has_white_piece(const uint8_t row, const uint8_t col) const;
	inline void _get_white_pawn_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	inline void _get_white_knight_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	inline void _get_white_bishop_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_white_rook_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_white_king_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_black_pawn_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captuer) const;
	inline void _get_black_knight_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	inline void _get_black_bishop_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_black_rook_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	inline void _get_black_king_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards) const;
	std::array<std::array<uint8_t, 8>, 8> _board;
	move::Move _en_passant;
};

#endif 
