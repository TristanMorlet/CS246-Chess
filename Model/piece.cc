#include "piece.h"
#include "board.h"


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
        if (!p || p->getColour() != colour) move.push_back({ {row, col}, c }); //If square is empty or piece at square is opposite colour,
                                                                                    // add to valid moves
    }
    return move;
}

// King move logic is the same as a knight, however they cannot endanger themselves.
std::vector<Move> King::getValidMoves(const Board &b) const {
    std::vector<Move> move;
    //2 arrays allow for every move combo
    static const int moveRow[8] = {-1,-1,-1,0,0,1,1,1}; //negative means a down move, positive up
    static const int moveCol[8] = {-1,0,1,-1,1,-1,0,1}; //negative means a down move, positive right
    for (int i = 0; i < 8; ++i) { //iterate over all possible move combos
        int newRow = row + moveRow[i];
        int newCol = col + moveCol[i];
        if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) continue; //If out of bounds disregard
        Coordinate c{newRow, newCol};
        const Piece *p = b.getPieceAt(c);
        if (b.isDanger(c, colour)) continue; //If move puts king in check, invalid move
        if (!p || p->getColour() != colour) move.push_back({ {row, col}, c});
    }
        //Special case: Castling. Requires king not in check, no pieces between rook and king, rook and king not moved
        // and obviously king cannot put himself in danger
        if (!hasMoved() && !b.isDanger({row, col}, colour)) {

            // Normal castles
            const Piece* rook = b.getPieceAt({row, 7});
            if (rook && !rook->hasMoved() && rook->getColour() == colour) {
                Coordinate f{row, 5}, g{row, 6};
                if (!b.getPieceAt(f) && !b.getPieceAt(g) && !b.isDanger(f, colour) && !b.isDanger(g, colour)) {
                    move.push_back({ {row, col}, g });
                }
            }
            // Long castles
            const Piece* rookL = b.getPieceAt({row, 0});
            if (rookL && !rookL->hasMoved() && rookL->getColour() == colour) {
                Coordinate bf{row, 1}, cf{row, 2}, df{row, 3};
                if (!b.getPieceAt(bf) && !b.getPieceAt(cf) && !b.getPieceAt(df) && !b.isDanger(cf, colour) && !b.isDanger(df, colour)) {
                    move.push_back({ {row, col}, cf });
                }
            }
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
                move.push_back({ {row, col}, c });
        } else {//We hit a piece. If the piece is an opposing colour, also a valid move. Otherwise just end.
            if (p->getColour() != colour) move.push_back({ {row, col}, c });
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
                    move.push_back({ {row, col}, c });
            } else {
                if (p->getColour() != colour) move.push_back({ {row, col}, c});
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
                move.push_back({ {row, col}, c });
        } else {
            if (p->getColour() != colour) move.push_back({ {row, col}, c });
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
    std::vector<Move> moves;

    if (colour == Colour::White) {
        // --- 1. Standard one-square move ---
        Coordinate one_fwd{row + 1, col};
        if (one_fwd.row < 8 && !b.getPieceAt(one_fwd)) {
            moves.push_back({ {row, col}, one_fwd });

            // --- 2. Two-square initial move ---
            // This can only happen if the first square is also empty.
            if (row == 1 && !hasMoved()) {
                Coordinate two_fwd{row + 2, col};
                if (!b.getPieceAt(two_fwd)) {
                    moves.push_back({ {row, col}, two_fwd });
                }
            }
        }

        // --- 3. Captures ---
         Coordinate cap_right{row + 1, col + 1};
        if (cap_right.row < 8 && cap_right.col < 8) {
            const Piece *p = b.getPieceAt(cap_right);
            if (p && p->getColour() == Colour::Black) moves.push_back({ {row, col}, cap_right });
            // FIX: en passant compares cap_right with EP square correctly
            else if (!p && b.isEnPassantAvailable() &&
                     cap_right.row == b.getEnPassantSquare().row &&
                     cap_right.col == b.getEnPassantSquare().col)
                moves.push_back({ {row, col}, cap_right });
        }

        Coordinate cap_left{row + 1, col - 1};
        if (cap_left.row < 8 && cap_left.col >= 0) {
            const Piece *p = b.getPieceAt(cap_left);
            if (p && p->getColour() == Colour::Black) moves.push_back({ {row, col}, cap_left });
            // FIX: used cap_left, not cap_right
            else if (!p && b.isEnPassantAvailable() &&
                     cap_left.row == b.getEnPassantSquare().row &&
                     cap_left.col == b.getEnPassantSquare().col)
                moves.push_back({ {row, col}, cap_left });
        }
    } else { // Black Pawn
        // --- 1. Standard one-square move ---
        Coordinate one_fwd{row - 1, col};
        if (one_fwd.row >= 0 && !b.getPieceAt(one_fwd)) { // Fixed boundary
            moves.push_back({ {row, col}, one_fwd });

            // --- 2. Two-square initial move ---
            if (row == 6 && !hasMoved()) {
                Coordinate two_fwd{row - 2, col};
                if (!b.getPieceAt(two_fwd)) {
                    moves.push_back({ {row, col}, two_fwd });
                }
            }
        }

        // --- 3. Captures ---
Coordinate cap_right{row - 1, col + 1};
        if (cap_right.row >= 0 && cap_right.col < 8) {
            const Piece *p = b.getPieceAt(cap_right);
            if (p && p->getColour() == Colour::White) moves.push_back({ {row, col}, cap_right });
            // FIX: en passant for black right
            else if (!p && b.isEnPassantAvailable() &&
                     cap_right.row == b.getEnPassantSquare().row &&
                     cap_right.col == b.getEnPassantSquare().col)
                moves.push_back({ {row, col}, cap_right });
        }

        Coordinate cap_left{row - 1, col - 1};
        if (cap_left.row >= 0 && cap_left.col >= 0) {
            const Piece *p = b.getPieceAt(cap_left);
            if (p && p->getColour() == Colour::White) moves.push_back({ {row, col}, cap_left });
            // FIX: en passant for black left
            else if (!p && b.isEnPassantAvailable() &&
                     cap_left.row == b.getEnPassantSquare().row &&
                     cap_left.col == b.getEnPassantSquare().col)
                moves.push_back({ {row, col}, cap_left });
        }
    }
    return moves;
}
