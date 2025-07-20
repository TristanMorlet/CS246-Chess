#include "controller.h"
#include "../View/textview.h"
#include "../types/types.h" // Assuming you have a types.h for Colour, Coordinate etc.
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/*void Controller::run() {
    Board b;
    TextView tv{b};
    b.attach(&tv);
    b.notifyObservers();
}*/

void Controller::run() {
    std::string line;
    bool gameInProgress = false;

    // Create the view and attach it to the board
    std::unique_ptr<TextView> tv = nullptr;

    while (std::getline(std::cin, line)) {
        std::stringstream ss{line};
        std::string cmd;
        ss >> cmd;

        if (cmd == "game") {
            std::string whitePlayer, blackPlayer;
            ss >> whitePlayer >> blackPlayer;
            game.newGame(whitePlayer, blackPlayer);

            // Create the TextView AFTER the new board has been created in newGame.
            tv = std::make_unique<TextView>(*(game.getBoard()));
            
            // Attach the new view to the new board.
            const_cast<Board*>(game.getBoard())->attach(tv.get());

            
            gameInProgress = true;
            const_cast<Board*>(game.getBoard())->notifyObservers(); // Display the new board
        
        } else if (cmd == "move" && gameInProgress) {
            Player* currentPlayer = game.getCurrentPlayer();
            Move m;

            if (currentPlayer->isHuman()) {
                std::string from, to;
                ss >> from >> to;
                m = {parseCoordinate(from), parseCoordinate(to)};
            } else { // It's a computer's turn
                m = currentPlayer->getMove(*(game.getBoard()));
            }

            if (!game.makeMove(m)) {
                std::cout << "Invalid move." << std::endl;
            }

        } else if (cmd == "resign" && gameInProgress) {
            // Resign logic will go here
            gameInProgress = false;
            std::cout << (game.getCurrentPlayer()->getColour() == Colour::White ? "Black" : "White") << " wins!" << std::endl;
        
        } else if (cmd == "setup" && !gameInProgress) {
            // Setup mode logic will go here
        }
    }
}

// Helper to convert "a1", "h8", etc. to {row, col}
Coordinate Controller::parseCoordinate(const std::string& s) {
    int col = s[0] - 'a';
    int row = s[1] - '1';
    return {row, col};
}

