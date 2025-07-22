#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>
#include "board.h"
#include "player.h"
#include "../types/types.h"

class Game {
private:
    std::unique_ptr<Board> board;
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;
    Player* currentPlayer; // A raw pointer to track the current player

    GameState currentState;
    void updateGameState();

public:
    Game();

    // Sets up a new game with the specified player types ("human" or "computer"[1-4])
    void newGame(const std::string& white, const std::string& black);

    // Executes a move
    bool makeMove(const Move& m);

    // Accessors for the Controller and View
    const Board* getBoard() const;
    Player* getCurrentPlayer() const;
    void setCurrentPlayer(Colour colour);

    GameState getGameState() const;

};

#endif
