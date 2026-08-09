#ifndef ATTACKS_H
#define ATTACKS_H

#include "BoardState.h"

// Used for check detection, castling rules, and move legality.
bool isSquareAttacked(int square, bool byWhite, boardState& pos);

#endif
