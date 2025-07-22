#include "strategy.h"
#include "../Model/board.h"
#include "../Model/piece.h"
#include <random>
#include <cctype>


// Mersenne random number generator
static std::mt19937 rng(std::random_device{}());


// Helper to get all valid moves for all pieces
static std::vector<Move> allMoves(const Board& b, Colour side) {
    std::vector<Move> v; //Vector for all possible moves
    for (int r = 0; r < 8; ++r) //Iterate rank then file wise
        for (int c = 0; c < 8; ++c) {
            const Piece* p = b.getPieceAt({r,c}); //Get piece at the coords
            if (p && p->getColour() == side) { //If there is a piece there, check if its the proper colour
                auto mv = p->getValidMoves(b); // Get all the piece's legal moves
                v.insert(v.end(), mv.begin(), mv.end()); // Add all moves to the big vector
            }
        }
    return v;
}

// --- LEVEL 1 ---
// Fully random move
Move Level1::chooseMove(const Board& b, Colour side) {
    auto v = allMoves(b, side); // Get all valid moves
    if (v.empty()) return {};
    // Then we build a uniform distribution with all legal moves
    std::uniform_int_distribution<size_t> d(0, v.size()-1);
    return v[d(rng)]; // Return one at random
}

// --- LEVEL 2 ---
// Prioritize captures and checks
Move Level2::chooseMove(const Board& b, Colour side) {
    auto v = allMoves(b, side); // Get all valid moves
    if (v.empty()) return {};
    std::vector<Move> satisfied; // Vector for all moves that satisfy conditions
    satisfied.reserve(v.size()); 
    for (const auto& m : v) {
        const Piece* target = b.getPieceAt(m.to); // Get target square
        // If there is a piece at target square, add it to the new vector
        if ((target && target->getColour() != side)
        || (target && std::tolower(target->getCharRepresentation()) == 'k' && target->getColour() != side)) {
            satisfied.push_back(m); }
    } // Now if there are no capturing/checking moves, we just use the regular set. 
    if (satisfied.empty()) satisfied = v;
    std::uniform_int_distribution<size_t> d(0, satisfied.size()-1);
    return satisfied[d(rng)];
}

// --- LEVEL 3 ---
// Prioritize avoiding capture, capturing, and checks
Move Level3::chooseMove(const Board& b, Colour side) {
    auto v = allMoves(b, side); // Get all valid moves
    if (v.empty()) return {};
    std::vector<Move> satisfied; // Vector for all moves that satisfy
    satisfied.reserve(v.size()); 
    for (const auto& m : v) {
        const Piece* target = b.getPieceAt(m.to); // Get target square

        // First if will be non dangeous capturing/checking moves
        if ((target && target->getColour() != side && !b.isDanger(m.to, side))
        || (target && std::tolower(target->getCharRepresentation()) == 'k' && target->getColour() != side && !b.isDanger(m.to, side))) {
            satisfied.push_back(m); }

        // Next if there are no safe capturing/checking moves, we'll just move safely.
        if (!target && !b.isDanger(m.to, side)) satisfied.push_back(m);

    } // If there are no satisfiable moves, we just use the regular set. 
    if (satisfied.empty()) satisfied = v;
    std::uniform_int_distribution<size_t> d(0, satisfied.size()-1);
    return satisfied[d(rng)];
}

// --- LEVEL 4 ---
// Proper ai. Probably use openai key


std::unique_ptr<Strategy> makeStrategy(Level lvl) {
    switch (lvl) {
        case Level::L2: return std::make_unique<Level2>();
        case Level::L3: return std::make_unique<Level3>();
        default:       return std::make_unique<Level1>();
    }
}
