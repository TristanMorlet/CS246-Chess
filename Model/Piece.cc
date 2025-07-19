#include "Piece.h"
#include "Board.h"

std::vector<Move> Pawn::getValidMoves(const Board &b) const {
    return {};
}

std::vector<Move> Knight::getValidMoves(const Board &b) const {
    return {};
}

std::vector<Move> Bishop::getValidMoves(const Board &b) const {
    return {};
}

std::vector<Move> Rook::getValidMoves(const Board &b) const {
    return {};
}

std::vector<Move> Queen::getValidMoves(const Board &b) const {
    return {};
}

std::vector<Move> King::getValidMoves(const Board &b) const {
    std::vector<Piece::Move> mv;
    static const int moveRow[8] = {-1,-1,-1,0,0,1,1,1}; //Possible row/col offsets (moveRow[i] + )
    static const int moveCol[8] = {-1,0,1,-1,1,-1,0,1}; 

}