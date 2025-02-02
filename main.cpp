#include "board.hpp"
#include "search.hpp"
#include "visualization.hpp"
#include "zobrist.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <fstream>

bool check_if_threefold_repetition(std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash);
void inspect_position();
void play_stockfish();

int main() {
	//inspect_position();
	play_stockfish();
	Board board{};
	move::Move best_move{0, 0, 0, 0};
	int white_depth = 4;
	int black_depth = 4;
	bool maximizing_player = true;
	bool is_game_finished = false;

	Zobrist zobrist_hasher{board, maximizing_player};
	uint64_t zobrist_hash = zobrist_hasher.get_initial_zobrist_hash();
	std::map<uint64_t, uint8_t> position_repeat_counter{};
	while (!is_game_finished) {
		visualization::update_visualization(board);
		if (maximizing_player) {
			search::search(board, white_depth, maximizing_player, zobrist_hasher, position_repeat_counter, zobrist_hash, best_move);
		} else {
			search::search(board, black_depth, maximizing_player, zobrist_hasher, position_repeat_counter, zobrist_hash, best_move);
		}

		Board prev_board = board;
		board = Board{board, best_move};
		std::cout << best_move.get_move_as_string() << "\n";
		zobrist_hash = zobrist_hasher.new_zobrist_hash(board, prev_board, best_move, zobrist_hash);
		if (check_if_threefold_repetition(position_repeat_counter, zobrist_hash)) {
			std::cout << "Threefold repetition\n";
			break;
		}
		
		maximizing_player = !maximizing_player;
	}
	visualization::update_visualization(board);

  return 0;
}

bool check_if_threefold_repetition(std::map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash) {
	if (position_repeat_counter.find(zobrist_hash) == position_repeat_counter.end()) {
		position_repeat_counter[zobrist_hash] = 1;
	} else {
		position_repeat_counter[zobrist_hash] += 1;
		if (position_repeat_counter[zobrist_hash] == 3) {
			return true;
		}
	}
	return false;
}

void inspect_position() {
	std::ifstream position_file;
  position_file.open("positions/1.txt");
	std::string position = "";
	std::getline(position_file, position);
	position_file.close();
	Board board{position};
	visualization::update_visualization(board);

	move::Move best_move{0, 0, 0, 0};
	int depth = 4;
	bool maximizing_player = false;
	Zobrist zobrist_hasher{board, maximizing_player};
	uint64_t zobrist_hash = zobrist_hasher.get_initial_zobrist_hash();
	std::map<uint64_t, uint8_t> position_repeat_counter{};
	search::search(board, depth, maximizing_player, zobrist_hasher, position_repeat_counter, zobrist_hash, best_move);
	board = Board{board, best_move};
	visualization::update_visualization(board);

	exit(0);
}


std::string get_best_move() {
	std::ifstream f("best_move.txt");
	std::string best_move;
	getline(f, best_move);
	f.close();
	return best_move;
}

void play_stockfish() {
	Board board{};
	move::Move best_move{0, 0, 0, 0};
	int white_depth = 4;
	bool maximizing_player = true;
	bool is_game_finished = false;

	Zobrist zobrist_hasher{board, maximizing_player};
	uint64_t zobrist_hash = zobrist_hasher.get_initial_zobrist_hash();
	std::map<uint64_t, uint8_t> position_repeat_counter{};
	std::vector<std::string> move_sequence;
	while (!is_game_finished) {
		visualization::update_visualization(board);
		if (maximizing_player) {
			search::search(board, white_depth, maximizing_player, zobrist_hasher, position_repeat_counter, zobrist_hash, best_move);
			std::cout << "White move: ";
		} else {
			std::string command = "source venv/bin/activate; python main.py ";
			for (auto move_made : move_sequence) {
				command += move_made.c_str() + std::string{" "};
			}
			system(command.c_str());
			best_move = move::Move(get_best_move());
			std::cout << "Black move: ";
		}
		std::cout << best_move.get_move_as_string() << "\n";
		move_sequence.push_back(best_move.get_move_as_string());

		Board prev_board = board;
		board = Board{board, best_move};
		zobrist_hash = zobrist_hasher.new_zobrist_hash(board, prev_board, best_move, zobrist_hash);
		if (check_if_threefold_repetition(position_repeat_counter, zobrist_hash)) {
			std::cout << "Threefold repetition\n";
			break;
		}
		
		maximizing_player = !maximizing_player;
	}
	visualization::update_visualization(board);

	exit(0);
}




