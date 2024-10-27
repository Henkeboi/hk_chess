#ifndef __EVAL__HPP__
#define __EVAL__HPP__

#include "stdint.h"
#include <array>

namespace eval {
	float eval(const std::array<std::array<uint8_t, 8>, 8>& board);
}

#endif
