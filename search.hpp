#ifndef __ALPHA__BETA__HPP__
#define __ALPHA__BETA__HPP__

#include "board.hpp"

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
	void alpha_beta_with_timeout(const Board& board, int depth, bool maximizing_player, move::Move& best_move, int timeout);
}

#endif // __MINMAX__HPP__
