#include "controller.h"
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
    std::cout << "Welcome to our game of Chess. Please enter on of the following to begin: 1. game <human or computer[1-4]> <human or computer[1-4]> 2. setup" << std::endl;
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

            //const_cast<Board*> is used so we can bypass the constness of .getBoard()
            gameInProgress = true;
            const_cast<Board*>(game.getBoard())->notifyObservers(); // Display the new board



            // TREE CHANGES, AI players should NOT be given move commands, because they make the moves themselves,
            Player* currentPlayer = game.getCurrentPlayer();

            while (!(currentPlayer->isHuman())) {
                if (!gameInProgress) {
                    break;
                }
                // AI player is on white and has first turn, after first board display, display the first move made by the AI player
                Move m;
                m = currentPlayer->getMove(*(game.getBoard()));

                // make this a helper? used twice now.
                if (game.makeMove(m)) {
                    // --- ADD THIS LOGIC ---
                    // After a successful move, check the new game state.
                    GameState currentState = game.getGameState();
                    
                    // Determine which player won, if any
                    std::string winner = (game.getCurrentPlayer()->getColour() == Colour::White) ? "Black" : "White";

                    switch (currentState) {
                        case GameState::Checkmate:
                            std::cout << "Checkmate! " << winner << " wins!" << std::endl;
                            gameInProgress = false;
                            break;
                        case GameState::Stalemate:
                            std::cout << "Stalemate!" << std::endl;
                            gameInProgress = false;
                            break;
                        case GameState::Check:
                            std::cout << (game.getCurrentPlayer()->getColour() == Colour::White ? "White" : "Black") << " is in check." << std::endl;
                            break;
                        case GameState::InProgress:
                            // Do nothing, the game continues.
                            break;
                    }
                    Player* nextPlayer = game.getCurrentPlayer();
                    if (gameInProgress && !nextPlayer->isHuman()) {
                        
                        Move aiMove = nextPlayer->getMove(*(game.getBoard()));
                        if (game.makeMove(aiMove)) {
                            // --- After a successful AI move, check the game state AGAIN ---
                            GameState aiMoveState = game.getGameState();
                            std::string aiWinner = (game.getCurrentPlayer()->getColour() == Colour::White) ? "Black" : "White";

                            switch (aiMoveState) {
                                case GameState::Checkmate:
                                    std::cout << "Checkmate! " << aiWinner << " wins!" << std::endl;
                                    gameInProgress = false;
                                    break;
                                case GameState::Stalemate:
                                    std::cout << "Stalemate!" << std::endl;
                                    gameInProgress = false;
                                    break;
                                case GameState::Check:
                                    std::cout << (game.getCurrentPlayer()->getColour() == Colour::White ? "White" : "Black") << " is in check." << std::endl;
                                    break;
                                case GameState::InProgress:
                                    // Do nothing, the game continues.
                                    continue;
                            }
                        }
                    } else {
                        break;
                    }

                }

            }
        
        } else if (cmd == "move" && gameInProgress) {
            Player* currentPlayer = game.getCurrentPlayer();
            Move m;

            if (currentPlayer->isHuman()) {
                std::string from, to;
                ss >> from >> to;
                m = {parseCoordinate(from), parseCoordinate(to)};
                if (game.makeMove(m)) {
                // --- ADD THIS LOGIC ---
                // After a successful move, check the new game state.
                GameState currentState = game.getGameState();
                
                // Determine which player won, if any
                std::string winner = (game.getCurrentPlayer()->getColour() == Colour::White) ? "Black" : "White";

                switch (currentState) {
                    case GameState::Checkmate:
                        std::cout << "Checkmate! " << winner << " wins!" << std::endl;
                        gameInProgress = false;
                        break;
                    case GameState::Stalemate:
                        std::cout << "Stalemate!" << std::endl;
                        gameInProgress = false;
                        break;
                    case GameState::Check:
                        std::cout << (game.getCurrentPlayer()->getColour() == Colour::White ? "White" : "Black") << " is in check." << std::endl;
                        break;
                    case GameState::InProgress:
                        std::cout << "\n";
                        break;
                        
                }
                // Check if AI is the next player, and play its move automatically after the player makes its move.
                Player* nextPlayer = game.getCurrentPlayer();
                    if (gameInProgress && !nextPlayer->isHuman()) {
                        Move aiMove = nextPlayer->getMove(*(game.getBoard()));
                        if (game.makeMove(aiMove)) {
                            // --- After a successful AI move, check the game state AGAIN ---
                            GameState aiMoveState = game.getGameState();
                            std::string aiWinner = (game.getCurrentPlayer()->getColour() == Colour::White) ? "Black" : "White";

                            switch (aiMoveState) {
                                case GameState::Checkmate:
                                    std::cout << "Checkmate! " << aiWinner << " wins!" << std::endl;
                                    gameInProgress = false;
                                    break;
                                case GameState::Stalemate:
                                    std::cout << "Stalemate!" << std::endl;
                                    gameInProgress = false;
                                    break;
                                case GameState::Check:
                                    std::cout << (game.getCurrentPlayer()->getColour() == Colour::White ? "White" : "Black") << " is in check." << std::endl;
                                    break;
                                case GameState::InProgress:
                                    // Do nothing, the game continues.
                                    break;
                            }
                        }
                    }
            } else {
                std::cout << "Invalid move." << std::endl;
            } 
        }
        } else if (cmd == "resign" && gameInProgress) {
            // Resign logic will go here
            game.resign();
            gameInProgress = false;
            std::cout << (game.getCurrentPlayer()->getColour() == Colour::White ? "Black" : "White") << " wins!" << std::endl;
        
        } else if (cmd == "setup" && !gameInProgress) {
            enterSetupMode(gameInProgress, tv);
        } else {
            if (cmd != "") {
                std::cout << "Invalid Command." << endl;
                std::cout << "Here is a list of Valid Commands: move pos1 pos2, resign, setup, game <human or computer[1-4]> <human or computer[1-4]>" << std::endl;
            }
        }
    }
    game.printFinalScore();
}

