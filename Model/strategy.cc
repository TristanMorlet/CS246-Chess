#include "strategy.h"
#include "../Model/board.h"
#include "../Model/piece.h"
#include <random>
#include <cctype>
#include <cmath>
#include "../types/types.h"
#include <algorithm>
#include <unordered_map>
#include <cstdint>

// Forward dec
static std::vector<Move> allMoves(const Board&, Colour);
static Coordinate        findKing(const Board&, Colour);
static bool              givesCheck(const Board&, const Move&, Colour);
static bool              givesMate(const Board&, const Move&, Colour);
static bool              safeAfter(const Board&, const Move&, Colour);

//LVL 4 Score Values
static const double S_MATE        = 14.0;
static const double S_PROMO       = 13.0;
static const double S_SAFE_CAP    = 10.0;
static const double S_UNSAFE_CAP  = 4.0;
static const double S_CHECK       = 9.0;
static const double S_SAVE        = 8.0;
static const double S_CASTLE      = 5.0;
static const double S_QUIET       = 3.0;
static std::mt19937 rng(std::random_device{}());

static inline Colour opp(Colour c){ return c==Colour::White ? Colour::Black : Colour::White; }

static int matVal(char ch){
    switch(ch){
        case 'P': case 'p': return 100;
        case 'N': case 'n': return 320;
        case 'B': case 'b': return 330;
        case 'R': case 'r': return 500;
        case 'Q': case 'q': return 900;
        case 'K': case 'k': return 0;
        default: return 0;
    }
}

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

static Coordinate findKing(const Board& b, Colour who){
    for(int r=0;r<8;++r)for(int c=0;c<8;++c){
        const Piece* p=b.getPieceAt({r,c});
        if(p && p->getColour()==who && std::tolower(p->getCharRepresentation())=='k') return {r,c};
    }
    return {-1,-1};
}

static bool givesCheck(const Board& b, const Move& m, Colour me){
    Board tmp = b;
    tmp.applyMove(m);
    Colour opp = (me==Colour::White)?Colour::Black:Colour::White;
    Coordinate k = findKing(tmp, opp);
    return tmp.isDanger(k, opp);
}

static bool givesMate(const Board& b, const Move& m, Colour me){
    Board tmp = b;
    tmp.applyMove(m);
    Colour opp = (me==Colour::White)?Colour::Black:Colour::White;
    Coordinate k = findKing(tmp, opp);
    if(!tmp.isDanger(k, opp)) return false;
    for(int r=0;r<8;++r)for(int c=0;c<8;++c){
        const Piece* p=tmp.getPieceAt({r,c});
        if(!p || p->getColour()!=opp) continue;
        for(const auto& mv : p->getValidMoves(tmp))
            if(tmp.isMoveValid(mv)) return false;
    }
    return true;
}

static bool safeAfter(const Board& b, const Move& m, Colour side){
    Board tmp = b;
    tmp.applyMove(m);
    return !tmp.isDanger(m.to, side);
}

// --- LEVEL 1 ---
Move Level1::chooseMove(const Board& b, Colour side) {
    auto v = allMoves(b, side);
    if (v.empty()) return {};
    std::uniform_int_distribution<size_t> d(0, v.size()-1);
    return v[d(rng)];
}

