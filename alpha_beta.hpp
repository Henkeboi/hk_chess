#ifndef __ALPHA__BETA__HPP__
#define __ALPHA__BETA__HPP__

#include "board.hpp"
#include <memory>
#include <vector>

namespace alpha_beta {
	int alpha_beta(Board board, int depth, float alpha, float beta, bool maximizing_player, move::Move& best_move, const int start_depth);

}

#endif // __MINMAX__HPP__
