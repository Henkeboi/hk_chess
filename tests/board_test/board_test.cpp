#include "../../board.hpp"
#include <gtest/gtest.h>

void test_find_moves_first_position(Board board);
Board test_make_move_first_position(Board board);
void test_find_moves_second_position(Board board);
Board test_make_move_second_position(Board board);
void test_find_moves_third_position(Board board);
Board test_make_move_third_position(Board board);
void test_find_moves_fourth_position(Board board);
Board test_make_move_fourth_position(Board board);
void test_find_moves_fifth_position(Board board);
Board test_make_move_fifth_position(Board board);
void test_find_moves_sixth_position(Board board);
Board test_make_move_sixth_position(Board board);
void test_find_moves_seventh_position(Board board);
Board test_make_move_seventh_position(Board board);

TEST(BoardTest, find_and_make_moves_test) {
	Board board{};

	test_find_moves_first_position(board);
	Move first_move{1, 4, 3, 4, pieces::empty, false, true};
	board = Board{board, first_move};
	board = test_make_move_first_position(board);

	test_find_moves_second_position(board);
	Move second_move{6, 4, 4, 4, pieces::empty, false, true};
	board = Board{board, second_move};
	board = test_make_move_second_position(board);

	test_find_moves_third_position(board);
	Move third_move{0, 6, 2, 5};
	board = Board{board, third_move};
	board = test_make_move_third_position(board);

	test_find_moves_fourth_position(board);
	Move fourth_move{7, 1, 5, 2};
	board = Board{board, fourth_move};
	board = test_make_move_fourth_position(board);

	test_find_moves_fifth_position(board);
	Move fifth_move{0, 5, 4, 1};
	board = Board{board, fifth_move};
	board = test_make_move_fifth_position(board);

	test_find_moves_sixth_position(board);
	Move sixth_move{7, 6, 5, 5};
	board = Board{board, sixth_move};
	board = test_make_move_sixth_position(board);

	test_find_moves_seventh_position(board);
	//Move sixth_move{7, 6, 5, 5};
	//board = Board{board, sixth_move};
	//board = test_make_move_sixth_position(board);
}

void test_find_moves_first_position(Board board) {
	// Find all moves
	std::vector<Board> boards_found_after_white_move;
	std::vector<Move> white_moves_found;
	board.get_white_moves(white_moves_found, boards_found_after_white_move, false);

	std::vector<Move> expected_white_moves;
	expected_white_moves.push_back(Move{0, 1, 2, 2});
	expected_white_moves.push_back(Move{0, 1, 2, 0});
	expected_white_moves.push_back(Move{0, 6, 2, 7});
	expected_white_moves.push_back(Move{0, 6, 2, 5});
	expected_white_moves.push_back(Move{1, 0, 2, 0});
	expected_white_moves.push_back(Move{1, 0, 3, 0});
	expected_white_moves.push_back(Move{1, 1, 2, 1});
	expected_white_moves.push_back(Move{1, 1, 3, 1});
	expected_white_moves.push_back(Move{1, 2, 2, 2});
	expected_white_moves.push_back(Move{1, 2, 3, 2});
	expected_white_moves.push_back(Move{1, 3, 2, 3});
	expected_white_moves.push_back(Move{1, 3, 3, 3});
	expected_white_moves.push_back(Move{1, 4, 2, 4});
	expected_white_moves.push_back(Move{1, 4, 3, 4});
	expected_white_moves.push_back(Move{1, 5, 2, 5});
	expected_white_moves.push_back(Move{1, 5, 3, 5});
	expected_white_moves.push_back(Move{1, 6, 2, 6});
	expected_white_moves.push_back(Move{1, 6, 3, 6});
	expected_white_moves.push_back(Move{1, 7, 2, 7});
	expected_white_moves.push_back(Move{1, 7, 3, 7});
	
	EXPECT_EQ(expected_white_moves.size(), white_moves_found.size());
	for (size_t i = 0; i < expected_white_moves.size(); ++i) {
		EXPECT_EQ(expected_white_moves[i].get_from_row(), white_moves_found[i].get_from_row());
		EXPECT_EQ(expected_white_moves[i].get_from_col(), white_moves_found[i].get_from_col());
		EXPECT_EQ(expected_white_moves[i].get_to_row(), white_moves_found[i].get_to_row());
		EXPECT_EQ(expected_white_moves[i].get_to_col(), white_moves_found[i].get_to_col());
		EXPECT_EQ(expected_white_moves[i].get_promotion_piece(), white_moves_found[i].get_promotion_piece());
	}

	// Find captures
	std::vector<Board> boards_found_after_white_capture;
	std::vector<Move> white_captures_found;
	board.get_white_moves(white_captures_found, boards_found_after_white_move, true);
	std::vector<Move> expected_white_captures;
	EXPECT_EQ(expected_white_captures.size(), white_captures_found.size());
}

