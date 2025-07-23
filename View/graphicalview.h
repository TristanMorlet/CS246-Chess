#ifndef GRAPHICALVIEW_H
#define GRAPHICALVIEW_H

#include "../View/observer.h"
#include <memory> // For std::unique_ptr
#include <vector> // Required for the state tracking vector

// Forward declarations
class Board;
class Xwindow;

class GraphicalView : public Observer {
    const Board& board;
    std::unique_ptr<Xwindow> window;
    const int gridSize = 50; // The size of each square in pixels
    // NEW: A 2D vector to store the character representation of the last known board state.
    std::vector<std::vector<char>> previousState;

public:
    GraphicalView(const Board& b);
    ~GraphicalView();

    void notify() override;

private:
    void display(); // Private helper for drawing logic
};

#endif
