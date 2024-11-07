#ifndef __ALPHA__BETA__HPP__
#define __ALPHA__BETA__HPP__

#include "board.hpp"
#include <limits.h>

namespace alpha_beta {
	int alpha_beta(const Board& board, int depth, bool maximizing_player, move::Move& best_move, const int start_depth, int alpha=INT_MIN, int beta=INT_MAX);

}

#endif // __MINMAX__HPP__