Board test_make_move_first_position(const Board board) {
	// En passant tracking
	Move expected_en_passant{1, 4, 3, 4, pieces::empty, false, true};
	EXPECT_EQ(expected_en_passant.is_en_passant(), board.get_last_move().is_en_passant());
	EXPECT_EQ(expected_en_passant.enables_en_passant(), board.get_last_move().enables_en_passant());
	EXPECT_EQ(expected_en_passant.get_promotion_piece(), board.get_last_move().get_promotion_piece());

	// Piece movement
	pieces::piece expected_white_pawn = pieces::pawn | pieces::white;
	pieces::piece expected_empty = pieces::empty;
	EXPECT_EQ(expected_white_pawn, board.get_raw_board()[3][4]);
	EXPECT_EQ(expected_empty, board.get_raw_board()[1][4]);

	// Castling rights
	EXPECT_TRUE(board.can_white_castle_queen_side());
	EXPECT_TRUE(board.can_white_castle_king_side());
	EXPECT_TRUE(board.can_black_castle_queen_side());
	EXPECT_TRUE(board.can_black_castle_king_side());

	return board;
}

void test_find_moves_second_position(const Board board) {
	// Find all moves
	std::vector<Board> boards_found_after_black_move;
	std::vector<Move> black_moves_found;
	board.get_black_moves(black_moves_found, boards_found_after_black_move, false);

	std::vector<Move> expected_black_moves;
	expected_black_moves.push_back(Move{6, 0, 5, 0});
	expected_black_moves.push_back(Move{6, 0, 4, 0});
	expected_black_moves.push_back(Move{6, 1, 5, 1});
	expected_black_moves.push_back(Move{6, 1, 4, 1});
	expected_black_moves.push_back(Move{6, 2, 5, 2});
	expected_black_moves.push_back(Move{6, 2, 4, 2});
	expected_black_moves.push_back(Move{6, 3, 5, 3});
	expected_black_moves.push_back(Move{6, 3, 4, 3});
	expected_black_moves.push_back(Move{6, 4, 5, 4});
	expected_black_moves.push_back(Move{6, 4, 4, 4});
	expected_black_moves.push_back(Move{6, 5, 5, 5});
	expected_black_moves.push_back(Move{6, 5, 4, 5});
	expected_black_moves.push_back(Move{6, 6, 5, 6});
	expected_black_moves.push_back(Move{6, 6, 4, 6});
	expected_black_moves.push_back(Move{6, 7, 5, 7});
	expected_black_moves.push_back(Move{6, 7, 4, 7});
	
	expected_black_moves.push_back(Move{7, 1, 5, 2});
	expected_black_moves.push_back(Move{7, 1, 5, 0});
	expected_black_moves.push_back(Move{7, 6, 5, 7});
	expected_black_moves.push_back(Move{7, 6, 5, 5});

	EXPECT_EQ(expected_black_moves.size(), black_moves_found.size());
	for (size_t i = 0; i < expected_black_moves.size(); ++i) {
		EXPECT_EQ(expected_black_moves[i].get_from_row(), black_moves_found[i].get_from_row());
		EXPECT_EQ(expected_black_moves[i].get_from_col(), black_moves_found[i].get_from_col());
		EXPECT_EQ(expected_black_moves[i].get_to_row(), black_moves_found[i].get_to_row());
		EXPECT_EQ(expected_black_moves[i].get_to_col(), black_moves_found[i].get_to_col());
		EXPECT_EQ(expected_black_moves[i].get_promotion_piece(), black_moves_found[i].get_promotion_piece());
	}

	// Find captures
	std::vector<Board> boards_found_after_black_capture;
	std::vector<Move> black_captures_found;
	board.get_black_moves(black_captures_found, boards_found_after_black_move, true);
	std::vector<Move> expected_black_captures;
	EXPECT_EQ(expected_black_captures.size(), black_captures_found.size());
}

