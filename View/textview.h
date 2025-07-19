#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include "observer.h"

class Board; // Forward declaration

class TextView : public Observer {
    const Board& board; // The non-owning reference to the board
public:
    TextView(const Board& b);
    void notify() override; // The update function required by the Observer interface
private:
    void display(); // A private helper to do the actual printing
};

#endif
