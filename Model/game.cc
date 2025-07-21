#include "game.h"
#include <iostream>

Game::Game() : 
    board{std::make_unique<Board>()}, whitePlayer{nullptr}, blackPlayer{nullptr}, currentPlayer{nullptr} {};
    


void Game::newGame(const std::string& white, const std::string& black) {
    // Create the white player
    if (white == "human") {
        whitePlayer = std::make_unique<HumanPlayer>(Colour::White);
    } else { // Assume "computer"
        whitePlayer = std::make_unique<ComputerPlayer>(Colour::White);
    }

    // Create the black player
    if (black == "human") {
        blackPlayer = std::make_unique<HumanPlayer>(Colour::Black);
    } else { // Assume "computer"
        blackPlayer = std::make_unique<ComputerPlayer>(Colour::Black);
    }
    
    // White player always starts
    currentPlayer = whitePlayer.get();
    
    // Reset the board to the initial state
    board = std::make_unique<Board>();
}

bool Game::makeMove(const Move& m) {
    if (board->isMoveValid(m)) {
        //Check for promotion
        char promChoice = '\0';
        const Piece* moving = board->getPieceAt(m.from); 
        char ch = moving->getCharRepresentation();
        if ((ch == 'P' && m.to.row == 7) || (ch == 'p' && m.to.row == 0)) {
            if (currentPlayer->isHuman()) {
                while (true) {
                    std::cout << "Promote to (q,r,b,n): ";
                    std::cin >> std::tolower(promChoice);
                    if (promChoice != 'q' && promChoice != 'r' && promChoice != 'b' && promChoice != 'n') continue;
                    else break;
                }
            }
            else promChoice = 'q'; //will need to change for cpu logic
            }
        // If the move is valid, apply it to the board.
        board->applyMove(m, promChoice);
        // Switch the current player to the other player.
        currentPlayer = (currentPlayer == whitePlayer.get()) ? blackPlayer.get() : whitePlayer.get();
        
        return true; // Return true to indicate success
    } else {
        // If the move is invalid, do nothing to the board state.
        return false; // Return false to indicate failure
    }
}

const Board* Game::getBoard() const {
    return board.get();
}

Player* Game::getCurrentPlayer() const {
    return currentPlayer;
}
