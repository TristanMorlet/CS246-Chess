#include "controller.h"
#include "../types/types.h" 
#include <iostream>
#include <string>
#include <sstream>

void Controller::run() {
    std::string line;
    bool gameInProgress = false;

    std::unique_ptr<TextView> tv = nullptr;
    std::unique_ptr<GraphicalView> gv = nullptr;

    std::cout << "Welcome to Chess! Enter 'game human human' to begin." << std::endl;

    while (std::getline(std::cin, line)) {
        std::stringstream ss{line};
        std::string cmd;
        ss >> cmd;

        if (cmd == "game") {
            std::string whitePlayer, blackPlayer;
            ss >> whitePlayer >> blackPlayer;
            game.newGame(whitePlayer, blackPlayer);
            
            tv = std::make_unique<TextView>(*(game.getBoard()));
            gv = std::make_unique<GraphicalView>(*(game.getBoard()));
            
            Board* board = const_cast<Board*>(game.getBoard());
            board->attach(tv.get());
            board->attach(gv.get());

            gameInProgress = true;
            board->notifyObservers();

            // Handle case where AI is the first player
            handleAiTurn(gameInProgress);
        
        } else if (cmd == "move" && gameInProgress) {
            Player* currentPlayer = game.getCurrentPlayer();
            if (currentPlayer->isHuman()) {
                std::string from, to, promotion;
                ss >> from >> to >> promotion; // Read optional promotion
                
                Move m = {parseCoordinate(from), parseCoordinate(to)};

                if (game.makeMove(m)) {
                    // After a successful human move, check if the next player is an AI
                    checkGameState(gameInProgress);
                    handleAiTurn(gameInProgress);
                } else {
                    std::cout << "Invalid move." << std::endl;
                }
            } else {
                 std::cout << "It is the computer's turn. You cannot use the 'move' command." << std::endl;
            }
        } else if (cmd == "resign" && gameInProgress) {
            game.resign();
            gameInProgress = false;
            std::cout << (game.getCurrentPlayer()->getColour() == Colour::White ? "Black" : "White") << " wins!" << std::endl;
        
        } else if (cmd == "setup" && !gameInProgress) {
            enterSetupMode(gameInProgress, tv, gv);
        } else {
            if (!cmd.empty()) {
                 std::cout << "Invalid Command." << std::endl;
            }
        }
    }
    game.printFinalScore();
}

void Controller::checkGameState(bool& gameInProgress) {
    if (!gameInProgress) return;

    GameState currentState = game.getGameState();
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
        case GameState::MoveRule:
            std::cout << "Draw - 50 Move Rule!" << std::endl;
            gameInProgress = false;
            break;
        case GameState::Check:
            std::cout << (game.getCurrentPlayer()->getColour() == Colour::White ? "White" : "Black") << " is in check." << std::endl;
            break;
        case GameState::InProgress:
            break;
    }
}

// Private helper to handle an AI's turn
void Controller::handleAiTurn(bool& gameInProgress) {
    while (gameInProgress && !game.getCurrentPlayer()->isHuman()) {
        Move aiMove = game.getCurrentPlayer()->getMove(*(game.getBoard()));
        
        if (!game.makeMove(aiMove)) {
            // This should ideally not happen if AI logic is perfect
            std::cout << "AI attempted an invalid move." << std::endl;
            break; 
        }

        checkGameState(gameInProgress);
    }
}


void Controller::enterSetupMode(bool& gameInProgress, std::unique_ptr<TextView>& tv, std::unique_ptr<GraphicalView>& gv) {
    std::string line;
    
    game.newGame("human", "human"); 
    tv = std::make_unique<TextView>(*(game.getBoard()));
    gv = std::make_unique<GraphicalView>(*(game.getBoard()));
    Board* board = const_cast<Board*>(game.getBoard());
    board->attach(tv.get());
    board->attach(gv.get());
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
        } else if (setup_cmd == "-") {
            std::string pos;
            setup_ss >> pos;
            board->removePiece(parseCoordinate(pos));
        } else if (setup_cmd == "=") {
            std::string colour_str;
            setup_ss >> colour_str;
            if (colour_str == "white") board->setTurn(Colour::White);
            else if (colour_str == "black") board->setTurn(Colour::Black);
        } else if (setup_cmd == "done") {
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


Coordinate Controller::parseCoordinate(const std::string& s) {
    if (s.length() != 2) {
        return {-1, -1};
    }
    int col = tolower(s[0]) - 'a';
    int row = s[1] - '1';
    return {row, col};
}
