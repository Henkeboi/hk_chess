#ifndef __SEARCH__HPP__
#define __SEARCH__HPP__

#include "board.hpp"
#include "zobrist.hpp"
#include "robin_map/robin_map.h"

namespace {
	class SearchStatistics {
		public:
		SearchStatistics();
		bool timeout_exceeded;
		int evaluation;
		int positions_evaluated;
		int positions_generated;
	};
}

namespace search {
	
	class Search {
	public:
		Search(const Board& board, bool maximizing_player);
		move::Move find_next_move(const Board& board, bool maximizing_player);

	private:
		Board _board;	
		Zobrist _zobrist_hasher;
		uint64_t _last_zobrist_hash;
		tsl::robin_map<uint64_t, uint8_t> _position_repeat_counter;
		int _white_search_depth;
		int _black_search_depth;
	}; 

	void search(const Board& board, int depth, bool maximizing_player, move::Move& best_move, uint64_t board_zobrist_hash, Zobrist& zobrist_hasher, tsl::robin_map<uint64_t, uint8_t> position_repeat_counter);
}

#endif // __MINMAX__HPP__
