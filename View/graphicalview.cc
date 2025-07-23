#include "graphicalview.h"
#include "../Model/board.h"
#include "../Model/piece.h"
#include "window.h" // The X11 helper class
#include <string>

GraphicalView::GraphicalView(const Board& b) : board{b} {
    // The total window size is 8 squares + 1 square for labels
    int windowSize = gridSize * 9;
    window = std::make_unique<Xwindow>(windowSize, windowSize);
}

// The unique_ptr will automatically manage the Xwindow's memory,
// so the destructor can be empty.
GraphicalView::~GraphicalView() {}

void GraphicalView::notify() {
    display();
}

void GraphicalView::display() {
    // Clear the window by drawing a white background
    window->fillRectangle(0, 0, window->getWidth(), window->getHeight(), Xwindow::White);

    // Draw the rank and file labels
    for (int i = 0; i < 8; ++i) {
        // File labels (a-h)
        std::string file(1, 'a' + i);
        window->drawString(gridSize * (i + 1) + gridSize / 2, gridSize / 2, file);
        // Rank labels (1-8)
        std::string rank(1, '8' - i);
        window->drawString(gridSize / 2, gridSize * (i + 1) + gridSize / 2, rank);
    }

    // Draw the board squares and pieces
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            // Calculate pixel coordinates for the top-left corner of the square
            // We add 1 to gridSize to account for the labels
            int x = (c + 1) * gridSize;
            // Y-coordinate is inverted because (0,0) is top-left in graphics
            int y = (7 - r + 1) * gridSize;

            // Determine square color
            if ((r + c) % 2 == 0) {
                window->fillRectangle(x, y, gridSize, gridSize, Xwindow::Green);
            } else {
                window->fillRectangle(x, y, gridSize, gridSize, Xwindow::White);
            }

            // Draw the piece if one exists
            const Piece* p = board.getPieceAt({r, c});
            if (p) {
                std::string piece_str(1, p->getCharRepresentation());
                // Center the piece character in the middle of the square
                window->drawString(x + gridSize / 2, y + gridSize / 2, piece_str);
            }
        }
    }
}
