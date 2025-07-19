#include "textview.h"
#include "../Model/board.h"
#include <iostream>

using namespace std;

TextView::TextView(const Board& b) : board{b} {}

void TextView::notify() {
    display();
}

void TextView::display() {
    // Loop through rows from 8 down to 1
    for (int row = 7; row >= 0; --row) {
        cout << row + 1 << " "; // Print the rank number
        // Loop through columns from 'a' to 'h'
        for (int col = 0; col < 8; ++col) {
            const Piece* piece = board.getPieceAt(row, col);
            if (piece) {
                cout << piece->getCharRepresentation();
            } 
            else {
                // If the square is empty, print space or underscore
                // A common trick is to check if the sum of coordinates is even or odd
                if ((row + col) % 2 == 0) {
                    cout << ' '; // White square
                } else {
                    cout << '_'; // Dark square
                }
            }
        }
        cout << endl;
    }

    cout << endl << "  abcdefgh" << endl;
}
