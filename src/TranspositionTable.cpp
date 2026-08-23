#include "TranspositionTable.h"
#include <vector>

struct TTEntry
{
    uint64_t key = 0;
    int depth = -1;
    int score = 0;
    TTFlag flag = TT_EXACT;
    Move bestMove;
    bool occupied = false;
};

static const size_t TT_SIZE = 1u << 20;
static std::vector<TTEntry> table;

static const int MATE_THRESHOLD = 900000;

static int scoreToTT(int score, int ply) {
    if (score >= MATE_THRESHOLD) return score + ply;
    if (score <= -MATE_THRESHOLD) return score - ply;
    return score;
}

static int scoreFromTT(int score, int ply) {
    if (score >= MATE_THRESHOLD) return score - ply;
    if (score <= -MATE_THRESHOLD) return score + ply;
    return score;
}

void initTT()
{
    table.assign(TT_SIZE, TTEntry{});
}

bool probeTT(uint64_t key, int depth, int alpha, int beta, int ply, int& outScore, Move& outMove)
{
    TTEntry& entry = table[key & (TT_SIZE - 1)];

    if (!entry.occupied || entry.key != key)
        return false;

    outMove = entry.bestMove;

    if (entry.depth < depth)
        return false;

    int ttScore = scoreFromTT(entry.score, ply);

    if (entry.flag == TT_EXACT)
    {
        outScore = ttScore;
        return true;
    }

    if (entry.flag == TT_ALPHA && ttScore <= alpha)
    {
        outScore = alpha;
        return true;
    }

    if (entry.flag == TT_BETA && ttScore >= beta)
    {
        outScore = beta;
        return true;
    }

    return false;
}

void storeTT(uint64_t key, int depth, int score, int ply, TTFlag flag, const Move& bestMove)
{
    TTEntry& entry = table[key & (TT_SIZE - 1)];

    entry.key = key;
    entry.depth = depth;
    entry.score = scoreToTT(score, ply);
    entry.flag = flag;
    entry.bestMove = bestMove;
    entry.occupied = true;
}

Move getTTMove(uint64_t key)
{
    const TTEntry& entry = table[key & (TT_SIZE - 1)];

    if (entry.occupied && entry.key == key)
        return entry.bestMove;

    return Move();
}
