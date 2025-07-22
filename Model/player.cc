#include "player.h"
#include "strategy.h" //Need strat def
#include <stdexcept>

//player impl
Player::Player(Colour c) : colour{c} {}

Colour Player::getColour() const {
    return colour;
}

//human player impl

HumanPlayer::HumanPlayer(Colour c) : Player{c} {}

Move HumanPlayer::getMove(const Board& b) {
    throw std::logic_error("getMove() called on HumanPlayer."); //We shouldnt be calling getMove for a human player
}

bool HumanPlayer::isHuman() const {
    return true; // HumanPlayer returns true
}

//computer player impl
ComputerPlayer::ComputerPlayer(Colour c, std::unique_ptr<Strategy> s) : 
    Player{c},  strat{std::move(s)} {} // New parameter: strategy

Move ComputerPlayer::getMove(const Board& b) {
    // AI logic will go here
    // design pattern for levels. for lvl 1/2/3 import random on board and run thru getvalid moves for first owned piece
    // lvl one we hit any piece thats ours call getvalid moves and first valid move we just lowwe it?
    //lvl 2 we just call getvalid moves and first capture move we lowwe it
    // lvl 3 we call getvalidmoves and first safe capture move we lowwe it
    //lvl 4 openai api key yessirr
    return strat->chooseMove(b, colour);
}

bool ComputerPlayer::isHuman() const {
    return false; // ComputerPlayer returns false
}
