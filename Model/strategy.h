#ifndef STRATEGY_H
#define STRATEGY_H
#include "../types/types.h"
#include <memory>

class Board;


enum class Level { L1, L2, L3 };

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
    
    std::unique_ptr<Strategy> makeStrategy(Level lvl);
    
    
    #endif
