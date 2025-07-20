#include "piece.h"
#include "board.h"
//TODO: check if a move results in a self check
// This may need to be implemented somewhere else


//Knights have 8 possible moves that are only impossible if out of bounds or if
//destination square is occupied by a friendly
std::vector<Move> Knight::getValidMoves(const Board &b) const { 
    std::vector<Move> move;
    static const int moveRow[8] = {-2, -2, -1, -1, 1, 1, 2, 2}; //knight moves 2 in one direction, 1 in another
    static const int moveCol[8] = {-1, 1, -2, 2, -2, 2, -1, 1};
    for (int i = 0; i < 8; ++i) { //iterate over all possible move combos
        int newRow = row + moveRow[i];
        int newCol = col + moveCol[i];
        if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) continue; //If out of bounds disregard
        Coordinate c{newRow, newCol};
        const Piece *p = b.getPieceAt(c);
        if (!p || p->getColour() != colour) move.push_back({ {row, col}, {newRow, newCol} }); //If square is empty or piece at square is opposite colour,
                                                                                    // add to valid moves
    }
    return move;
}

// King move logic is the same as a knight, however they cannot endanger themselves.
std::vector<Move> King::getValidMoves(const Board &b) const {
    std::vector<Move> move;
    //2 arrays allow for every move combo
    static const int moveRow[8] = {-1,-1,-1,0,0,1,1,1}; //negative means a left move, positive right
    static const int moveCol[8] = {-1,0,1,-1,1,-1,0,1}; //negative means a down move, positive right
    for (int i = 0; i < 8; ++i) { //iterate over all possible move combos
        int newRow = row + moveRow[i];
        int newCol = col + moveCol[i];
        if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) continue; //If out of bounds disregard
        Coordinate c{newRow, newCol};
        const Piece *p = b.getPieceAt(c);
        if (!p || p->getColour() != colour) move.push_back({ {row, col}, {newRow, newCol} }); //If square is empty or piece at square is opposite colour,
                                                                                    // add to valid moves, otherwise disregard
    }
    return move;
}

//The queen moves similar to the king, but can go as many spaces as she pleases
//until she runs into another piece
std::vector<Move> Queen::getValidMoves(const Board &b) const {
    std::vector<Move> move;
    static const int moveRow[8] = {-1,-1,-1,0,0,1,1,1}; //Same logic as above
    static const int moveCol[8] = {-1,0,1,-1,1,-1,0,1}; 
    for (int i = 0; i < 8; ++i) { //iterate over all possible move combos
        int newRow = row + moveRow[i];
        int newCol = col + moveCol[i];
        //to accomodate for the unbounded move in any direction,
        //for each pair in the array, we keep going out until we hit a piece
        while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            Coordinate c{newRow, newCol};
            const Piece *p = b.getPieceAt(c);
            if (!p) { //If space is empty (piece is nullptr), its a valid move
                move.push_back({ {row, col}, {newRow, newCol} });
        } else {//We hit a piece. If the piece is an opposing colour, also a valid move. Otherwise just end.
            if (p->getColour() != colour) move.push_back({ {row, col}, {newRow, newCol} });
            break;
        }
        newRow += moveRow[i];
        newCol += moveCol[i];
        }
    }
    return move;
}

//Bishop has the same logic as a queen, just only on the diagonals
std::vector<Move> Bishop::getValidMoves(const Board &b) const {
        std::vector<Move> move;
        static const int moveRow[4] = {-1,1, 1, -1};
        static const int moveCol[4] = {-1, 1, -1, 1};  
        for (int i = 0; i < 4; ++i) { //iterate over all possible move combos
            int newRow = row + moveRow[i];
            int newCol = col + moveCol[i];
            while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) { //in each direction, go as far out until you hit another piece
                Coordinate c{newRow, newCol};
                const Piece *p = b.getPieceAt(c);
                if (!p) {
                    move.push_back({ {row, col}, {newRow, newCol} });
            } else {
                if (p->getColour() != colour) move.push_back({ {row, col}, {newRow, newCol} });
                break;
            }
            newRow += moveRow[i];
            newCol += moveCol[i];
            }
        }
        return move;
    }

//The rook moves like the queen, just on horizontals
std::vector<Move> Rook::getValidMoves(const Board &b) const {
    std::vector<Move> move;
    static const int moveRow[4] = {-1,1, 0, 0};
    static const int moveCol[4] = {0, 0, -1, 1};  
    for (int i = 0; i < 4; ++i) { //iterate over all possible move combos
        int newRow = row + moveRow[i];
        int newCol = col + moveCol[i];
        while (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) { //in each direction, go as far out until you hit another piece
            Coordinate c{newRow, newCol};
            const Piece *p = b.getPieceAt(c);
            if (!p) {
                move.push_back({ {row, col}, {newRow, newCol} });
        } else {
            if (p->getColour() != colour) move.push_back({ {row, col}, {newRow, newCol} });
            break;
        }
        newRow += moveRow[i];
        newCol += moveCol[i];
        }
    }
    return move;
}

//Pawns behave differently depending on black or white.
//Since the validity of a move differs on every single case, it's easier to hard code all move possibilities
std::vector<Move> Pawn::getValidMoves(const Board &b) const {
    std::vector<Move> move;
    Coordinate c{row, col};
if (colour == Colour::White) { //Check for colour first. This determines which way we should be checking for valid moves
    c.row = row + 1;
    c.col = col;
    //If square in front of the piece is empty, add to valid moves
    if (c.row <= 8 && !b.getPieceAt(c)) {
        move.push_back({ {row, col}, c });
        if (row == 2) { //If pawn is on second rank, check if it can perform the double move (check if rank 4 is empty)
            ++c.row;
            if (!b.getPieceAt(c)) move.push_back({ {row, col}, c });
        }
    }
    Coordinate capR{row + 1, col + 1}; //Right capture validity. If there is a piece there and piece colour is black, valid move
    if (capR.row <= 8 && capR.col <= 8) {
        if (const Piece *pr = b.getPieceAt(capR))
            if (pr->getColour() == Colour::Black) move.push_back({ {row, col}, capR });
    }
    Coordinate capL{row + 1, col - 1}; //Same logic, on the left
    if (capL.row <= 8 && capL.col >= 1) {
        if (const Piece *pl = b.getPieceAt(capL))
            if (pl->getColour() == Colour::Black) move.push_back({ {row, col}, capL });
    }
} else if (colour == Colour::Black) { //Repeat cases, inverting row increments for black.
    c.row = row - 1;
    c.col = col;
    if (c.row >= 1 && !b.getPieceAt(c)) {
        move.push_back({ {row, col}, c });
        if (row == 7) {
            --c.row;
            if (!b.getPieceAt(c)) move.push_back({ {row, col}, c });
        }
    }
    Coordinate capR{row - 1, col + 1};
    if (capR.row >= 1 && capR.col <= 8) {
        if (const Piece *pr = b.getPieceAt(capR))
            if (pr->getColour() == Colour::White) move.push_back({ {row, col}, capR });
    }
    Coordinate capL{row - 1, col - 1};
    if (capL.row >= 1 && capL.col >= 1) {
        if (const Piece *pl = b.getPieceAt(capL))
            if (pl->getColour() == Colour::White) move.push_back({ {row, col}, capL });
    }
}
return move;
}
