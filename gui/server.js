const express = require('express');
const path = require('path');
const bodyParser = require('body-parser');
const cors = require('cors')
const app = express();
const port = 3000;

app.use(bodyParser.json());
app.use('/public', express.static('public'));
app.use(cors())

app.get('/', function(req, res){
    res.sendFile(__dirname + '/public/index.html');
});

let can_white_castle_queen_side = true;
let can_white_castle_king_side = true;
let can_black_castle_queen_side = true;
let can_black_castle_king_side = true;
let white_to_move = true;
let en_passant_x = -1;
let board = [
    ['R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'],
    ['P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'],
    ['', '', '', '', '', '', '', ''],
    ['', '', '', '', '', '', '', ''],
    ['', '', '', '', '', '', '', ''],
    ['', '', '', '', '', '', '', ''],
    ['p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'],
    ['r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'],
    [can_white_castle_queen_side, can_white_castle_king_side, can_black_castle_queen_side,
        can_black_castle_king_side, en_passant_x, white_to_move]
];
let last_move = null;

app.get('/board', function(req, res) {
    return res.json({
        'data': board,
        success: true
    })
});

app.get('/last-move', function(req, res) {
    return res.json({
        'data': last_move,
        success: true
    });
});

app.post('/make-move', (req, res) => {
    if (make_move(req.body.move) == 1) {
        last_move = req.body.move;
    }
    res.json({ success: true });
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});

function make_move(move) {
    from_col = Number(move[2]);
    from_row = Number(move[0]);
    to_col = Number(move[6]);
    to_row = Number(move[4]);
    promotion_piece = move[8];

    if (white_to_move) {
        if (board[from_row][from_col] !== board[from_row][from_col].toUpperCase()) {
            console.log("Illegal move");
            return 0;
        }
        if (board[to_row][to_col] !== '') {
            if (board[to_row][to_col] !== board[to_row][to_col].toLowerCase()) {
                console.log("Illegal move");
                return 0;
            }
        }
    } else {
        if (board[from_row][from_col] !== board[from_row][from_col].toLowerCase()) {
            console.log("Illegal move");
            return 0;
        }
        if (board[to_row][to_col] !== '') {
            if (board[to_row][to_col] !== board[to_row][to_col].toUpperCase()) {
                console.log("Illegal move");
                return 0;
            }
        }
    }

    white_to_move = !white_to_move;
	// Update castling rights
	if (from_row == 0 && from_col == 0) {
		can_white_castle_queen_side = false;
	} else if (from_row == 0 && from_col == 7) {
		can_white_castle_king_side = false;
	} else if (from_row == 0 && from_col == 4) {
		can_white_castle_queen_side = false;
		can_white_castle_king_side = false;
	} else if (from_row == 7 && from_col == 0) {
		can_black_castle_queen_side = false;
	} else if (from_row == 7 && from_col == 7) {
		can_black_castle_king_side = false;
	} else if (from_row == 7 && from_col == 4) {
		can_black_castle_queen_side = false;
		can_black_castle_king_side = false;
	}

	// Move rook if castling
	if (board[from_row][from_col] == 'K' || board[from_row][from_col] == 'k') {
        if (from_col - to_col == 2) { // Queen side castle
            board[from_row][3] = board[from_row][0];
            board[from_row][0] = '';
		} else if (to_col - from_col == 2) { // King side castle
			board[from_row][5] = board[from_row][7];
			board[from_row][7] = '';
		}
	}
    if (promotion_piece !== ' ') {
		board[to_row][to_col] = promotion_piece;
		board[from_row][from_col] = '';
    } else {
        board[to_row][to_col] = board[from_row][from_col];
        board[from_row][from_col] = '';
    }
    return 1;
}