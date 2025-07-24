#ifndef STRATEGY_H
#define STRATEGY_H
#include "../types/types.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <cstdint>

class Board;


enum class Level { L1, L2, L3, L4 };

class Strategy {
    public:
        virtual ~Strategy() = default;
        virtual Move chooseMove(const Board& b, Colour side) = 0;
    };
    
    class Level1 : public Strategy {
    public:
        Move chooseMove(const Board& b, Colour side) override;
    };
    
    class Level2 : public Strategy {
    public:
        Move chooseMove(const Board& b, Colour side) override;
    };

    class Level3 : public Strategy {
        public:
            Move chooseMove(const Board& b, Colour side) override;
        };
    
     class Level4 : public Strategy {
        public:
            Move chooseMove(const Board& b, Colour side) override;

private:
    // NEW: principal variation keeps the planned line so we "remember" next moves
    std::vector<Move> pv;

    // NEW: simple transposition table entry
    struct TTEntry {
        int depth;
        int score;
        Move best;
        enum Flag { EXACT, LOWER, UPPER } flag;
    };
    // NEW: zobrist-keyed TT
    std::unordered_map<uint64_t, TTEntry> tt;

    int maxDepth = 4;                 // NEW: search depth knob
    int search(Board& b, int depth, int alpha, int beta, Colour me, Move& bestOut); // NEW
    int quiescence(Board& b, int alpha, int beta, Colour me);                       // NEW
    int eval(const Board& b, Colour me) const;                                      // NEW
    uint64_t hash(const Board& b) const;                                            // NEW
};

    std::unique_ptr<Strategy> makeStrategy(Level lvl);
    
    
    #endif
