#pragma once

#include "Move.h"
#include <cstdint>


enum TTFlag
{
    TT_EXACT,   // exact score
    TT_ALPHA,   // upper bound
    TT_BETA     // lower bound
};


void initTT();

bool probeTT(uint64_t key, int depth, int alpha, int beta, int ply, int& outScore, Move& outMove);

// Stores (or overwrites) the result for key
void storeTT(uint64_t key, int depth, int score, int ply, TTFlag flag, const Move& bestMove);

Move getTTMove(uint64_t key);
