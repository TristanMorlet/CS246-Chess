#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <memory>
#include "../types/types.h"

class Board;

class Piece { 
protected:
    Piece(Colour colour, int row, int col): colour{colour}, moved{false}, row{row}, col{col} {} //define piece
    Colour colour; //define colour
    bool moved; //For castling and double pawn moves and maybe en passant
    int row;
    int col;

public:

    virtual ~Piece() {} //dtor

    Colour getColour() const { return colour; }
    virtual std::vector<Move> getValidMoves(const Board&) const = 0; //method to find all valid moves for pieces
    virtual char getCharRepresentation() const = 0; //Get char representation for pieces

    //NEW METHODS NEEDED FOR BOARD
    virtual std::unique_ptr<Piece> clone() const = 0;   //For boards copy ctor to work

    void setPosition(int r, int c) {
        row = r;
        col = c;
    }

    void setMoved() {
        moved = false;
    }
};

class Pawn : public Piece {
public:
    Pawn(Colour c, int row, int col): Piece{c,row,col} {}
    std::vector<Move> getValidMoves(const Board &b) const override;
    char getCharRepresentation() const override {
        return colour == Colour::White ? 'P' : 'p'; //For representation, White pieces are capital, black pieces are lowercase
    }
    bool just2Moved = false;
    std::unique_ptr<Piece> clone() const override {
        return std::make_unique<Pawn>(*this); // Creates a copy of the current Pawn
    }
};

class Knight : public Piece {
    public:
        Knight(Colour c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
        char getCharRepresentation() const override {
            return colour == Colour::White ? 'N' : 'n';
        }

        std::unique_ptr<Piece> clone() const override {
            return std::make_unique<Knight>(*this); // Creates a copy of the current Pawn
        }

};


class Bishop : public Piece {
    public:
        Bishop(Colour c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
        char getCharRepresentation() const override {
            return colour == Colour::White ? 'B' : 'b';
        }

        std::unique_ptr<Piece> clone() const override {
            return std::make_unique<Bishop>(*this); // Creates a copy of the current Pawn
        }
        
};


class Rook : public Piece {
    public:
        Rook(Colour c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
        char getCharRepresentation() const override {
            return colour == Colour::White ? 'R' : 'r';
        }

        std::unique_ptr<Piece> clone() const override {
            return std::make_unique<Rook>(*this); // Creates a copy of the current Pawn
        }
};


class Queen : public Piece {
    public:
        Queen(Colour c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
        char getCharRepresentation() const override {
            return colour == Colour::White ? 'Q' : 'q';
        }

        std::unique_ptr<Piece> clone() const override {
            return std::make_unique<Queen>(*this); // Creates a copy of the current Pawn
        }
    };

class King : public Piece {
    public:
        King(Colour c, int row, int col): Piece{c,row,col} {}
        std::vector<Move> getValidMoves(const Board &b) const override;
        char getCharRepresentation() const override {
            return colour == Colour::White ? 'K' : 'k';
        }

        std::unique_ptr<Piece> clone() const override {
            return std::make_unique<King>(*this); // Creates a copy of the current Pawn
        }
};


#endif
