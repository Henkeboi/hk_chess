#ifndef __ALPHA__BETA__HPP__
#define __ALPHA__BETA__HPP__

#include "board.hpp"

namespace {
	class search_statistics {
		public:
		search_statistics();
		bool timeout_exceeded;
			
	};
}

namespace search {
	int alpha_beta_with_timeout(const Board& board, int depth, bool maximizing_player, move::Move& best_move, int timeout);
}

#endif // __MINMAX__HPP__
