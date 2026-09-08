#pragma once

#include "BoardState.h"
#include <cstdint>


void initZobrist();


uint64_t zobristHash(const boardState& pos);
