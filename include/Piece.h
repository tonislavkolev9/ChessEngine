#ifndef PIECE_H
#define PIECE_H

const int MAX_PAWNS = 16;
const int MAX_ROOKS = 20;
const int MAX_KNIGHTS = 20;
const int MAX_BISHOPS = 20;
const int MAX_QUEENS = 18;
const int MAX_KINGS = 2;

enum Piece 
{
    EMP = -1,
    WP = 0, WR = 1, WN, WB, WQ, WK,
    BP, BR, BN, BB, BQ, BK
};

#endif
