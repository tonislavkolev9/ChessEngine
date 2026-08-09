#ifndef MOVE_H
#define MOVE_H

#include "Piece.h"
#include <string>

struct Move;
std::string moveToString(const Move& move);

struct Move {
    int from;
    int to;

    Piece captured = EMP;
    int capturedIndex = -1; // Piece list index for undo

    Piece promotion = EMP;
    int oldEnPassant = -1;


    bool isEnPassant = false;

    bool oldWhiteKingMoved;
    bool oldBlackKingMoved; 
    bool oldWhiteKingsideRookMoved;
    bool oldWhiteQueensideRookMoved;
    bool oldBlackKingsideRookMoved;
    bool oldBlackQueensideRookMoved;

    bool oldWhiteToMove;
};

#endif