Board test_make_move_second_position(const Board board) {
	// En passant tracking
	Move expected_en_passant{6, 4, 4, 4, pieces::empty, false, true};
	EXPECT_EQ(expected_en_passant.is_en_passant(), board.get_last_move().is_en_passant());
	EXPECT_EQ(expected_en_passant.enables_en_passant(), board.get_last_move().enables_en_passant());
	EXPECT_EQ(expected_en_passant.get_promotion_piece(), board.get_last_move().get_promotion_piece());
	
	// Piece movement
	pieces::piece expected_black_pawn = pieces::pawn | pieces::black;
	pieces::piece expected_empty = pieces::empty;
	EXPECT_EQ(expected_black_pawn, board.get_raw_board()[4][4]);
	EXPECT_EQ(expected_empty, board.get_raw_board()[6][4]);

	// Castling rights
	EXPECT_TRUE(board.can_white_castle_queen_side());
	EXPECT_TRUE(board.can_white_castle_king_side());
	EXPECT_TRUE(board.can_black_castle_queen_side());
	EXPECT_TRUE(board.can_black_castle_king_side());

	return board;
}

void test_find_moves_third_position(Board board) {
	// Find all moves
	std::vector<Board> boards_found_after_white_move;
	std::vector<Move> white_moves_found;
	board.get_white_moves(white_moves_found, boards_found_after_white_move, false);

	std::vector<Move> expected_white_moves;
	expected_white_moves.push_back(Move{0, 1, 2, 2});
	expected_white_moves.push_back(Move{0, 1, 2, 0});
	expected_white_moves.push_back(Move{0, 3, 1, 4});
	expected_white_moves.push_back(Move{0, 3, 2, 5});
	expected_white_moves.push_back(Move{0, 3, 3, 6});
	expected_white_moves.push_back(Move{0, 3, 4, 7});
	expected_white_moves.push_back(Move{0, 4, 1, 4});
	expected_white_moves.push_back(Move{0, 5, 1, 4});
	expected_white_moves.push_back(Move{0, 5, 2, 3});
	expected_white_moves.push_back(Move{0, 5, 3, 2});
	expected_white_moves.push_back(Move{0, 5, 4, 1});
	expected_white_moves.push_back(Move{0, 5, 5, 0});
	expected_white_moves.push_back(Move{0, 6, 2, 7});
	expected_white_moves.push_back(Move{0, 6, 2, 5});
	expected_white_moves.push_back(Move{0, 6, 1, 4});

	expected_white_moves.push_back(Move{1, 0, 2, 0});
	expected_white_moves.push_back(Move{1, 0, 3, 0});
	expected_white_moves.push_back(Move{1, 1, 2, 1});
	expected_white_moves.push_back(Move{1, 1, 3, 1});
	expected_white_moves.push_back(Move{1, 2, 2, 2});
	expected_white_moves.push_back(Move{1, 2, 3, 2});
	expected_white_moves.push_back(Move{1, 3, 2, 3});
	expected_white_moves.push_back(Move{1, 3, 3, 3});
	expected_white_moves.push_back(Move{1, 5, 2, 5});
	expected_white_moves.push_back(Move{1, 5, 3, 5});
	expected_white_moves.push_back(Move{1, 6, 2, 6});
	expected_white_moves.push_back(Move{1, 6, 3, 6});
	expected_white_moves.push_back(Move{1, 7, 2, 7});
	expected_white_moves.push_back(Move{1, 7, 3, 7});
	
	EXPECT_EQ(expected_white_moves.size(), white_moves_found.size());
	for (size_t i = 0; i < expected_white_moves.size(); ++i) {
		EXPECT_EQ(expected_white_moves[i].get_from_row(), white_moves_found[i].get_from_row());
		EXPECT_EQ(expected_white_moves[i].get_from_col(), white_moves_found[i].get_from_col());
		EXPECT_EQ(expected_white_moves[i].get_to_row(), white_moves_found[i].get_to_row());
		EXPECT_EQ(expected_white_moves[i].get_to_col(), white_moves_found[i].get_to_col());
		EXPECT_EQ(expected_white_moves[i].get_promotion_piece(), white_moves_found[i].get_promotion_piece());
	}

	// Find captures
	std::vector<Board> boards_found_after_white_capture;
	std::vector<Move> white_captures_found;
	board.get_white_moves(white_captures_found, boards_found_after_white_move, true);
	std::vector<Move> expected_white_captures;
	EXPECT_EQ(expected_white_captures.size(), white_captures_found.size());
}

