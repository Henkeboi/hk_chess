#ifndef __ALPHA__BETA__HPP__
#define __ALPHA__BETA__HPP__

#include "board.hpp"
#include <limits.h>
#include <thread>

namespace alpha_beta {
	int alpha_beta(const Board& board, int depth, bool maximizing_player, move::Move& best_move, const int start_depth, std::stop_token& stop_token, int alpha=INT_MIN, int beta=INT_MAX);
	int alpha_beta_with_timeout(const Board& board, int depth, bool maximizing_player, move::Move& best_move, int timeout);
}

#endif // __MINMAX__HPP__
