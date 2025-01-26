#ifndef __SEARCH__HPP__
#define __SEARCH__HPP__

#include "board.hpp"
#include "zobrist.hpp"
#include <map>

namespace {
	class SearchStatistics {
		public:
		SearchStatistics();
		bool timeout_exceeded;
		int positions_evaluated;
		int positions_generated;
	};
}


namespace search {
	void search(const Board board, int depth, bool maximizing_player, Zobrist zobrist_hasher, std::map<uint64_t, uint8_t> position_repeat_counter, uint64_t zobrist_hash, move::Move& best_move);
}

#endif // __MINMAX__HPP__
