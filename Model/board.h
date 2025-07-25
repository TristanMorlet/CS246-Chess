#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#include "../View/observer.h"
#include "../types/types.h"

#include <vector>
#include <memory>
#include <string>

class Board {
private:
    std::vector<std::vector<std::unique_ptr<Piece>>> theBoard;
    std::vector<Observer*> observers;
    Colour whoseTurn; // Tracks the colour of the current player
    bool enPassantAvailable = false; // Tracks whether en passant can be played
    Coordinate enPassantSquare{}; // Where the capture will take place
    int moveRule = 0; // 50 move rule
    bool isCap = false; // If move is capture or not

public:
    Board();    // Sets up the 8x8 grid and places the 32 pieces.
    Board(const Board& other);

    //  Setup Methods 
    void setupPiece(char piece, const Coordinate& coord); // Places a piece on a specified square for setup.
    void removePiece(const Coordinate& coord);            // Removes any piece from a specified square for setup.
    void setTurn(Colour colour);                          // Sets which colour's turn it is (for setup mode).
    bool validateSetup() const;                           // Ensures that the setup made by the user is valid

    
    //  Game Logic Methods 
    bool isMoveValid(const Move& move) const;             // Checks if a proposed move is legal.
    void applyMove(const Move& move, char promChoice = '\0');                     // Executes a move, updating the board state. NEW PARAMETER(promChoice)
    bool isDanger(const Coordinate& sq, Colour bw) const;       // Checks if a piece can be captured by other pieces.(NEW NOT IN UML)
    // Below all new. The use should be obvious.
    bool isEnPassantAvailable() const { return enPassantAvailable; } 
    Coordinate getEnPassantSquare() const { return enPassantSquare; }
    bool isMoveRuleVoilated() const;


    //  Accessors (Getters) 
    const Piece* getPieceAt(const Coordinate& coord) const; // Returns a const pointer to the piece at a coordinate.
    Colour getTurn() const;                                 // Returns the colour of the player whose turn it is.(NEW NOT IN UML)

    // Observer pattern methods
    void attach(Observer* o);
    void detach(Observer* o);
    void notifyObservers();
};

#endif
