const html_board = document.getElementById('board');
let moves = [];

const pieceImages = {
    'r': 'public/images/black_rook.png',
    'n': 'public/images/black_knight.png',
    'b': 'public/images/black_bishop.png',
    'q': 'public/images/black_queen.png',
    'k': 'public/images/black_king.png',
    'p': 'public/images/black_pawn.png',
    'R': 'public/images/white_rook.png',
    'N': 'public/images/white_knight.png',
    'B': 'public/images/white_bishop.png',
    'Q': 'public/images/white_queen.png',
    'K': 'public/images/white_king.png',
    'P': 'public/images/white_pawn.png'
};

function loadBoard() {
    fetch("/board").then(function(response) {
        return response.json();
    }).then(function(data) {
        board = data["data"];
        showBoard(board);
    }).catch(function(err) {
        console.log('Fetch Error :-S', err);
    });
}

function showBoard(board) {
    html_board.innerHTML = "";
    for (let row = 0; row < 8; row++) {
        for (let col = 0; col < 8; col++) {
            const square = document.createElement('div');
            square.classList.add('square');
            square.classList.add((row + col) % 2 === 0 ? 'white' : 'black');
            square.dataset.row = row;
            square.dataset.col = col;
            square.addEventListener('click', () => onSquareClick(row, col));

            const piece = board[row][col];
            if (piece) {
                const pieceElement = document.createElement('img');
                pieceElement.src = pieceImages[piece];
                pieceElement.alt = piece;
                pieceElement.classList.add('piece');
                pieceElement.dataset.row = row;
                pieceElement.dataset.col = col;
                square.appendChild(pieceElement);
            }
            html_board.appendChild(square);
        }
    }
}

let selectedPiece = null;
let selectedSquare = null;
let last_move = null;

function onSquareClick(row, col) {
    const targetSquare = html_board.children[row * 8 + col];
    if (selectedPiece) {
        if (selectedPiece.dataset.row === row.toString()
            && selectedPiece.dataset.col === col.toString()) {
            selectedPiece = null;
            selectedSquare = null;
            return;
        }
        from_row = selectedSquare.dataset.row;
        from_col = selectedSquare.dataset.col;
        const move = `${from_row},${from_col},${row},${col}, `;
        makeMove(move);
        loadBoard();
        selectedPiece = null;
        selectedSquare = null;
        last_move = move;
    } else {
        const piece = targetSquare.querySelector('.piece');
        if (piece) {
            selectedPiece = piece;
            selectedSquare = targetSquare;
        }
    }
}

function makeMove(move) {
    fetch('http://localhost:3000/make-move', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({move})
    })
    .then(response => response.json())
    .then(data => {
        if (!data.success) {
            alert('Error making move');
        }
    });
}

function getLastMove() {
    fetch("/last-move").then(function(response) {
        return response.json();
    }).then(function(data) {
        let move = data["data"];
        if (move !== last_move) {
            console.log(last_move);
            last_move = move;
            loadBoard();
        }
    }).catch(function(err) {
        console.log('Fetch Error :-S', err);
    });
}

loadBoard();
setInterval(getLastMove, 500);