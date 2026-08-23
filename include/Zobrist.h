#pragma once

#include "BoardState.h"
#include <cstdint>


void initZobrist();

// Returns the 64-bit hash of the current position
uint64_t zobristHash(const boardState& pos);
