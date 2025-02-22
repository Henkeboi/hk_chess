import sys
from stockfish import Stockfish
from pathlib import Path


def main():
	stockfish = Stockfish(path="stockfish")
	move_sequence = sys.argv[1:]
	stockfish.set_elo_rating(1600)
	stockfish.update_engine_parameters({"Minimum Thinking Time": 2})
	stockfish.set_position(move_sequence)
	filename = Path('best_move.txt')
	filename.touch(exist_ok=True)
	with open("best_move.txt", "w") as f:
		move = stockfish.get_best_move()
		if move:
			f.write(move)
		else:
			f.write("")


if __name__ == '__main__':
	main()
