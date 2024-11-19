#ifndef __BOARD__HPP__
#define __BOARD__HPP__

#include "move.hpp"

#include <vector>
#include <array>

class Board {
public:
	Board();
	Board(const Board& board, const move::Move& move);
	void print() const;
	void get_white_moves(std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle=true) const;
	void get_black_moves(std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle=true) const;
	bool white_in_checkmate(const std::vector<move::Move>& moves) const;
	bool black_in_checkmate(const std::vector<move::Move>& moves) const;
	bool is_game_finished(bool white_to_move) const;
	const std::array<std::array<uint8_t, 8>, 8>& get_raw_board() const;
	const move::Move& get_en_passant() const;
	bool can_white_castle_queen_side() const;
	bool can_white_castle_king_side() const;
	bool can_black_castle_queen_side() const;
	bool can_black_castle_king_side() const;
private:
	inline bool _square_has_white_piece(const uint8_t row, const uint8_t col) const;
	inline bool _square_has_black_piece(const uint8_t row, const uint8_t col) const;
	inline bool _square_is_empty(const uint8_t row, const uint8_t col) const;
	inline void _get_white_pawn_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	inline void _get_white_knight_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	inline void _get_white_bishop_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	inline void _get_white_rook_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	inline void _get_white_king_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle) const;
	inline void _get_black_pawn_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captuer) const;
	inline void _get_black_knight_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	inline void _get_black_bishop_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	inline void _get_black_rook_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures) const;
	inline void _get_black_king_moves(uint8_t row, uint8_t col, std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle) const;
	std::array<std::array<uint8_t, 8>, 8> _board;
	move::Move _en_passant;
	bool _can_white_castle_queen_side;
	bool _can_white_castle_king_side;
	bool _can_black_castle_queen_side;
	bool _can_black_castle_king_side;
};

#endif 
