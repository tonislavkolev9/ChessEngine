#pragma once
#include "BoardState.h"

long long perft(boardState& pos, int depth);
void perftDivide(boardState& pos, int depth);