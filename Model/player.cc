#include "player.h"
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
ComputerPlayer::ComputerPlayer(Colour c) : Player{c} {}

Move ComputerPlayer::getMove(const Board& b) {
    // AI logic will go here
    return {}; 
}

bool ComputerPlayer::isHuman() const {
    return false; // ComputerPlayer returns false
}
