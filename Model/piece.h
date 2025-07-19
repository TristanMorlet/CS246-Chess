#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include "../types/types.h"

class Board;

class Piece { 
protected:
    Piece(Colour colour, int row, int col): colour{colour}, row{row}, col{col} {} //define piece
    Colour colour; //define colour
    int row;
    int col;

public:

    virtual ~Piece() {} //dtor

    Colour getColour() const { return colour; }
    virtual std::vector<Move> getValidMoves(const Board&) const = 0; //method to find all valid moves for pieces
    virtual char getCharRepresentation() const = 0; //Get char representation for pieces
};

class Pawn : public Piece {
public:
    Pawn(Colour c, int row, int col): Piece{c,row,col} {}
    std::vector<Move> getValidMoves(const Board &b) const override;
    char getCharRepresentation() const override {
        return colour == Colour::White ? 'P' : 'p'; //For representation, White pieces are capital, black pieces are lowercase
    }
};

class Knight : public Piece {
    public:
        Knight(Colour c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
        char getCharRepresentation() const override {
            return colour == Colour::White ? 'N' : 'n';
        }

};


class Bishop : public Piece {
    public:
        Bishop(Colour c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
        char getCharRepresentation() const override {
            return colour == Colour::White ? 'B' : 'b';
        }
};


class Rook : public Piece {
    public:
        Rook(Colour c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
        char getCharRepresentation() const override {
            return colour == Colour::White ? 'R' : 'r';
        }
};


class Queen : public Piece {
    public:
        Queen(Colour c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
        char getCharRepresentation() const override {
            return colour == Colour::White ? 'Q' : 'q';
        }
    };

class King : public Piece {
    public:
        King(Colour c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
        char getCharRepresentation() const override {
            return colour == Colour::White ? 'K' : 'k';
        }
};


#endif
