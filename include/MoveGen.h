#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "BoardState.h"
#include "Move.h"

// Shared move buffer reused each generation
extern Move movelist[256];
extern int moveCount;

// Generates pseudo-legal moves
void generateMove(boardState& pos);

void generateLegalMoves(boardState& pos);

#endif // MOVEGEN_H
