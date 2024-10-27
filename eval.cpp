#include "eval.hpp"
#include "pieces.hpp"

#include <print>

float eval::eval(const std::array<std::array<uint8_t, 8>, 8>& board) {
	float eval = 0;
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			uint8_t piece = board[row][col];
			if (piece >= pieces::black) {
				eval -= static_cast<float>((0b0111 & piece));
			} else {
				eval += static_cast<float>(piece);
			}
		}
	}
	return eval;
}
