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



let can_white_castle_queen_side = true 
let can_white_castle_king_side = true 
let can_black_castle_queen_side = true 
let can_black_castle_king_side = true 
let en_passant_x = -1
let board = [
    ['r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'],
    ['p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'],
    ['', '', '', '', '', '', '', ''],
    ['', '', '', '', '', '', '', ''],
    ['', '', '', '', '', '', '', ''],
    ['', '', '', '', '', '', '', ''],
    ['P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'],
    ['R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'],
    [can_white_castle_queen_side, can_white_castle_king_side, can_black_castle_queen_side, can_black_castle_king_side, en_passant_x]
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
    last_move = req.body.move;
    from_x = Number(last_move[0]);
    from_y = Number(last_move[2]);
    to_x = Number(last_move[4]);
    to_y = Number(last_move[6]);
    board[to_x][to_y] = board[from_x][from_y];
    board[from_x][from_y] = '';
    res.json({ success: true });
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
