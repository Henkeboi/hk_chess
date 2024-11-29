#include "../../board.hpp"
#include "../../zobrist.hpp"
#include <gtest/gtest.h>
#include <iostream>

class HashStatistics {
	public:
	int positions_evaluated;
	int duplicate_hashes;
};

void test_unique_zobrist_hash();
void move_search(int depth, const Board& board, bool maximizing_player, std::map<uint64_t, Board>& transposition_table, uint64_t board_hash, const Zobrist zobrist_hasher, HashStatistics& stats);

TEST(ZobrisTest, test_hashing) {
	test_unique_zobrist_hash();
}

void test_unique_zobrist_hash() {
	Board board = Board{};
	board = Board{board, move::Move(1, 4, 3, 4, 0, false, true)};
	board = Board{board, move::Move(6, 4, 4, 4, 0, false, true)};
	board = Board{board, move::Move(0, 6, 2, 5, 0, false, true)};
	const Zobrist zobrist_hasher(board, false);
	std::map<uint64_t, Board> transposition_table{};
	uint64_t initial_zobrist_hash = zobrist_hasher.get_initial_zobrist_hash();
	HashStatistics stats;

	int depth = 5;
	move_search(depth, board, true, transposition_table, initial_zobrist_hash, zobrist_hasher, stats);
	EXPECT_EQ(stats.duplicate_hashes, 0);
}

void move_search(int depth, const Board& board, bool maximizing_player, std::map<uint64_t, Board>& transposition_table, uint64_t board_hash, const Zobrist zobrist_hasher, HashStatistics& stats) {
	if (depth == 0) 
		return;

	std::vector<move::Move> moves;
 	std::vector<Board> boards;
	if (maximizing_player) {
		board.get_white_moves(moves, boards, false);
		for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
 			uint64_t new_zobrist_hash = zobrist_hasher.new_zobrist_hash(boards[i], board, moves[i], board_hash);
			stats.positions_evaluated += 1;
			if (transposition_table.find(new_zobrist_hash) == transposition_table.end()) {
				transposition_table[new_zobrist_hash] = boards[i]; 
			} else {
				if (boards[i] != transposition_table[new_zobrist_hash]) {
					stats.duplicate_hashes += 1;
					moves[i].print();
				}
			}
			move_search(depth - 1, boards[i], !maximizing_player, transposition_table, new_zobrist_hash, zobrist_hasher, stats);
		}
	} else {
		board.get_black_moves(moves, boards, false);
		for (size_t i = 0; i < moves.size() && i < boards.size(); ++i) {
 			uint64_t new_zobrist_hash = zobrist_hasher.new_zobrist_hash(boards[i], board, moves[i], board_hash);
			stats.positions_evaluated += 1;
			if (transposition_table.find(new_zobrist_hash) == transposition_table.end()) {
				transposition_table[new_zobrist_hash] = boards[i]; 
			} else {
				if (boards[i] != transposition_table[new_zobrist_hash]) {
					stats.duplicate_hashes += 1;
				}
			}
			move_search(depth - 1, boards[i], !maximizing_player, transposition_table, new_zobrist_hash, zobrist_hasher, stats);
		}
	}
}
