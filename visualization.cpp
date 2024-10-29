#include "visualization.hpp"
#include <map>
#include <string>
#include <fstream>
#include "pieces.hpp"
#include <stdint.h>
#include <print>

#include <iostream>

static std::map<std::string, int> coordinate_to_line_number = {
	{"00", 108}, {"01", 109}, {"02", 110}, {"03", 111},
	{"04", 112}, {"05", 113}, {"06", 114}, {"07", 115},

	{"10", 97}, {"11", 98}, {"12", 99}, {"13", 100},
	{"14", 101}, {"15", 102}, {"16", 103}, {"17", 104},

	{"20", 86}, {"21", 87}, {"22", 88}, {"23", 89},
	{"24", 90}, {"25", 91}, {"26", 92}, {"27", 93},

	{"30", 75}, {"31", 76}, {"32", 77}, {"33", 78},
	{"34", 79}, {"35", 80}, {"36", 81}, {"37", 82},

	{"40", 64}, {"41", 65}, {"42", 66}, {"43", 67},
	{"44", 68}, {"45", 69}, {"46", 70}, {"47", 71},

	{"50", 53}, {"51", 54}, {"52", 55}, {"53", 56},
	{"54", 57}, {"55", 58}, {"56", 59}, {"57", 60},

	{"60", 42}, {"61", 43}, {"62", 44}, {"63", 45},
	{"64", 46}, {"65", 47}, {"66", 48}, {"67", 49},

	{"70", 31}, {"71", 32}, {"72", 33}, {"73", 34},
	{"74", 35}, {"75", 36}, {"76", 37}, {"77", 38},
};

static std::map<uint8_t, std::string> piece_to_image = { 
	{pieces::empty, std::string("empty")},
	{pieces::pawn, std::string("white_pawn.png")},
	{pieces::knight, std::string("white_knight.png")},
	{pieces::bishop, std::string("white_bishop.png")},
	{pieces::rook, std::string("white_rook.png")},
	{pieces::queen, std::string("white_queen.png")},
	{pieces::king, std::string("white_king.png")},

	{(pieces::pawn | pieces::black), std::string("black_pawn.png")},
	{(pieces::knight | pieces::black), std::string("black_knight.png")},
	{(pieces::bishop | pieces::black), std::string("black_bishop.png")},
	{(pieces::rook | pieces::black), std::string("black_rook.png")},
	{(pieces::queen | pieces::black), std::string("black_queen.png")},
	{(pieces::king | pieces::black), std::string("black_king.png")},
};

void replace_line(std::string image, int line_number) {
	std::string filename = "visualization/visualization.html";
	std::ifstream input_file(filename);
	std::vector<std::string> lines;
	std::string input;

	while (std::getline(input_file, input)) {
			lines.push_back(input);
	}

	int line_counter = 1;
	for (auto& line : lines) {
			if (line_counter == line_number) {
				if (line.find("dark") != std::string::npos) {
					line.erase(line.begin() + 37, line.end());
				} else if (line.find("light") != std::string::npos) {
					line.erase(line.begin() + 38, line.end());
				}
				if (image == "empty") {	
					line += std::string("<img src=\"") + image + std::string("\" height=\"0px\" width=\"0px\" class=\"center\"></img></td>");
				} else {
					line += std::string("<img src=\"") + image + std::string("\" height=\"75px\" width=\"80px\" class=\"center\"></img></td>");
				}
			}
			++line_counter;
	}
	input_file.close();
	std::ofstream output_file(filename);
	for (auto const& line : lines) {
			output_file << line << '\n';
	}
	output_file.close();
}

void visualization::update_visualization(const Board& board) {
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			int line_number = coordinate_to_line_number[std::to_string(row) + std::to_string(col)];
			std::string image = piece_to_image[board.get_board()[row][col]];
			replace_line(image, line_number);
		}
	}
}
