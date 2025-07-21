#ifndef TYPES_H
#define TYPES_H

enum class Colour { White, Black, None };

enum class GameState {
    InProgress,
    Check,
    Checkmate,
    Stalemate
};

struct Coordinate {
    int row;
    int col;
};

struct Move {
    Coordinate from;
    Coordinate to;
};

#endif