void Controller::enterSetupMode(bool& gameInProgress, std::unique_ptr<TextView>& tv) {
    std::string line;
    
    // Create a fresh board and view for setup
    game.newGame("human", "human"); 
    tv = std::make_unique<TextView>(*(game.getBoard()));
    Board* board = const_cast<Board*>(game.getBoard());
    board->attach(tv.get());
    board->notifyObservers();

    std::cout << "Entered setup mode." << std::endl;
    while (std::getline(std::cin, line)) {
        std::stringstream setup_ss{line};
        std::string setup_cmd;
        setup_ss >> setup_cmd;

        if (setup_cmd == "+") {
            char piece;
            std::string pos;
            setup_ss >> piece >> pos;
            board->setupPiece(piece, parseCoordinate(pos));
        } 
        
        else if (setup_cmd == "-") {
            std::string pos;
            setup_ss >> pos;
            board->removePiece(parseCoordinate(pos));
        } 
        
        else if (setup_cmd == "=") {    //issue here for some reason gives u stalemate
            std::string colour_str;
            setup_ss >> colour_str;
            if (colour_str == "white") board->setTurn(Colour::White);
            else if (colour_str == "black") board->setTurn(Colour::Black);
        } 
        
        else if (setup_cmd == "done") {
            if (board->validateSetup()) {
                game.setCurrentPlayer(board->getTurn());
                std::cout << "Setup complete. Starting game." << std::endl;
                gameInProgress = true;
                break; 
            } else {
                std::cout << "Invalid board setup. Please fix before continuing." << std::endl;
            }
        }
    }
}

// Helper to convert "a1", "h8", etc. to {row, col}
Coordinate Controller::parseCoordinate(const std::string& s) {
    int col = s[0] - 'a';
    int row = s[1] - '1';
    return {row, col};
}