Board test_make_move_third_position(Board board) {
	// En passant tracking
	Move expected_en_passant{0, 0, 0, 0, pieces::empty, false, false};
	EXPECT_EQ(expected_en_passant.is_en_passant(), board.get_last_move().is_en_passant());
	EXPECT_EQ(expected_en_passant.enables_en_passant(), board.get_last_move().enables_en_passant());
	EXPECT_EQ(expected_en_passant.get_promotion_piece(), board.get_last_move().get_promotion_piece());
	
	// Piece movement
	pieces::piece expected_white_knight = pieces::knight | pieces::white;
	pieces::piece expected_empty = pieces::empty;
	EXPECT_EQ(expected_white_knight, board.get_raw_board()[2][5]);
	EXPECT_EQ(expected_empty, board.get_raw_board()[0][6]);

	// Castling rights
	EXPECT_TRUE(board.can_white_castle_queen_side());
	EXPECT_TRUE(board.can_white_castle_king_side());
	EXPECT_TRUE(board.can_black_castle_queen_side());
	EXPECT_TRUE(board.can_black_castle_king_side());

	return board;
}

void test_find_moves_fourth_position(const Board board) {
	// Find all moves
	std::vector<Board> boards_found_after_black_move;
	std::vector<Move> black_moves_found;
	board.get_black_moves(black_moves_found, boards_found_after_black_move, false);

	std::vector<Move> expected_black_moves;
	expected_black_moves.push_back(Move{6, 0, 5, 0});
	expected_black_moves.push_back(Move{6, 0, 4, 0});
	expected_black_moves.push_back(Move{6, 1, 5, 1});
	expected_black_moves.push_back(Move{6, 1, 4, 1});
	expected_black_moves.push_back(Move{6, 2, 5, 2});
	expected_black_moves.push_back(Move{6, 2, 4, 2});
	expected_black_moves.push_back(Move{6, 3, 5, 3});
	expected_black_moves.push_back(Move{6, 3, 4, 3});
	expected_black_moves.push_back(Move{6, 5, 5, 5});
	expected_black_moves.push_back(Move{6, 5, 4, 5});
	expected_black_moves.push_back(Move{6, 6, 5, 6});
	expected_black_moves.push_back(Move{6, 6, 4, 6});
	expected_black_moves.push_back(Move{6, 7, 5, 7});
	expected_black_moves.push_back(Move{6, 7, 4, 7});
	expected_black_moves.push_back(Move{7, 1, 5, 2});
	expected_black_moves.push_back(Move{7, 1, 5, 0});
	expected_black_moves.push_back(Move{7, 3, 6, 4});
	expected_black_moves.push_back(Move{7, 3, 5, 5});
	expected_black_moves.push_back(Move{7, 3, 4, 6});
	expected_black_moves.push_back(Move{7, 3, 3, 7});
	expected_black_moves.push_back(Move{7, 4, 6, 4});
	expected_black_moves.push_back(Move{7, 5, 6, 4});
	expected_black_moves.push_back(Move{7, 5, 5, 3});
	expected_black_moves.push_back(Move{7, 5, 4, 2});
	expected_black_moves.push_back(Move{7, 5, 3, 1});
	expected_black_moves.push_back(Move{7, 5, 2, 0});
	expected_black_moves.push_back(Move{7, 6, 6, 4});
	expected_black_moves.push_back(Move{7, 6, 5, 7});
	expected_black_moves.push_back(Move{7, 6, 5, 5});

	EXPECT_EQ(expected_black_moves.size(), black_moves_found.size());
	for (size_t i = 0; i < expected_black_moves.size(); ++i) {
		EXPECT_EQ(expected_black_moves[i].get_from_row(), black_moves_found[i].get_from_row());
		EXPECT_EQ(expected_black_moves[i].get_from_col(), black_moves_found[i].get_from_col());
		EXPECT_EQ(expected_black_moves[i].get_to_row(), black_moves_found[i].get_to_row());
		EXPECT_EQ(expected_black_moves[i].get_to_col(), black_moves_found[i].get_to_col());
		EXPECT_EQ(expected_black_moves[i].get_promotion_piece(), black_moves_found[i].get_promotion_piece());
	}

	// Find captures
	std::vector<Board> boards_found_after_black_capture;
	std::vector<Move> black_captures_found;
	board.get_black_moves(black_captures_found, boards_found_after_black_move, true);
	std::vector<Move> expected_black_captures;
	EXPECT_EQ(expected_black_captures.size(), black_captures_found.size());
}

