#include "board.h"

using namespace std;



Board::Board() : whoseTurn{Colour::White} {
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

Board::Board(const Board& other) : whoseTurn{other.whoseTurn} {
    // 1. Initialize the new board to be an 8x8 grid of nullptrs
    theBoard.resize(8);
    for (int i = 0; i < 8; ++i) {
        theBoard[i].resize(8);
    }

    // 2. Loop through the 'other' board and clone each piece
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece* p = other.getPieceAt({r, c});
            if (p) {
                // If a piece exists on the other board, clone it for this board
                theBoard[r][c] = p->clone();
            }
        }
    }
    
    // We intentionally do NOT copy the observers. The temporary board
    // used for move validation does not need to be displayed.
}

void Board::setupPiece(char piece, const Coordinate& coord){
    Colour colour = (piece >= 'a' && piece <= 'z') ? Colour::Black : Colour::White; //determine the color
    char p_lower = tolower(piece);

    if (p_lower == 'p') theBoard[coord.row][coord.col] = std::make_unique<Pawn>(colour, coord.row, coord.col);
    else if (p_lower == 'n') theBoard[coord.row][coord.col] = std::make_unique<Knight>(colour, coord.row, coord.col);
    else if (p_lower == 'b') theBoard[coord.row][coord.col] = std::make_unique<Bishop>(colour, coord.row, coord.col);
    else if (p_lower == 'r') theBoard[coord.row][coord.col] = std::make_unique<Rook>(colour, coord.row, coord.col);
    else if (p_lower == 'q') theBoard[coord.row][coord.col] = std::make_unique<Queen>(colour, coord.row, coord.col);
    else if (p_lower == 'k') theBoard[coord.row][coord.col] = std::make_unique<King>(colour, coord.row, coord.col);

    notifyObservers();
}


void Board::removePiece(const Coordinate& coord){
    if (coord.row >= 0 && coord.row < 8 && coord.col >= 0 && coord.col < 8) {
        theBoard[coord.row][coord.col] = nullptr;
        notifyObservers();
    }
}

void Board::setTurn(Colour colour){
    whoseTurn = colour;
}

bool Board::validateSetup() const {
    int whiteKingCount = 0;
    int blackKingCount = 0;
    Coordinate whiteKingPos = {-1, -1};
    Coordinate blackKingPos = {-1, -1};

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece* p = getPieceAt({r, c});
            if (p) {
                char type = p->getCharRepresentation();
                // Rule 2: Check for pawns on the first or last row
                if (tolower(type) == 'p' && (r == 0 || r == 7)) {
                    return false; // Invalid: Pawn on back rank
                }
                // Rule 1: Count kings and find their positions
                if (type == 'K') {
                    whiteKingCount++;
                    whiteKingPos = {r, c};
                } else if (type == 'k') {
                    blackKingCount++;
                    blackKingPos = {r, c};
                }
            }
        }
    }

    // Rule 1 check
    if (whiteKingCount != 1 || blackKingCount != 1) {
        return false;
    }

    // Rule 3: Check if either king is in check
    if (isDanger(whiteKingPos, Colour::White) || isDanger(blackKingPos, Colour::Black)) {
        return false; 
    }

    return true; // All conditions met, the setup is valid
}

const Piece* Board::getPieceAt(const Coordinate& coord) const{
    return (coord.row < 0 || coord.row >= 8 || coord.col < 0 || coord.col >= 8) ? 
    nullptr : theBoard[coord.row][coord.col].get();
}

Colour Board::getTurn() const {
    return whoseTurn;
}


bool Board::isMoveValid(const Move& move) const {
    const Piece* p = getPieceAt(move.from);

    // 1. Basic checks: piece exists and belongs to the current player
    if (!p || p->getColour() != whoseTurn) {
        return false;
    }

    // 2. Check if the destination is a valid move for that piece type
    std::vector<Move> validMoves = p->getValidMoves(*this);
    bool isPossibleMove = false;
    for (const auto& validMove : validMoves) {
        if (validMove.to.row == move.to.row && validMove.to.col == move.to.col) {
            isPossibleMove = true;
            break;
        }
    }
    if (!isPossibleMove) {
        return false;
    }


    // 3. Check if this move would put your own king in check.
    // To do this, we simulate the move on a temporary copy of the board.
    Board tempBoard = *this; // Requires a copy constructor for Board
    tempBoard.applyMove(move);

    // Find the king of the player who just moved
    Coordinate kingPos = {-1, -1};
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const Piece* tempP = tempBoard.getPieceAt({r, c});
            if (tempP && tempP->getColour() == p->getColour() && (tolower(tempP->getCharRepresentation()) == 'k')) {
                kingPos = {r, c};
                break;
            }
        }
        if (kingPos.row != -1) break;
    }

    // If the king is now in danger on the temporary board, the move is illegal.
    if (tempBoard.isDanger(kingPos, p->getColour())) {
        return false;
    }
    
    return true;
}

