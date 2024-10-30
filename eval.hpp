#ifndef __EVAL__HPP__
#define __EVAL__HPP__

#include "board.hpp"
#include "move.hpp"
#include "stdint.h"
#include <memory>

namespace eval {
	int eval(const Board& board);
}

#endif
