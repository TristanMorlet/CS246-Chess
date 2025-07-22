#ifndef PLAYER_H
#define PLAYER_H

#include "../types/types.h"
#include "strategy.h"
#include <memory> // Need for unique_ptr

class Board; // Forward declaration
class Strategy; // New forward declaration

// Abstract base class for all players
class Player {
protected:
    Colour colour;
public:
    Player(Colour c);
    virtual ~Player() = default;
    Colour getColour() const;

    // Pure virtual method for getting the next move
    virtual Move getMove(const Board& b) = 0;

    virtual bool isHuman() const = 0;   //NEW METHOD to check if the current player is humon or ai
};


// Concrete class for human players
class HumanPlayer : public Player {
public:
    HumanPlayer(Colour c);
    Move getMove(const Board& b) override;
    bool isHuman() const override;
};

// Concrete class for computer players
class ComputerPlayer : public Player {
    std::unique_ptr<Strategy> strat;
public:
ComputerPlayer(Colour c, std::unique_ptr<Strategy> s); // NEW PARAMETER (unique_ptr for strat)
    Move getMove(const Board& b) override;
    bool isHuman() const override; // NEW
 };

#endif
