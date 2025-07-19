#include "Piece.h"
#include "Board.h"

std::vector<Move> Pawn::getValidMoves(const Board &b) const { //do later
/*
    static const int moveRow[4] = {-1, 0, 1, 0};
    static const int moveCol[4] = {1, 1, 1, 2};
    if (newRow < 0 || newCol >= 8 || newRow < 0 || newCol >= 8) continue; 
    Coordinate c{newRow = row + moveRow[0], newCol = row + moveCol[0]};
    Piece *p = b.getPieceAt(c);
    if (p->getColour() != colour) move.push_back({row,col,newRow,newCol});
*/
}

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
        if (!p || p->getColour() != colour) move.push_back({row,col,newRow,newCol}); //If square is empty or piece at square is opposite colour,
                                                                                    // add to valid moves
    }
    return move;
}


std::vector<Move> Bishop::getValidMoves(const Board &b) const {
        std::vector<Move> move;
        static const int moveRow[4] = {-1,1, 1, -1}; //Possible row/col offsets (left==neg)
        static const int moveCol[4] = {-1, 1, -1, 1};  
        for (int i = 0; i < 4; ++i) { //iterate over all possible move combos
            int newRow = row + moveRow[i];
            int newCol = col + moveCol[i];
            while (newRow > 0 && newRow =< 8 && newCol > 0 && newCol =< 8) { //in each direction, go as far out until you hit another piece
                Coordinate c{newRow, newCol};
                const Piece *p = b.getPieceAt(c);
                if (!p) {
                    move.push_back({row,col,newRow,newCol});
            } else {
                if (p->getColour() != colour) move.push_back({row, col, newRow, newCol});
                break;
            }
            newRow += moveRow[i];
            newCol += moveCol[i];
            }
        }
        return move;
    }

std::vector<Move> Rook::getValidMoves(const Board &b) const {
    std::vector<Move> move;
    static const int moveRow[4] = {-1,1, 0, 0}; //Possible row/col offsets (left==neg)
    static const int moveCol[4] = {0, 0, -1, 1};  
    for (int i = 0; i < 4; ++i) { //iterate over all possible move combos
        int newRow = row + moveRow[i];
        int newCol = col + moveCol[i];
        while (newRow > 0 && newRow =< 8 && newCol > 0 && newCol =< 8) { //in each direction, go as far out until you hit another piece
            Coordinate c{newRow, newCol};
            const Piece *p = b.getPieceAt(c);
            if (!p) {
                move.push_back({row,col,newRow,newCol});
        } else {
            if (p->getColour() != colour) move.push_back({row, col, newRow, newCol});
            break;
        }
        newRow += moveRow[i];
        newCol += moveCol[i];
        }
    }
    return move;
}

std::vector<Move> Queen::getValidMoves(const Board &b) const {
    std::vector<Move> move;
    static const int moveRow[8] = {-1,-1,-1,0,0,1,1,1}; //Possible row/col offsets (left==neg)
    static const int moveCol[8] = {-1,0,1,-1,1,-1,0,1}; 
    for (int i = 0; i < 8; ++i) { //iterate over all possible move combos
        int newRow = row + moveRow[i];
        int newCol = col + moveCol[i];
        while (newRow > 0 && newRow =< 8 && newCol > 0 && newCol =< 8) { //in each direction, go as far out until you hit another piece
            Coordinate c{newRow, newCol};
            const Piece *p = b.getPieceAt(c);
            if (!p) {
                move.push_back({row,col,newRow,newCol});
        } else {
            if (p->getColour() != colour) move.push_back({row, col, newRow, newCol});
            break;
        }
        newRow += moveRow[i];
        newCol += moveCol[i];
        }
    }
    return move;
}

std::vector<Move> King::getValidMoves(const Board &b) const {
    std::vector<Move> move;
    static const int moveRow[8] = {-1,-1,-1,0,0,1,1,1}; //Possible row/col offsets (left==neg)
    static const int moveCol[8] = {-1,0,1,-1,1,-1,0,1}; //(down==neg)
    for (int i = 0; i < 8; ++i) { //iterate over all possible move combos
        int newRow = row + moveRow[i];
        int newCol = col + moveCol[i];
        if (newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) continue; //If out of bounds disregard
        Coordinate c{newRow, newCol};
        const Piece *p = b.getPieceAt(c);
        if (!p || p->getColour() != colour) move.push_back({row,col,newRow,newCol}); //If square is empty or piece at square is opposite colour,
                                                                                    // add to valid moves
    }
    return move;
}
