#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include "Piece.h"
#include <string>


struct boardState {
    Piece board[64];

    bool whiteToMove = true;

    int enPassantSquare = -1; // -1 means no en passant target

    int white_pawns_list[MAX_PAWNS];
    int white_rooks_list[MAX_ROOKS];
    int white_knights_list[MAX_KNIGHTS];
    int white_bishops_list[MAX_BISHOPS];
    int white_queens_list[MAX_QUEENS];
    int white_kings_list[MAX_KINGS];


    int white_list_board[64];


    int nWhitePawns = 8;
    int nWhiteRooks = 2;
    int nWhiteKnights = 2;
    int nWhiteBishops = 2;
    int nWhiteQueens = 1;
    int nWhiteKings = 1;


    int black_pawns_list[MAX_PAWNS];
    int black_rooks_list[MAX_ROOKS];
    int black_knights_list[MAX_KNIGHTS];
    int black_bishops_list[MAX_BISHOPS];
    int black_queens_list[MAX_QUEENS];
    int black_kings_list[MAX_KINGS];

    int black_list_board[64];

    int nBlackPawns = 8;
    int nBlackRooks = 2;
    int nBlackKnights = 2;
    int nBlackBishops = 2;
    int nBlackQueens = 1;
    int nBlackKings = 1;


    bool hasWhiteKingMoved = false;
    bool hasBlackKingMoved = false;

    bool hasWhiteKingsideRookMoved = false;   // h1 rook
    bool hasWhiteQueensideRookMoved = false;  // a1 rook
    bool hasBlackKingsideRookMoved = false;   // h8 rook
    bool hasBlackQueensideRookMoved = false;  // a8 rook
};

void initStartPosition(boardState& pos);

void loadFEN(boardState& pos, const std::string& fen);

#endif
