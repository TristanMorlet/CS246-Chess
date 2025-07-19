#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <memory>
#include <string>

#include "../View/observer.h"
#include "Piece.h"


enum class Colour { White, Black, None };

struct Coordinate {
    int row;
    int col;
};

struct Move {
    Coordinate from;
    Coordinate to;
};

class Board {
private:
    std::vector<std::vector<std::unique_ptr<Piece>>> theBoard;
    std::vector<Observer*> observers;

public:
    Board();

    void setupPiece(char piece, const Coordinate& coord);
    void removePiece(const Coordinate& coord);
    const Piece* getPieceAt(const Coordinate& coord) const;

    
    bool isMoveValid(const Move& move) const;
    void applyMove(const Move& move);
    void setTurn(Colour colour);

    // Observer pattern methods
    void attach(Observer* o);
    void detach(Observer* o);
    void notifyObservers();
};

#endif
