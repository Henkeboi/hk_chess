#ifndef BOARD_HPP_
#define BOARD_HPP_

#include "move.hpp"

#include <vector>
#include <array>
#include <string>

class Board {
public:
	Board();
	Board(std::string position);
	Board(const Board& board, const move::Move& move);
	void print() const;
	void get_white_moves(std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle=true) const;
	void get_black_moves(std::vector<move::Move>& moves, std::vector<Board>& boards, bool only_captures, bool check_castle=true) const;
	[[nodiscard]] bool white_in_checkmate(const std::vector<move::Move>& moves) const;
	[[nodiscard]] bool black_in_checkmate(const std::vector<move::Move>& moves) const;
	[[nodiscard]] bool is_checkmate(bool white_to_move) const;
	[[nodiscard]] const std::array<std::array<uint8_t, 8>, 8>& get_raw_board() const;
	[[nodiscard]] const move::Move& get_last_move() const;
	[[nodiscard]] bool can_white_castle_queen_side() const;
	[[nodiscard]] bool can_white_castle_king_side() const;
	[[nodiscard]] bool can_black_castle_queen_side() const;
	[[nodiscard]] bool can_black_castle_king_side() const;
	[[nodiscard]] bool operator==(const Board& rhs) const;
private:
	[[nodiscard]] inline bool _square_has_white_piece(const uint8_t row, const uint8_t col) const;
	[[nodiscard]] inline bool _square_has_black_piece(const uint8_t row, const uint8_t col) const;
	[[nodiscard]] inline bool _square_is_empty(const uint8_t row, const uint8_t col) const;
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
	move::Move _last_move;
	bool _can_white_castle_queen_side;
	bool _can_white_castle_king_side;
	bool _can_black_castle_queen_side;
	bool _can_black_castle_king_side;
};

#endif 
