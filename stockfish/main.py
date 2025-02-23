import sys
from stockfish import Stockfish
from pathlib import Path
import requests


def main():
	stockfish = Stockfish(path="stockfish")
	move_sequence = sys.argv[1:]
	stockfish.set_elo_rating(1500)
	stockfish.set_position(move_sequence)
	filename = Path('best_move.txt')
	filename.touch(exist_ok=True)

	with open("best_move.txt", "w") as f:
		move = stockfish.get_best_move()
		move_str = str(ord(move[1]) - 49) + ","
		move_str += str(ord(move[0]) - 97) + ","
		move_str += str(ord(move[3]) - 49) + ","
		move_str += str(ord(move[2]) - 97) + ","
		move_str +=  " "

		requests.post("http://localhost:3000/make-move", json={"move": move_str})

		if move:
			f.write(move)
		else:
			f.write("")


if __name__ == '__main__':
	main()
