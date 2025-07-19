#ifndef BOARD_H
#define BOARD_H

#include <algorithm>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>


#include "../View/observer.h"
#include "Piece.h"

using namespace std;

enum class Colour{ // move to .h later if needed
    White,
    Black,
    None
};

struct Move{ // move to .h later if needed
    string to;
    string from;
}

class Board{  
    private:
        vector<vector<unique_ptr<Piece>>> theBoard; // 2d grid of Pieces
        vector<Observer *> observers;

    public:


        // ctor
        Board();



        void setupPiece(char piece, int row, int col);
        bool isMoveValid(Move move);
        void applyMove(Move move);
        void removePiece(int row, int col);
        void setTurn(Colour colour);

        void attach(Observer* o);
        void detach(Observer* o);   
        void notifyObservers();
        
        // getters and setters
        Piece* getPieceAt(int row, int col) const;
        void setPieceAt(int row, int col, string piece_name); // use for pawn promotion

};

#endif
