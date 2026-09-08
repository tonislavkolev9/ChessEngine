#pragma once

#include "Move.h"
#include <cstdint>


enum TTFlag
{
    TT_EXACT,
    TT_ALPHA,
    TT_BETA
};


void initTT();

bool probeTT(uint64_t key, int depth, int alpha, int beta, int ply, int& outScore, Move& outMove);


void storeTT(uint64_t key, int depth, int score, int ply, TTFlag flag, const Move& bestMove);

Move getTTMove(uint64_t key);
