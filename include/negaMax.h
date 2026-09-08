#pragma once

#include "BoardState.h"
#include "Move.h"
#include <vector>
#include <cstdint>

int negaMax(boardState& pos, int depth, int alpha, int beta, int ply = 0);
Move searchBestMove(boardState& pos, int depth, int alpha = -1000000, int beta = 1000000, int* outScore = nullptr);


Move iterativeDeepening(boardState& pos, int maxDepth, long long timeLimitMs = 0);
int quiescence(boardState& pos, int alpha, int beta);


extern std::vector<uint64_t> gameHistory;

void resetGameHistory();