Board test_make_move_fourth_position(Board board) {
	// En passant tracking
	Move expected_en_passant{0, 0, 0, 0, pieces::empty, false, false};
	EXPECT_EQ(expected_en_passant.is_en_passant(), board.get_last_move().is_en_passant());
	EXPECT_EQ(expected_en_passant.enables_en_passant(), board.get_last_move().enables_en_passant());
	EXPECT_EQ(expected_en_passant.get_promotion_piece(), board.get_last_move().get_promotion_piece());
		
	// Piece movement
	pieces::piece expected_black_knight = pieces::knight | pieces::black;
	pieces::piece expected_empty = pieces::empty;
	EXPECT_EQ(expected_black_knight, board.get_raw_board()[5][2]);
	EXPECT_EQ(expected_empty, board.get_raw_board()[7][1]);

	// Castling rights
	EXPECT_TRUE(board.can_white_castle_queen_side());
	EXPECT_TRUE(board.can_white_castle_king_side());
	EXPECT_TRUE(board.can_black_castle_queen_side());
	EXPECT_TRUE(board.can_black_castle_king_side());

	return board;
}

void test_find_moves_fifth_position(Board board) {
	// Find all moves
	std::vector<Board> boards_found_after_white_move;
	std::vector<Move> white_moves_found;
	board.get_white_moves(white_moves_found, boards_found_after_white_move, false);

	std::vector<Move> expected_white_moves;
	expected_white_moves.push_back(Move{0, 1, 2, 2});
	expected_white_moves.push_back(Move{0, 1, 2, 0});
	expected_white_moves.push_back(Move{0, 3, 1, 4});
	expected_white_moves.push_back(Move{0, 4, 1, 4});
	expected_white_moves.push_back(Move{0, 5, 1, 4});
	expected_white_moves.push_back(Move{0, 5, 2, 3});
	expected_white_moves.push_back(Move{0, 5, 3, 2});
	expected_white_moves.push_back(Move{0, 5, 4, 1});
	expected_white_moves.push_back(Move{0, 5, 5, 0});
	expected_white_moves.push_back(Move{0, 7, 0, 6});
	expected_white_moves.push_back(Move{1, 0, 2, 0});
	expected_white_moves.push_back(Move{1, 0, 3, 0});
	expected_white_moves.push_back(Move{1, 1, 2, 1});
	expected_white_moves.push_back(Move{1, 1, 3, 1});
	expected_white_moves.push_back(Move{1, 2, 2, 2});
	expected_white_moves.push_back(Move{1, 2, 3, 2});
	expected_white_moves.push_back(Move{1, 3, 2, 3});
	expected_white_moves.push_back(Move{1, 3, 3, 3});
	expected_white_moves.push_back(Move{1, 6, 2, 6});
	expected_white_moves.push_back(Move{1, 6, 3, 6});
	expected_white_moves.push_back(Move{1, 7, 2, 7});
	expected_white_moves.push_back(Move{1, 7, 3, 7});
	expected_white_moves.push_back(Move{2, 5, 4, 6});
	expected_white_moves.push_back(Move{2, 5, 4, 4});
	expected_white_moves.push_back(Move{2, 5, 3, 7});
	expected_white_moves.push_back(Move{2, 5, 3, 3});
	expected_white_moves.push_back(Move{2, 5, 0, 6});
	
	EXPECT_EQ(expected_white_moves.size(), white_moves_found.size());
	for (size_t i = 0; i < expected_white_moves.size(); ++i) {
		EXPECT_EQ(expected_white_moves[i].get_from_row(), white_moves_found[i].get_from_row());
		EXPECT_EQ(expected_white_moves[i].get_from_col(), white_moves_found[i].get_from_col());
		EXPECT_EQ(expected_white_moves[i].get_to_row(), white_moves_found[i].get_to_row());
		EXPECT_EQ(expected_white_moves[i].get_to_col(), white_moves_found[i].get_to_col());
		EXPECT_EQ(expected_white_moves[i].get_promotion_piece(), white_moves_found[i].get_promotion_piece());
	}

	// Find captures
	std::vector<Board> boards_found_after_white_capture;
	std::vector<Move> white_captures_found;
	board.get_white_moves(white_captures_found, boards_found_after_white_move, true);

	std::vector<Move> expected_white_captures;
	expected_white_captures.push_back(Move{2, 5, 4, 4});

	EXPECT_EQ(expected_white_captures.size(), white_captures_found.size());
	for (size_t i = 0; i < expected_white_moves.size(); ++i) {
		EXPECT_EQ(expected_white_moves[i].get_from_row(), white_moves_found[i].get_from_row());
		EXPECT_EQ(expected_white_moves[i].get_from_col(), white_moves_found[i].get_from_col());
		EXPECT_EQ(expected_white_moves[i].get_to_row(), white_moves_found[i].get_to_row());
		EXPECT_EQ(expected_white_moves[i].get_to_col(), white_moves_found[i].get_to_col());
		EXPECT_EQ(expected_white_moves[i].get_promotion_piece(), white_moves_found[i].get_promotion_piece());
	}
}

