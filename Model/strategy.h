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
            int depthLevel = 3;
            int eval(const Board& b, Colour side) const;
            int minimax(Board& b, int depth, Colour root, bool maximizing, Move& bestOut, const Move& prev);
};

    std::unique_ptr<Strategy> makeStrategy(Level lvl);
    
    
#endif
