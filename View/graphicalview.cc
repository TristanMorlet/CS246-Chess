#include "graphicalview.h"
#include "../Model/board.h"
#include "../Model/piece.h"
#include "window.h" // The X11 helper class
#include <string>

GraphicalView::GraphicalView(const Board& b) : board{b} {
    // The total window size is 8 squares + 1 square for labels
    int windowSize = gridSize * 9;
    window = std::make_unique<Xwindow>(windowSize, windowSize);

    // Initialize the previousState with a dummy value that won't match any piece
    previousState.resize(8, std::vector<char>(8, '\0'));
    
    // Perform an initial full draw of the board and labels
    // Clear the window
    window->fillRectangle(0, 0, window->getWidth(), window->getHeight(), Xwindow::White);
    // Draw labels
    for (int i = 0; i < 8; ++i) {
        std::string file(1, 'a' + i);
        window->drawString(gridSize * (i + 1) + gridSize / 2, gridSize / 2, file);
        std::string rank(1, '8' - i);
        window->drawString(gridSize / 2, gridSize * (i + 1) + gridSize / 2, rank);
    }
    // Initial draw of all squares
    notify();
}

// The unique_ptr will automatically manage the Xwindow's memory,
// so the destructor can be empty.
GraphicalView::~GraphicalView() {}

void GraphicalView::notify() {
    display();
}

void GraphicalView::display() {
    // Loop through the board and compare current state to previous state
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            char currentPieceChar = ' '; // Default to empty
            const Piece* p = board.getPieceAt({r, c});
            if (p) {
                currentPieceChar = p->getCharRepresentation();
            }

            // If the state of this square has changed, redraw it
            if (currentPieceChar != previousState[r][c]) {
                int x = (c + 1) * gridSize;
                int y = (7 - r + 1) * gridSize;

                // Redraw the background square color
                if ((r + c) % 2 == 0) {
                    window->fillRectangle(x, y, gridSize, gridSize, Xwindow::Green);
                } else {
                    window->fillRectangle(x, y, gridSize, gridSize, Xwindow::White);
                }

                // If there's a piece, draw it with its color
                if (p) {
                    std::string piece_str(1, currentPieceChar);
                    window->drawString(x + gridSize / 2, y + gridSize / 2, piece_str);
                }
                
                // Update the previous state for this square
                previousState[r][c] = currentPieceChar;
            }
        }
    }
}