// --- LEVEL 2 ---
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
Move Level3::chooseMove(const Board& b, Colour side) {
    auto v = allMoves(b, side);
    if (v.empty()) return {};
    std::vector<Move> capture;
    std::vector<Move> nocap;
    capture.reserve(v.size());
    nocap.reserve(v.size());
    for (const auto& m : v) {
        const Piece* target = b.getPieceAt(m.to);
        if ((target && target->getColour() != side && safeAfter(b, m, side)) || givesCheck(b, m, side) || givesMate(b, m, side)) {
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
int Level4::eval(const Board& b, Colour me) const {
    int s = 0;
    for(int r=0;r<8;++r)for(int c=0;c<8;++c){
        const Piece* p = b.getPieceAt({r,c});
        if(!p) continue;
        int v = matVal(p->getCharRepresentation());
        s += (p->getColour()==me)? v : -v;
    }
    return s;
}

int Level4::quiescence(Board& b, int alpha, int beta, Colour me){
    int stand = eval(b, me);
    if(stand >= beta) return beta;
    if(stand > alpha) alpha = stand;

    auto moves = allMoves(b, me);
    for(const auto& m : moves){
        const Piece* tgt = b.getPieceAt(m.to);
        if(!tgt && !givesCheck(b, m, me)) continue;
        Board child = b;
        child.applyMove(m);
        int score = -quiescence(child, -beta, -alpha, opp(me));
        if(score >= beta) return beta;
        if(score > alpha) alpha = score;
    }
    return alpha;
}

int Level4::search(Board& b, int depth, int alpha, int beta, Colour me, Move& bestOut){
    uint64_t h = hash(b);
    auto it = tt.find(h);
    if(it != tt.end() && it->second.depth >= depth){
        const auto& e = it->second;
        if(e.flag == TTEntry::EXACT){ bestOut = e.best; return e.score; }
        if(e.flag == TTEntry::LOWER && e.score > alpha) alpha = e.score;
        else if(e.flag == TTEntry::UPPER && e.score < beta) beta = e.score;
        if(alpha >= beta){ bestOut = e.best; return e.score; }
    }

    if(depth == 0) return quiescence(b, alpha, beta, me);

    auto moves = allMoves(b, me);
    if(moves.empty()){
        Coordinate k = findKing(b, me);
        if(b.isDanger(k, me)) return -30000 + (4 - depth);
        return 0;
    }

    std::sort(moves.begin(), moves.end(), [&](const Move& a, const Move& bmv){
        const Piece* ta = b.getPieceAt(a.to);
        const Piece* tb = b.getPieceAt(bmv.to);
        int ca = (givesMate(b,a,me)?30000:(givesCheck(b,a,me)?5000:0)) + (ta? matVal(ta->getCharRepresentation()) : 0);
        int cb = (givesMate(b,bmv,me)?30000:(givesCheck(b,bmv,me)?5000:0)) + (tb? matVal(tb->getCharRepresentation()) : 0);
        return ca > cb;
    });

    Move localBest{};
    int origAlpha = alpha;

    for(const auto& m : moves){
        Board child = b;
        child.applyMove(m);
        Move ignore{};
        int score = -search(child, depth-1, -beta, -alpha, opp(me), ignore);
        if(score > alpha){
            alpha = score;
            localBest = m;
            if(alpha >= beta) break;
        }
    }

    TTEntry e;
    e.depth = depth;
    e.score = alpha;
    e.best  = localBest;
    if(alpha <= origAlpha) e.flag = TTEntry::UPPER;
    else if(alpha >= beta) e.flag = TTEntry::LOWER;
    else e.flag = TTEntry::EXACT;
    tt[h] = e;

    bestOut = localBest;
    return alpha;
}

uint64_t Level4::hash(const Board& b) const{
    static uint64_t zob[12][64];
    static bool init=false;
    if(!init){
        std::mt19937_64 r(20250724);
        for(int p=0;p<12;++p)for(int s=0;s<64;++s) zob[p][s]=r();
        init=true;
    }
    uint64_t h=0;
    for(int r=0;r<8;++r)for(int c=0;c<8;++c){
        const Piece* p=b.getPieceAt({r,c});
        if(!p) continue;
        char ch= p->getCharRepresentation();
        int idx;
        switch(ch){
            case 'P': idx=0;break; case 'N': idx=1;break; case 'B': idx=2;break;
            case 'R': idx=3;break; case 'Q': idx=4;break; case 'K': idx=5;break;
            case 'p': idx=6;break; case 'n': idx=7;break; case 'b': idx=8;break;
            case 'r': idx=9;break; case 'q': idx=10;break; default: idx=11;break;
        }
        h ^= zob[idx][r*8+c];
    }
    return h;
}

Move Level4::chooseMove(const Board& b, Colour side){
    if(!pv.empty()){
        Move m = pv.front();
        if(b.isMoveValid(m)){ pv.erase(pv.begin()); return m; }
        pv.clear();
    }

    Move best{};
    for(int d=1; d<=maxDepth; ++d){
        Board copy = b;
        Move tmp{};
        search(copy, d, -30000, 30000, side, tmp);
        if(b.isMoveValid(tmp)) best = tmp;
    }

    if(!b.isMoveValid(best)){
        auto v = allMoves(b, side);
        if(v.empty()) return {};
        std::uniform_int_distribution<size_t> dist(0, v.size()-1);
        return v[dist(rng)];
    }

    pv.clear();
    pv.push_back(best);
    return best;
}

std::unique_ptr<Strategy> makeStrategy(Level lvl) {
    switch (lvl) {
        case Level::L2: return std::make_unique<Level2>();
        case Level::L3: return std::make_unique<Level3>();
        case Level::L4: return std::make_unique<Level4>();
        default:        return std::make_unique<Level1>();
    }
}