void Board::applyMove(const Move& move, char promChoice) { // New parameter for pawn promotion

    Piece* pp = theBoard[move.from.row][move.from.col].get(); // Need this to check en passant validity

    // En Passant Logic must happen before we move
    // We check if a pawn moved diagonally and if en passant square == move square
    if (pp && std::tolower(pp->getCharRepresentation()) == 'p' && std::abs(move.to.col - move.from.col) == 1
    && !getPieceAt(move.to) && enPassantAvailable && move.to.row == enPassantSquare.row && move.to.col == enPassantSquare.col ) { 
        int capRow = (pp->getColour() == Colour::White) ? move.to.row - 1 : move.to.row + 1;
        theBoard[capRow][move.to.col].reset(); // Delete the en passanted pawn
    }

    // Use std::move to transfer ownership of the piece
    theBoard[move.to.row][move.to.col] = std::move(theBoard[move.from.row][move.from.col]);
    
    Piece* p = theBoard[move.to.row][move.to.col].get();
    
    if (p) {
        // Update the piece's internal state
        p->setPosition(move.to.row, move.to.col);
        p->setMoved();
    }

    // --- Other Special Cases ---

    // Pawn Promotion Logic
    // We check if promotion choice is changed (Only happens when a pawn promotion is valid)
    if (promChoice != '\0' && p) {
        Colour c = p->getColour();
        std::unique_ptr<Piece> np;
        switch (promChoice) {      // create new piece for promo
            case 'r': np = std::make_unique<Rook>  (c, move.to.row, move.to.col); break;
            case 'b': np = std::make_unique<Bishop>(c, move.to.row, move.to.col); break;
            case 'n': np = std::make_unique<Knight>(c, move.to.row, move.to.col); break;
            default : np = std::make_unique<Queen> (c, move.to.row, move.to.col); break;
            }
            theBoard[move.to.row][move.to.col] = std::move(np);   // overwrite pawn
        }

    // Castling Logic
    // The King can only move 2 spaces when he castles, so that is what we check for
    if (p && tolower(p->getCharRepresentation()) == 'k' && abs(move.to.col - move.from.col) == 2) {
        //King side castles
        if (move.to.col == 6) {
            // Move rook from h file to f file
            theBoard[move.to.row][5] = std::move(theBoard[move.to.row][7]);
            if (theBoard[move.to.row][5]) {
                theBoard[move.to.row][5]->setPosition(move.to.row, 5);
                theBoard[move.to.row][5]->setMoved();
            }
        }
        // Long castles
        else if (move.to.col == 2) {
            // Move rook from a file to d file
            theBoard[move.to.row][3] = std::move(theBoard[move.to.row][0]);
            if (theBoard[move.to.row][3]) {
                theBoard[move.to.row][3]->setPosition(move.to.row, 3);
                theBoard[move.to.row][3]->setMoved();
            }
        }
    }

    // En Passant Checking (next turn)
    // We check if a pawn moves 2 and set en passant available to true. Does not necessarily mean available move, just that a pawn moved 2
    // We verify all boxes are checked for en passant elsewhere
if (p && tolower(p->getCharRepresentation()) == 'p' && abs(move.to.row - move.from.row) == 2) {
    enPassantAvailable = true;
    enPassantSquare = {((move.to.row + move.from.row)/2), move.to.col};
} else enPassantAvailable = false;

    whoseTurn = (whoseTurn == Colour::White) ? Colour::Black : Colour::White;
    
    notifyObservers();
}

// This version does NOT call getValidMoves, avoiding infinite recursion.
bool Board::isDanger(const Coordinate& sq, Colour bw) const {
    Colour enemyColour = (bw == Colour::White) ? Colour::Black : Colour::White;

    // Check for attacks from sliding pieces (Rook, Bishop, Queen)
    int dirs[8][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1}};
    for (int i = 0; i < 8; ++i) {
        Coordinate current = sq;
        while (true) {
            current.row += dirs[i][0];
            current.col += dirs[i][1];
            if (current.row < 0 || current.row >= 8 || current.col < 0 || current.col >= 8) break;
            
            const Piece* p = getPieceAt(current);
            if (p) {
                if (p->getColour() == enemyColour) {
                    char type = tolower(p->getCharRepresentation());
                    // Diagonal attack (Bishop or Queen)
                    if (i >= 4 && (type == 'b' || type == 'q')) return true;
                    // Straight attack (Rook or Queen)
                    if (i < 4 && (type == 'r' || type == 'q')) return true;
                }
                break; // Path is blocked by a piece
            }
        }
    }

    // Check for attacks from Knights
    int knight_dirs[8][2] = {{-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1}};
    for (int i = 0; i < 8; ++i) {
        Coordinate k_pos = {sq.row + knight_dirs[i][0], sq.col + knight_dirs[i][1]};
        const Piece* p = getPieceAt(k_pos);
        if (p && p->getColour() == enemyColour && tolower(p->getCharRepresentation()) == 'n') {
            return true;
        }
    }

    // Check for attacks from Pawns
    int pawn_dir = (bw == Colour::White) ? -1 : 1;
    Coordinate p_left = {sq.row - pawn_dir, sq.col - 1};
    Coordinate p_right = {sq.row - pawn_dir, sq.col + 1};
    const Piece* p1 = getPieceAt(p_left);
    const Piece* p2 = getPieceAt(p_right);
    if (p1 && p1->getColour() == enemyColour && tolower(p1->getCharRepresentation()) == 'p') return true;
    if (p2 && p2->getColour() == enemyColour && tolower(p2->getCharRepresentation()) == 'p') return true;

    // Check for attacks from the enemy King
    for (int i = 0; i < 8; ++i) {
        Coordinate k_pos = {sq.row + dirs[i][0], sq.col + dirs[i][1]};
        const Piece* p = getPieceAt(k_pos);
        if (p && p->getColour() == enemyColour && tolower(p->getCharRepresentation()) == 'k') {
            return true;
        }
    }

    return false; // The square is safe
}


// Observer pattern methods
void Board::attach(Observer* o){
    observers.emplace_back(o);
}

void Board::detach(Observer* o) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == o) {
            observers.erase(it);
            break;
        }
    }
}

void Board::notifyObservers(){
    for (auto& ob : observers) {
        ob->notify();
    }
}
