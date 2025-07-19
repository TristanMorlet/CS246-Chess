#ifndef TYPES_H
#define TYPES_H

enum class Colour { White, Black, None };

struct Coordinate {
    int row;
    int col;
};

struct Move {
    Coordinate from;
    Coordinate to;
};

#endif
