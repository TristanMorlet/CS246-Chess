#ifndef BOARD_H
#define BOARD_H

#include <algorithm>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>


#include "../View/observer.h"
#include "Piece.h"

using namespace std;

enum class Colour{
    White,
    Black,
    None
};

struct Move{
    string to;
    string from;
}

class Board{  
    vector<Observer *> observers;

    public:



    void setupPiece(char piece, string coord);
    bool isMoveValid(Move move);
    void applyMove(Move move);
    void removePiece(string coord);
    void setTurn(Colour colour);

    void attach(Observer* o);
    void detach(Observer* o);   
    void notifyObservers();
    
    // getters and setters
    Piece* getPieceAt(string coordinate) const {
        if (Board[coordinate]) {}
    }
};

#endif