Board test_make_move_fifth_position(Board board) {
	// En passant tracking
	Move expected_en_passant{0, 0, 0, 0, pieces::empty, false, false};
	EXPECT_EQ(expected_en_passant.is_en_passant(), board.get_last_move().is_en_passant());
	EXPECT_EQ(expected_en_passant.enables_en_passant(), board.get_last_move().enables_en_passant());
	EXPECT_EQ(expected_en_passant.get_promotion_piece(), board.get_last_move().get_promotion_piece());
	
	// Piece movement
	pieces::piece expected_white_bishop = pieces::bishop | pieces::white;
	pieces::piece expected_empty = pieces::empty;
	EXPECT_EQ(expected_white_bishop, board.get_raw_board()[4][1]);
	EXPECT_EQ(expected_empty, board.get_raw_board()[0][5]);

	// Castling rights
	EXPECT_TRUE(board.can_white_castle_queen_side());
	EXPECT_TRUE(board.can_white_castle_king_side());
	EXPECT_TRUE(board.can_black_castle_queen_side());
	EXPECT_TRUE(board.can_black_castle_king_side());

	return board;
}

void test_find_moves_sixth_position(Board board) {
	// Find all moves
	std::vector<Board> boards_found_after_black_move;
	std::vector<Move> black_moves_found;
	board.get_black_moves(black_moves_found, boards_found_after_black_move, false);

	std::vector<Move> expected_black_moves;
	expected_black_moves.push_back(Move{5, 2, 7, 1});
	expected_black_moves.push_back(Move{5, 2, 6, 4});
	expected_black_moves.push_back(Move{5, 2, 4, 0});
	expected_black_moves.push_back(Move{5, 2, 3, 3});
	expected_black_moves.push_back(Move{5, 2, 3, 1});
	expected_black_moves.push_back(Move{6, 0, 5, 0});
	expected_black_moves.push_back(Move{6, 0, 4, 0});
	expected_black_moves.push_back(Move{6, 1, 5, 1});
	expected_black_moves.push_back(Move{6, 3, 5, 3});
	expected_black_moves.push_back(Move{6, 3, 4, 3});
	expected_black_moves.push_back(Move{6, 5, 5, 5});
	expected_black_moves.push_back(Move{6, 5, 4, 5});
	expected_black_moves.push_back(Move{6, 6, 5, 6});
	expected_black_moves.push_back(Move{6, 6, 4, 6});
	expected_black_moves.push_back(Move{6, 7, 5, 7});
	expected_black_moves.push_back(Move{6, 7, 4, 7});
	expected_black_moves.push_back(Move{7, 0, 7, 1});
	expected_black_moves.push_back(Move{7, 3, 6, 4});
	expected_black_moves.push_back(Move{7, 3, 5, 5});
	expected_black_moves.push_back(Move{7, 3, 4, 6});
	expected_black_moves.push_back(Move{7, 3, 3, 7});
	expected_black_moves.push_back(Move{7, 4, 6, 4});
	expected_black_moves.push_back(Move{7, 5, 6, 4});
	expected_black_moves.push_back(Move{7, 5, 5, 3});
	expected_black_moves.push_back(Move{7, 5, 4, 2});
	expected_black_moves.push_back(Move{7, 5, 3, 1});
	expected_black_moves.push_back(Move{7, 5, 2, 0});
	expected_black_moves.push_back(Move{7, 6, 6, 4});
	expected_black_moves.push_back(Move{7, 6, 5, 7});
	expected_black_moves.push_back(Move{7, 6, 5, 5});

	EXPECT_EQ(expected_black_moves.size(), black_moves_found.size());
	for (size_t i = 0; i < expected_black_moves.size(); ++i) {
		EXPECT_EQ(expected_black_moves[i].get_from_row(), black_moves_found[i].get_from_row());
		EXPECT_EQ(expected_black_moves[i].get_from_col(), black_moves_found[i].get_from_col());
		EXPECT_EQ(expected_black_moves[i].get_to_row(), black_moves_found[i].get_to_row());
		EXPECT_EQ(expected_black_moves[i].get_to_col(), black_moves_found[i].get_to_col());
		EXPECT_EQ(expected_black_moves[i].get_promotion_piece(), black_moves_found[i].get_promotion_piece());
	}

	// Find captures
	std::vector<Board> boards_found_after_black_capture;
	std::vector<Move> black_captures_found;
	board.get_black_moves(black_captures_found, boards_found_after_black_move, true);
	std::vector<Move> expected_black_captures;
	EXPECT_EQ(expected_black_captures.size(), black_captures_found.size());
}

