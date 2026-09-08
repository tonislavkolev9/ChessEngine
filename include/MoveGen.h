#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "BoardState.h"
#include "Move.h"

extern Move movelist[256];
extern int moveCount;


void generateMove(boardState& pos);

void generateLegalMoves(boardState& pos);

#endif
