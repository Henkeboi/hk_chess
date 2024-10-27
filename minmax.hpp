#ifndef __MINMAX__HPP__
#define __MINMAX__HPP__

#include "board.hpp"

namespace minmax {
	float minmax(const Board& board, int depth, bool maximizing_player);

}

#endif // __MINMAX__HPP__