Board test_make_move_sixth_position(Board board) {
	// En passant tracking
	Move expected_en_passant{0, 0, 0, 0, pieces::empty, false, false};
	EXPECT_EQ(expected_en_passant.is_en_passant(), board.get_last_move().is_en_passant());
	EXPECT_EQ(expected_en_passant.enables_en_passant(), board.get_last_move().enables_en_passant());
	EXPECT_EQ(expected_en_passant.get_promotion_piece(), board.get_last_move().get_promotion_piece());
	
	// Piece movement
	pieces::piece expected_black_knight = pieces::knight | pieces::black;
	pieces::piece expected_empty = pieces::empty;
	EXPECT_EQ(expected_black_knight, board.get_raw_board()[5][5]);
	EXPECT_EQ(expected_empty, board.get_raw_board()[7][6]);

	// Castling rights
	EXPECT_TRUE(board.can_white_castle_queen_side());
	EXPECT_TRUE(board.can_white_castle_king_side());
	EXPECT_TRUE(board.can_black_castle_queen_side());
	EXPECT_TRUE(board.can_black_castle_king_side());

	return board;
}


void test_find_moves_seventh_position(Board board) {
	// Find all moves
	std::vector<Board> boards_found_after_white_move;
	std::vector<Move> white_moves_found;
	board.get_white_moves(white_moves_found, boards_found_after_white_move, false);

	std::vector<Move> expected_white_moves;
	expected_white_moves.push_back(Move{0, 1, 2, 2});
	expected_white_moves.push_back(Move{0, 1, 2, 0});
	expected_white_moves.push_back(Move{0, 3, 1, 4});
	expected_white_moves.push_back(Move{0, 4, 1, 4});
	expected_white_moves.push_back(Move{0, 4, 0, 5});
	expected_white_moves.push_back(Move{0, 4, 0, 6});
	expected_white_moves.push_back(Move{0, 7, 0, 6});
	expected_white_moves.push_back(Move{0, 7, 0, 5});
	expected_white_moves.push_back(Move{1, 0, 2, 0});
	expected_white_moves.push_back(Move{1, 0, 3, 0});
	expected_white_moves.push_back(Move{1, 1, 2, 1});
	expected_white_moves.push_back(Move{1, 1, 3, 1});
	expected_white_moves.push_back(Move{1, 2, 2, 2});
	expected_white_moves.push_back(Move{1, 2, 3, 2});
	expected_white_moves.push_back(Move{1, 3, 2, 3});
	expected_white_moves.push_back(Move{1, 3, 3, 3});
	expected_white_moves.push_back(Move{1, 6, 2, 6});
	expected_white_moves.push_back(Move{1, 6, 3, 6});
	expected_white_moves.push_back(Move{1, 7, 2, 7});
	expected_white_moves.push_back(Move{1, 7, 3, 7});
	expected_white_moves.push_back(Move{2, 5, 4, 6});
	expected_white_moves.push_back(Move{2, 5, 4, 4});
	expected_white_moves.push_back(Move{2, 5, 3, 7});
	expected_white_moves.push_back(Move{2, 5, 3, 3});
	expected_white_moves.push_back(Move{2, 5, 0, 6});

	expected_white_moves.push_back(Move{4, 1, 5, 2});
	expected_white_moves.push_back(Move{4, 1, 5, 0});
	expected_white_moves.push_back(Move{4, 1, 3, 2});
	expected_white_moves.push_back(Move{4, 1, 2, 3});
	expected_white_moves.push_back(Move{4, 1, 1, 4});
	expected_white_moves.push_back(Move{4, 1, 0, 5});
	expected_white_moves.push_back(Move{4, 1, 3, 0});

	EXPECT_EQ(expected_white_moves.size(), white_moves_found.size());
	for (size_t i = 0; i < expected_white_moves.size(); ++i) {
		EXPECT_EQ(expected_white_moves[i].get_from_row(), white_moves_found[i].get_from_row());
		EXPECT_EQ(expected_white_moves[i].get_from_col(), white_moves_found[i].get_from_col());
		EXPECT_EQ(expected_white_moves[i].get_to_row(), white_moves_found[i].get_to_row());
		EXPECT_EQ(expected_white_moves[i].get_to_col(), white_moves_found[i].get_to_col());
		EXPECT_EQ(expected_white_moves[i].get_promotion_piece(), white_moves_found[i].get_promotion_piece());
	}

	// Find captures
	std::vector<Board> boards_found_after_white_capture;
	std::vector<Move> white_captures_found;
	board.get_white_moves(white_captures_found, boards_found_after_white_move, true);

	std::vector<Move> expected_white_captures;
	expected_white_captures.push_back(Move{2, 5, 4, 4});
	expected_white_captures.push_back(Move{4, 1, 5, 2});


	EXPECT_EQ(expected_white_captures.size(), white_captures_found.size());
	for (size_t i = 0; i < expected_white_moves.size(); ++i) {
		EXPECT_EQ(expected_white_moves[i].get_from_row(), white_moves_found[i].get_from_row());
		EXPECT_EQ(expected_white_moves[i].get_from_col(), white_moves_found[i].get_from_col());
		EXPECT_EQ(expected_white_moves[i].get_to_row(), white_moves_found[i].get_to_row());
		EXPECT_EQ(expected_white_moves[i].get_to_col(), white_moves_found[i].get_to_col());
		EXPECT_EQ(expected_white_moves[i].get_promotion_piece(), white_moves_found[i].get_promotion_piece());
	}
}
