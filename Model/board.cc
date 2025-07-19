#include "board.h"

using namespace std;



Board::Board() {
// 1. Initialize an 8x8 board with nullptrs
    /*
    theBoard: [ [], [], [], [], [], [], [], [] ] *before the for loop*
    theBoard: [ [nullptr,...,nullptr], ... , [nullptr, ..., nullptr]] *after the for loop*
    */

    theBoard.resize(8); //create 8 empty rows
    for (int row = 0; row < 8; ++row) {
        theBoard[row].resize(8); //create 8 columns for each row
    }

// 2. Place the Black pieces (top of the board)
    // Back rank (row 7)
    theBoard[7][0] = make_unique<Rook>(Colour::Black, 7, 0);
    theBoard[7][1] = make_unique<Knight>(Colour::Black, 7, 1);
    theBoard[7][2] = make_unique<Bishop>(Colour::Black, 7, 2);
    theBoard[7][3] = make_unique<Queen>(Colour::Black, 7, 3);
    theBoard[7][4] = make_unique<King>(Colour::Black, 7, 4);
    theBoard[7][5] = make_unique<Bishop>(Colour::Black, 7, 5);
    theBoard[7][6] = make_unique<Knight>(Colour::Black, 7, 6);
    theBoard[7][7] = make_unique<Rook>(Colour::Black, 7, 7);
    // Pawn rank (row 6)
    for (int col = 0; col < 8; ++col) {
        theBoard[6][col] = make_unique<Pawn>(Colour::Black, 6, col);
    }

    // 3. Place the White pieces (bottom of the board)
    // Pawn rank (row 1)
    for (int col = 0; col < 8; ++col) {
        theBoard[1][col] = make_unique<Pawn>(Colour::White, 1, col);
    }
    // Back rank (row 0)
    theBoard[0][0] = make_unique<Rook>(Colour::White, 0, 0);
    theBoard[0][1] = make_unique<Knight>(Colour::White, 0, 1);
    theBoard[0][2] = make_unique<Bishop>(Colour::White, 0, 2);
    theBoard[0][3] = make_unique<Queen>(Colour::White, 0, 3);
    theBoard[0][4] = make_unique<King>(Colour::White, 0, 4);
    theBoard[0][5] = make_unique<Bishop>(Colour::White, 0, 5);
    theBoard[0][6] = make_unique<Knight>(Colour::White, 0, 6);
    theBoard[0][7] = make_unique<Rook>(Colour::White, 0, 7);
}


void Board::setupPiece(char piece, const Coordinate& coord){
    
}


void Board::removePiece(const Coordinate& coord){

}
const Piece* Board::getPieceAt(const Coordinate& coord) const{
    return theBoard[coord.row][coord.col].get();
}


bool Board::isMoveValid(const Move& move) const{
    return true;

}
void Board::applyMove(const Move& move){

}
void Board::setTurn(Colour colour){

}

// Observer pattern methods
void Board::attach(Observer* o){
    observers.emplace_back(o);
}
void Board::detach(Observer* o){

}
void Board::notifyObservers(){
    for (auto& ob : observers) {
        ob->notify();
    }
}