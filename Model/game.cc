#include "game.h"
#include "strategy.h"
#include <memory>         // unique_ptr
#include <string>         // std::string
#include <cctype>         // std::tolower
#include <iostream>

Game::Game() : 
    board{std::make_unique<Board>()}, 
    whitePlayer{nullptr}, 
    blackPlayer{nullptr}, 
    currentPlayer{nullptr}, 
    whiteScore{0.0},
    blackScore{0.0},
    currentState{GameState::InProgress}{};
    


void Game::newGame(const std::string& white, const std::string& black) {
    // Create the white player
    if (white == "human") {
        whitePlayer = std::make_unique<HumanPlayer>(Colour::White);
    } else { // Assume "computer"
        if (white == "computer1") { 
            whitePlayer = std::make_unique<ComputerPlayer>(Colour::White, makeStrategy(Level::L1)); 
        }
        else if (white == "computer2") {
            whitePlayer = std::make_unique<ComputerPlayer>(Colour::White, makeStrategy(Level::L2));
        }
        else if (white == "computer3") {
            whitePlayer = std::make_unique<ComputerPlayer>(Colour::White, makeStrategy(Level::L3));
        }
        else {
            std::cout << "Since you didnt enter a valid player name, you will be facing the hardest ai..." << std::endl; // Lol I rock with this
            whitePlayer = std::make_unique<ComputerPlayer>(Colour::White, makeStrategy(Level::L3));
        }
    }

    // Create the black player
    if (black == "human") {
        blackPlayer = std::make_unique<HumanPlayer>(Colour::Black);
    } else { // Assume "computer"
        if (black == "computer1") { 
            blackPlayer = std::make_unique<ComputerPlayer>(Colour::Black, makeStrategy(Level::L1)); 
        }
        else if (black == "computer2") {
            blackPlayer = std::make_unique<ComputerPlayer>(Colour::Black, makeStrategy(Level::L2));
        }
        else if (black == "computer3") {
            blackPlayer = std::make_unique<ComputerPlayer>(Colour::Black, makeStrategy(Level::L3));
        }
        else {
            std::cout << "Since you didnt enter a valid player name, you will be facing the hardest ai..." << std::endl;
            blackPlayer = std::make_unique<ComputerPlayer>(Colour::Black, makeStrategy(Level::L3));
        }
    }
    
    // White player always starts
    currentPlayer = whitePlayer.get();
    
    // Reset the board to the initial state
    board = std::make_unique<Board>();
    currentState = GameState::InProgress;
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
                    std::cin >> promChoice;
                    promChoice = std::tolower(promChoice);
                    if (promChoice != 'q' && promChoice != 'r' && promChoice != 'b' && promChoice != 'n') continue;
                    else break;
                }
            }
            else promChoice = 'q'; //will need to change for cpu logic 
            // ^ Honestly I really dont feel like bothering. If we did the only scenario I could think of underpromoting
            // is to avoid stalemate or some knight promotion chess puzzle bs
            }
        // If the move is valid, apply it to the board.
        board->applyMove(m, promChoice);

        // Display the move made by the AI since now made automatically.
        if (!(currentPlayer->isHuman())){


            char colLetters[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

            std::string move_str = "";
            move_str += colLetters[m.from.col];
            move_str += std::to_string(m.from.row + 1);
            move_str += " ";
            move_str += colLetters[m.to.col];
            move_str += std::to_string(m.to.row + 1);
            std::cout << '\n' << "AI plays move " << move_str << std::endl;

            

        }
        // Switch the current player to the other player.
        currentPlayer = (currentPlayer == whitePlayer.get()) ? blackPlayer.get() : whitePlayer.get();

        updateGameState();
        
        return true; // Return true to indicate success
    } else {
        // If the move is invalid, do nothing to the board state.
        return false; // Return false to indicate failure
    }
}

void Game::updateGameState() {
    // 1. Find the current player's king
    Coordinate kingPos = {-1, -1};
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece* p = board->getPieceAt({r, c});
            if (p && p->getColour() == currentPlayer->getColour() && tolower(p->getCharRepresentation()) == 'k') {
                kingPos = {r, c};
                break;
            }
        }
        if (kingPos.row != -1) break;
    }

    // 2. Check if the current player has ANY legal moves
    bool hasLegalMove = false;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece* p = board->getPieceAt({r, c});
            if (p && p->getColour() == currentPlayer->getColour()) {
                std::vector<Move> moves = p->getValidMoves(*board);
                for (const auto& move : moves) {
                    if (board->isMoveValid(move)) {
                        hasLegalMove = true;
                        break;
                    }
                }
            }
            if (hasLegalMove) break;
        }
        if (hasLegalMove) break;
    }

    // 3. Determine the final game state
    bool kingInDanger = board->isDanger(kingPos, currentPlayer->getColour());

    if (!hasLegalMove) {
        if (kingInDanger) {
            currentState = GameState::Checkmate;
            if (currentPlayer->getColour() == Colour::White) blackScore += 1.0;
            else whiteScore += 1.0;
        } else {
            currentState = GameState::Stalemate;
            whiteScore += 0.5;
            blackScore += 0.5;
        }
    } else {
        if (kingInDanger) {
            currentState = GameState::Check;
        } else {
            currentState = GameState::InProgress;
        }
    }
}

const Board* Game::getBoard() const {
    return board.get();
}

Player* Game::getCurrentPlayer() const {
    return currentPlayer;
}

void Game::setCurrentPlayer(Colour colour) {
    if (colour == Colour::White) {
        currentPlayer = whitePlayer.get();
    }
    else {
        currentPlayer = blackPlayer.get();
    }
}

GameState Game::getGameState() const {
    return currentState;
}

void Game::resign() {
    if (currentPlayer->getColour() == Colour::White) {
        blackScore += 1.0;
    } else {
        whiteScore += 1.0;
    }
}

void Game::printFinalScore() const {
    std::cout << "Final Score:" << std::endl;
    std::cout << "White: " << whiteScore << std::endl;
    std::cout << "Black: " << blackScore << std::endl;
}
