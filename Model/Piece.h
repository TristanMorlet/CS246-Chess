#ifndef PIECE_H
#define PIECE_H

#include <vector>

class Board;

class Piece { 
protected:
    Piece(Colour colour, int row, int col): colour{colour}, row{row}, col{col} {}
    Colour colour;
    int row;
    int col;

public:
    enum class Colour { White, Black };
    struct Move { int fromRow, fromCol, toRow, toCol; };

    virtual ~Piece() {}

    Colour getColour() const { return colour; }
    virtual std::vector<Move> getValidMoves(const Board&) const = 0;
};

class Pawn : public Piece {
public:
    Pawn(Piece::Colour; c, int row, int col): Piece{c,row,col} {}
    std::vector<Move> getValidMoves(const Board &b) const override;
};

class Knight : public Piece {
    public:
        Knight(Piece::Colour; c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
    };


class Bishop : public Piece {
    public:
        Bishop(Piece::Colour; c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
    };


class Rook : public Piece {
    public:
        Rook(Piece::Colour; c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
    };


class Queen : public Piece {
    public:
        Queen(Piece::Colour; c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
    };

class King : public Piece {
    public:
        King(Piece::Colour; c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
    };


#endif
