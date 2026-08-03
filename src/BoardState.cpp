#include "BoardState.h"
#include <iostream>
#include <sstream>
#include <cctype>


void initStartPosition(boardState& pos) {

    Piece board[64] = {
        BR, BN, BB, BQ, BK, BB, BN, BR,
        BP, BP, BP, BP, BP, BP, BP, BP,
        EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP,
        EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP,
        EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP,
        EMP, EMP, EMP, EMP, EMP, EMP, EMP, EMP,
        WP, WP, WP, WP, WP, WP, WP, WP,
        WR, WN, WB, WQ, WK, WB, WN, WR,
    };

    // Copy the local board into the real boardState
    for (int i = 0; i < 64; i++)
        pos.board[i] = board[i];

    int whitePawns[]   = {48,49,50,51,52,53,54,55};
    int whiteRooks[]   = {56,63};
    int whiteKnights[] = {57,62};
    int whiteBishops[] = {58,61};

    int blackPawns[]   = {8,9,10,11,12,13,14,15};
    int blackRooks[]   = {0,7};
    int blackKnights[] = {1,6};
    int blackBishops[] = {2,5};


    for (int i = 0; i < 64; i++) {
        pos.white_list_board[i] = -1;
        pos.black_list_board[i] = -1;
    }

    for (int i = 0; i < 8; i++) {
        pos.white_pawns_list[i] = whitePawns[i];
        pos.white_list_board[whitePawns[i]] = i;

        pos.black_pawns_list[i] = blackPawns[i];
        pos.black_list_board[blackPawns[i]] = i;
    }

    for (int i = 0; i < 2; i++) {
        pos.white_rooks_list[i] = whiteRooks[i];
        pos.white_list_board[whiteRooks[i]] = i;

        pos.white_knights_list[i] = whiteKnights[i];
        pos.white_list_board[whiteKnights[i]] = i;

        pos.white_bishops_list[i] = whiteBishops[i];
        pos.white_list_board[whiteBishops[i]] = i;

        pos.black_rooks_list[i] = blackRooks[i];
        pos.black_list_board[blackRooks[i]] = i;

        pos.black_knights_list[i] = blackKnights[i];
        pos.black_list_board[blackKnights[i]] = i;

        pos.black_bishops_list[i] = blackBishops[i];
        pos.black_list_board[blackBishops[i]] = i;
    }

    pos.white_queens_list[0] = 59;
    pos.white_kings_list[0] = 60;
    pos.white_list_board[59] = 0;
    pos.white_list_board[60] = 0;

    pos.black_queens_list[0] = 3;
    pos.black_kings_list[0] = 4;
    pos.black_list_board[3] = 0;
    pos.black_list_board[4] = 0;


    pos.nWhitePawns = 8;
    pos.nWhiteRooks = 2;
    pos.nWhiteKnights = 2;
    pos.nWhiteBishops = 2;
    pos.nWhiteQueens = 1;
    pos.nWhiteKings = 1;

    pos.nBlackPawns = 8;
    pos.nBlackRooks = 2;
    pos.nBlackKnights = 2;
    pos.nBlackBishops = 2;
    pos.nBlackQueens = 1;
    pos.nBlackKings = 1;


    pos.whiteToMove = true;
    pos.enPassantSquare = -1;

    pos.hasWhiteKingMoved = false;
    pos.hasBlackKingMoved = false;
    pos.hasWhiteKingsideRookMoved = false;
    pos.hasWhiteQueensideRookMoved = false;
    pos.hasBlackKingsideRookMoved = false;
    pos.hasBlackQueensideRookMoved = false;
}


