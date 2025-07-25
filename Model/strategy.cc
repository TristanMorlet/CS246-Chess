#include "strategy.h"
#include "../Model/board.h"
#include "../Model/piece.h"
#include <random>
#include <cctype>
#include <cmath>
#include "../types/types.h"
#include <algorithm>

//Mersenne Random Number Generator
static std::mt19937 rng(std::random_device{}());


// --- Helper Functions ---

// Get the colour of the opposition
static Colour opp(Colour c) {
    return c == Colour::White ? Colour::Black : Colour::White;
}

// Get all moves
static std::vector<Move> allMoves(const Board& b, Colour side) {
    std::vector<Move> v;
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            const Piece* p = b.getPieceAt({r,c});
            if (p && p->getColour() == side) {
                auto potential_moves = p->getValidMoves(b);
                for (const auto& move : potential_moves) {
                    if (b.isMoveValid(move)) {
                        v.push_back(move);
                    }
                }
            }
        }
    return v;
}

// Find the king
static Coordinate findKing(const Board& b, Colour who){
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col){
            const Piece* p = b.getPieceAt({row, col});
            if (p && p->getColour() == who && std::tolower(p->getCharRepresentation()) == 'k') return {row, col};
        }
    }
    return {-1, -1};
}

// Check if a move gives check
static bool givesCheck(const Board& b, const Move& m, Colour me){
    Board tmp = b;
    tmp.applyMove(m);
    Colour other = opp(me);
    Coordinate k = findKing(tmp, other);
    return tmp.isDanger(k, other);
}

// Check if a moves is safe after it is played
static bool safeAfter(const Board& b, const Move& m, Colour side){
    Board tmp = b;
    tmp.applyMove(m);
    return !tmp.isDanger(m.to, side);
}


// --- LEVEL 1 ---
// Fully Random Moves
Move Level1::chooseMove(const Board& b, Colour side) {
    auto v = allMoves(b, side);
    if (v.empty()) return {};
    std::uniform_int_distribution<size_t> d(0, v.size()-1);
    return v[d(rng)];
}

// --- LEVEL 2 ---
// Prioritize checks and captures
Move Level2::chooseMove(const Board& b, Colour side) {
    auto v = allMoves(b, side);
    if (v.empty()) return {};
    std::vector<Move> satisfied;
    satisfied.reserve(v.size());
    for (const auto& m : v) {
        const Piece* target = b.getPieceAt(m.to);
        if ((target && target->getColour() != side)
        || (target && std::tolower(target->getCharRepresentation()) == 'k' && target->getColour() != side)) {
            satisfied.push_back(m); }
    }
    if (satisfied.empty()) satisfied = v;
    std::uniform_int_distribution<size_t> d(0, satisfied.size()-1);
    return satisfied[d(rng)];
}

// --- LEVEL 3 ---
// Prioritize safe moves or checks and captures
Move Level3::chooseMove(const Board& b, Colour side) {
    auto v = allMoves(b, side);
    if (v.empty()) return {};
    std::vector<Move> capture;
    std::vector<Move> nocap;
    capture.reserve(v.size());
    nocap.reserve(v.size());
    for (const auto& m : v) {
        const Piece* target = b.getPieceAt(m.to);
        if ((target && target->getColour() != side && safeAfter(b, m, side)) || givesCheck(b, m, side)) {
            capture.push_back(m);
            }
        if (!target && safeAfter(b, m, side)) nocap.push_back(m);
    }
    if (capture.empty()) {
        if (nocap.empty()) capture = v;
        else capture = nocap;
    }
    std::uniform_int_distribution<size_t> d(0, capture.size()-1);
    return capture[d(rng)];
}

// --- LEVEL 4 ---
// We use a minimax search algorithm with depth = 3

// Find the current score of the position (my material value - their material valeu)
int Level4::eval(const Board& b, Colour side) const {
    int score = 0;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
        const Piece* p = b.getPieceAt({ row, col });
        if (!p) continue;
        int value = static_cast<int>(p->getMaterialValue() * 100);
        score += (p->getColour() == side) ? value : -value;
        }
    }
    return score;
}

// Minmax Algorithm
int Level4::minimax(Board& b, int depth, Colour me, bool maximizing, Move& bestOut, const Move& prev){
    Colour side = maximizing ? me : opp(me); // Which side we are on depending on min or maxing
    if (depth == 0) { // If we reach our desired depth, evaluate the position
        bestOut = {};
        return eval(b, me);
    }

    // Next we look if there's checkmate
    auto moves = allMoves(b, side);
    if (moves.empty()) {
        Coordinate king = findKing(b, side);
        if (b.isDanger(king, side)){ // If there are no legal moves and the king is in danger, its mate.
            return (side == me) ? -67000 + depth : 67000 - depth; // Either really bad if its us (negative) or really good
        }
        return 0; //Otherwise its a stalemate
        }

    // Finally we evaluate other moves
    int bestVal = maximizing ? -67000 : 67000; //absolute worst/best value for us (We want to get this higher/lower)
    Move bestMove{};
    for (const auto& m : moves){
        Board temp = b;
        temp.applyMove(m);
        Move ignore{};
         const Move nextPrev = maximizing ? m : prev;   
        int score = minimax(temp, depth - 1, me, !maximizing, ignore, nextPrev); // Get the score of the next ply

        // Score penalty if the algorithm tries to shuffle
         if (prev.from.row == m.to.row && prev.from.col == m.to.col &&
            prev.to.row   == m.from.row && prev.to.col   == m.from.col) {
            score -= 10;
        }

        // Calculate the best move based on our score.
        // If the score in the next ply is better than current, switch dat (2 ways depending on max/min)
        if (maximizing) {
            if (score > bestVal) { 
                bestVal = score;
                bestMove = m;
                }
        }
        else if (score < bestVal) { 
            bestVal = score;
            bestMove = m;
            }
    }
    bestOut = bestMove;
    return bestVal;
}

Move Level4::chooseMove(const Board& b, Colour side){ // Now just choose the best move output by the algo
    Board temp = b;
    Move best{};
    minimax(temp, ply, side, true, best, Move{});
    if (b.isMoveValid(best)) return best;

    // If there is somehow no best move, just return a move at random (failsafe)
    auto v = allMoves(b, side);
    if (v.empty()) return {}; 
    std::uniform_int_distribution<size_t> d(0, v.size()-1);
    return v[d(rng)];
}

// Factory(Strategy) method for choosing level
std::unique_ptr<Strategy> makeStrategy(Level lvl) {
    switch (lvl) {
        case Level::L2: return std::make_unique<Level2>();
        case Level::L3: return std::make_unique<Level3>();
        case Level::L4: return std::make_unique<Level4>();
        default:        return std::make_unique<Level1>();
    }
}

// hawk TUAH!!
