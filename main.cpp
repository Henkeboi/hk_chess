#include "board.hpp"
#include "search.hpp"
#include "zobrist.hpp"
#include "chess_server.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "robin_map/robin_map.h"
#include <thread>

bool check_if_threefold_repetition(tsl::robin_map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash);
double play_stockfish();
double play_human();

int main() {
	double total_score = 0;
	int games_played_counter = 0;
	while (true) {
		//total_score += play_stockfish();
		total_score += play_human();

		++games_played_counter;
		std::cout << "Total score: " << total_score << "\n";
		std::cout << "Game played: " << games_played_counter << "\n\n";
	}

	return 0;
}

std::string get_best_move() {
	std::string filename = "best_move.txt";
	std::ifstream file_in(filename);
	std::string best_move;
	getline(file_in, best_move);
	file_in.close();
  	std::ofstream file_out(filename);
    file_out << "";
	file_out.close();
	return best_move;
}


double play_human() {
	Board board{};
	Move best_move{0, 0, 0, 0};
	int white_depth = 4;
	bool maximizing_player = true;

	Zobrist zobrist_hasher{board, maximizing_player};
	uint64_t zobrist_hash = zobrist_hasher.get_initial_zobrist_hash();
	tsl::robin_map<uint64_t, uint8_t> position_repeat_counter{};
	std::vector<std::string> move_sequence;
	while (true) {
		if (maximizing_player) {
			bool white_in_checkmate, black_in_checkmate = false;
			std::tie(best_move, white_in_checkmate, black_in_checkmate) = search::search(board, white_depth, maximizing_player,
															 zobrist_hasher, position_repeat_counter, zobrist_hash, best_move);
			std::string command = "curl --header \"Content-Type: application/json\" --request POST --data '{\"move\":\"" + best_move.as_string() + "\"}' http://localhost:3000/make-move";
			system(command.c_str());

			if (white_in_checkmate) {
				std::cout << "White in checkmate\n";
				return 0;
			}
			std::cout << "White move: ";
		} else {
			Move human_move = best_move;
			// For some reason == operator does not work on move !!!
			std::cout << "Waiting for black move\n";
			while (best_move.get_move_as_string() == human_move.get_move_as_string()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				std::string last_move = chess_server_get_last_move();
				uint8_t from_col = std::stoi(last_move.substr(9, 10));
				uint8_t from_row = std::stoi(last_move.substr(11, 12));
				uint8_t to_col = std::stoi(last_move.substr(13, 14));
				uint8_t to_row = std::stoi(last_move.substr(15, 16));
				human_move = Move{from_col, from_row, to_col, to_row, 0, 0};
			}
			best_move = human_move;
			std::cout << "Black move: ";
		}
		std::cout << best_move.as_string() << "\n";
		move_sequence.push_back(best_move.get_move_as_string());

		Board prev_board = board;
		board = Board{board, best_move};
		zobrist_hash = zobrist_hasher.new_zobrist_hash(board, prev_board, best_move, zobrist_hash);
		if (check_if_threefold_repetition(position_repeat_counter, zobrist_hash)) {
			std::cout << "Threefold repetition\n";
			return 0.5;
		}
		maximizing_player = !maximizing_player;
	}
}


double play_stockfish() {
	Board board{};
	Move best_move{0, 0, 0, 0};
	int white_depth = 4;
	bool maximizing_player = true;

	Zobrist zobrist_hasher{board, maximizing_player};
	uint64_t zobrist_hash = zobrist_hasher.get_initial_zobrist_hash();
	tsl::robin_map<uint64_t, uint8_t> position_repeat_counter{};
	std::vector<std::string> move_sequence;
	while (true) {
		if (maximizing_player) {
			bool white_in_checkmate, black_in_checkmate = false;
			std::tie(best_move, white_in_checkmate, black_in_checkmate) = search::search(board, white_depth, maximizing_player,
															 zobrist_hasher, position_repeat_counter, zobrist_hash, best_move);
			std::string command = "curl --header \"Content-Type: application/json\" --request POST --data '{\"move\":\"" + best_move.as_string() + "\"}' http://localhost:3000/make-move";
			system(command.c_str());

			if (white_in_checkmate) {
				std::cout << "White in checkmate\n";
				return 0;
			}
			std::cout << "White move: ";
		} else {
			std::string command = "source venv/bin/activate; python stockfish/main.py ";
			for (auto move_made : move_sequence) {
				command += move_made.c_str() + std::string{" "};
			}
			system(command.c_str());
			std::string best_move_str = get_best_move();
			if (best_move_str == "") {
				std::cout << "Black in checkmate\n";
				return 1;
			} else {
				best_move = Move(best_move_str);
				std::cout << "Black move: ";
			}
		}
		std::cout << best_move.as_string() << "\n";
		move_sequence.push_back(best_move.get_move_as_string());

		Board prev_board = board;
		board = Board{board, best_move};
		zobrist_hash = zobrist_hasher.new_zobrist_hash(board, prev_board, best_move, zobrist_hash);
		if (check_if_threefold_repetition(position_repeat_counter, zobrist_hash)) {
			std::cout << "Threefold repetition\n";
			return 0.5;
		}
		maximizing_player = !maximizing_player;
	}
}

bool check_if_threefold_repetition(tsl::robin_map<uint64_t, uint8_t>& position_repeat_counter, uint64_t zobrist_hash) {
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