void loadFEN(boardState& pos, const std::string& fen) {

    for (int i = 0; i < 64; i++) {
        pos.board[i] = EMP;
        pos.white_list_board[i] = -1;
        pos.black_list_board[i] = -1;
    }

    pos.nWhitePawns = pos.nWhiteRooks = pos.nWhiteKnights = 0;
    pos.nWhiteBishops = pos.nWhiteQueens = pos.nWhiteKings = 0;
    pos.nBlackPawns = pos.nBlackRooks = pos.nBlackKnights = 0;
    pos.nBlackBishops = pos.nBlackQueens = pos.nBlackKings = 0;

    std::istringstream iss(fen);
    std::string placement, activeColor, castling, epSquare;


    iss >> placement >> activeColor >> castling >> epSquare;

    int sq = 0;
    for (char c : placement) {

        if (c == '/')
            continue;

        if (std::isdigit(static_cast<unsigned char>(c))) {
            sq += (c - '0');
            continue;
        }

        if (sq < 0 || sq >= 64)
            break;

        Piece p = EMP;
        switch (c) {
            case 'P': p = WP; break;
            case 'N': p = WN; break;
            case 'B': p = WB; break;
            case 'R': p = WR; break;
            case 'Q': p = WQ; break;
            case 'K': p = WK; break;
            case 'p': p = BP; break;
            case 'n': p = BN; break;
            case 'b': p = BB; break;
            case 'r': p = BR; break;
            case 'q': p = BQ; break;
            case 'k': p = BK; break;
            default: break;
        }

        pos.board[sq] = p;

        switch (p) {
            case WP: pos.white_pawns_list[pos.nWhitePawns] = sq; pos.white_list_board[sq] = pos.nWhitePawns++; break;
            case WN: pos.white_knights_list[pos.nWhiteKnights] = sq; pos.white_list_board[sq] = pos.nWhiteKnights++; break;
            case WB: pos.white_bishops_list[pos.nWhiteBishops] = sq; pos.white_list_board[sq] = pos.nWhiteBishops++; break;
            case WR: pos.white_rooks_list[pos.nWhiteRooks] = sq; pos.white_list_board[sq] = pos.nWhiteRooks++; break;
            case WQ: pos.white_queens_list[pos.nWhiteQueens] = sq; pos.white_list_board[sq] = pos.nWhiteQueens++; break;
            case WK: pos.white_kings_list[pos.nWhiteKings] = sq; pos.white_list_board[sq] = pos.nWhiteKings++; break;
            case BP: pos.black_pawns_list[pos.nBlackPawns] = sq; pos.black_list_board[sq] = pos.nBlackPawns++; break;
            case BN: pos.black_knights_list[pos.nBlackKnights] = sq; pos.black_list_board[sq] = pos.nBlackKnights++; break;
            case BB: pos.black_bishops_list[pos.nBlackBishops] = sq; pos.black_list_board[sq] = pos.nBlackBishops++; break;
            case BR: pos.black_rooks_list[pos.nBlackRooks] = sq; pos.black_list_board[sq] = pos.nBlackRooks++; break;
            case BQ: pos.black_queens_list[pos.nBlackQueens] = sq; pos.black_list_board[sq] = pos.nBlackQueens++; break;
            case BK: pos.black_kings_list[pos.nBlackKings] = sq; pos.black_list_board[sq] = pos.nBlackKings++; break;
            default: break;
        }

        sq++;
    }

    pos.whiteToMove = (activeColor != "b");


    pos.hasWhiteKingMoved = false;
    pos.hasBlackKingMoved = false;
    pos.hasWhiteKingsideRookMoved  = (castling.find('K') == std::string::npos);
    pos.hasWhiteQueensideRookMoved = (castling.find('Q') == std::string::npos);
    pos.hasBlackKingsideRookMoved  = (castling.find('k') == std::string::npos);
    pos.hasBlackQueensideRookMoved = (castling.find('q') == std::string::npos);

    if (epSquare == "-" || epSquare.size() < 2) {
        pos.enPassantSquare = -1;
    } else {
        int file = epSquare[0] - 'a';
        int rank = epSquare[1] - '1';

        if (file < 0 || file > 7 || rank < 0 || rank > 7) {
            pos.enPassantSquare = -1;
        } else {
            int row = 7 - rank;
            pos.enPassantSquare = row * 8 + file;
        }
    }
}