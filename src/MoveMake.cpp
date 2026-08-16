#include "MoveMake.h"
#include <cstdlib>


void makeMove(Move& move, boardState& pos) {

    move.oldWhiteToMove = pos.whiteToMove;

    move.oldWhiteKingMoved = pos.hasWhiteKingMoved;
    move.oldBlackKingMoved = pos.hasBlackKingMoved;
    move.oldWhiteKingsideRookMoved = pos.hasWhiteKingsideRookMoved;
    move.oldWhiteQueensideRookMoved = pos.hasWhiteQueensideRookMoved;
    move.oldBlackKingsideRookMoved = pos.hasBlackKingsideRookMoved;
    move.oldBlackQueensideRookMoved = pos.hasBlackQueensideRookMoved;

    int square;

    move.oldEnPassant = pos.enPassantSquare;
    move.isEnPassant = false;

    int from = move.from;
    int to = move.to;
    Piece promotion = move.promotion;

    Piece moving = pos.board[from];
    Piece captured = pos.board[to];

    move.captured = captured;
    move.capturedIndex = -1;


    int capturedSquare = to;

    
    bool isDiagonalPawnMove = abs((to % 8) - (from % 8)) == 1;

    if (moving == WP && isDiagonalPawnMove && to == pos.enPassantSquare && pos.board[to] == EMP) {
        captured = BP;
        move.captured = BP;
        capturedSquare = to + 8;
        move.isEnPassant = true;
        pos.board[capturedSquare] = EMP;
    }

  if (moving == BP && isDiagonalPawnMove && to == pos.enPassantSquare && pos.board[to] == EMP) {
        captured = WP;
        move.captured = WP;
        capturedSquare = to - 8;
        move.isEnPassant = true;
        pos.board[capturedSquare] = EMP;
    }

    if (captured != EMP) {
        switch (captured) {

    case WP: {
        pos.nWhitePawns--;
        int i = pos.white_list_board[capturedSquare];
        move.capturedIndex = i;
        square = pos.white_pawns_list[pos.nWhitePawns];
        pos.white_pawns_list[i] = square;              
        pos.white_list_board[square] = i;              
        pos.white_list_board[capturedSquare] = -1; 
        break;
    }

    case WR: {
        pos.nWhiteRooks--;                           
        int i = pos.white_list_board[to];     
        move.capturedIndex = i;               
        square = pos.white_rooks_list[pos.nWhiteRooks]; 
        pos.white_rooks_list[i] = square;
        pos.white_list_board[square] = i;
        pos.white_list_board[to] = -1;
        break;
    }

    case WN: {
        pos.nWhiteKnights--;
        int i = pos.white_list_board[to];
        move.capturedIndex = i;
        square = pos.white_knights_list[pos.nWhiteKnights];
        pos.white_knights_list[i] = square;
        pos.white_list_board[square] = i;
        pos.white_list_board[to] = -1;
        break;
    }

    case WB: {
        pos.nWhiteBishops--;
        int i = pos.white_list_board[to];
        move.capturedIndex = i; 
        square = pos.white_bishops_list[pos.nWhiteBishops];
        pos.white_bishops_list[i] = square;
        pos.white_list_board[square] = i;
        pos.white_list_board[to] = -1;
        break;
    }

    case WQ: {
        pos.nWhiteQueens--;
        int i = pos.white_list_board[to];
        move.capturedIndex = i; 
        square = pos.white_queens_list[pos.nWhiteQueens];
        pos.white_queens_list[i] = square;
        pos.white_list_board[square] = i;
        pos.white_list_board[to] = -1;
        break;
    }

    case WK: {
        pos.nWhiteKings--;
        int i = pos.white_list_board[to];
        move.capturedIndex = i; 
        square = pos.white_kings_list[pos.nWhiteKings];
        pos.white_kings_list[i] = square;
        pos.white_list_board[square] = i;
        pos.white_list_board[to] = -1;
        break;
    }

    case BP: {
        pos.nBlackPawns--;
        int i = pos.black_list_board[capturedSquare];
        move.capturedIndex = i; 
        square = pos.black_pawns_list[pos.nBlackPawns];
        pos.black_pawns_list[i] = square;
        pos.black_list_board[square] = i;
        pos.black_list_board[capturedSquare] = -1;
        break;
    }

        case BR: {
        pos.nBlackRooks--;
        int i = pos.black_list_board[to];
        move.capturedIndex = i; 
        square = pos.black_rooks_list[pos.nBlackRooks];
        pos.black_rooks_list[i] = square;
        pos.black_list_board[square] = i;
        pos.black_list_board[to] = -1;
        break;
    }

    case BN: {
        pos.nBlackKnights--;
        int i = pos.black_list_board[to];
        move.capturedIndex = i; 
        square = pos.black_knights_list[pos.nBlackKnights];
        pos.black_knights_list[i] = square;
        pos.black_list_board[square] = i;
        pos.black_list_board[to] = -1;
        break;
    }

    case BB: {
        pos.nBlackBishops--;
        int i = pos.black_list_board[to];
        move.capturedIndex = i; 
        square = pos.black_bishops_list[pos.nBlackBishops];
        pos.black_bishops_list[i] = square;
        pos.black_list_board[square] = i;
        pos.black_list_board[to] = -1;
        break;
    }

    case BQ: {
        pos.nBlackQueens--;
        int i = pos.black_list_board[to];
        move.capturedIndex = i; 
        square = pos.black_queens_list[pos.nBlackQueens];
        pos.black_queens_list[i] = square;
        pos.black_list_board[square] = i;
        pos.black_list_board[to] = -1;
        break;
    }

    case BK: {
        pos.nBlackKings--;
        int i = pos.black_list_board[to];
        move.capturedIndex = i; 
        square = pos.black_kings_list[pos.nBlackKings];
        pos.black_kings_list[i] = square;
        pos.black_list_board[square] = i;
        pos.black_list_board[to] = -1;
        break;
    }

        }
    }

    if (captured == WR && to == 63)
    pos.hasWhiteKingsideRookMoved = true;

    if (captured == WR && to == 56)
        pos.hasWhiteQueensideRookMoved = true;

    if (captured == BR && to == 7)
        pos.hasBlackKingsideRookMoved = true;

    if (captured == BR && to == 0)
        pos.hasBlackQueensideRookMoved = true;


    switch (pos.board[from]) {

    case WP: {
        int i = pos.white_list_board[from];

        if (promotion == EMP) {
            pos.white_pawns_list[i] = to;
            pos.white_list_board[to] = i;
            pos.white_list_board[from] = -1;
        } else {
            square = pos.white_pawns_list[pos.nWhitePawns - 1];
            pos.nWhitePawns--;
            pos.white_pawns_list[i] = square;
            pos.white_list_board[square] = i;
            pos.white_list_board[from] = -1;

            switch (promotion) {
            case WQ:
                pos.white_queens_list[pos.nWhiteQueens] = to;
                pos.white_list_board[to] = pos.nWhiteQueens++;
                break;
            case WR:
                pos.white_rooks_list[pos.nWhiteRooks] = to;
                pos.white_list_board[to] = pos.nWhiteRooks++;
                break;
            case WB:
                pos.white_bishops_list[pos.nWhiteBishops] = to;
                pos.white_list_board[to] = pos.nWhiteBishops++;
                break;
            case WN:
                pos.white_knights_list[pos.nWhiteKnights] = to;
                pos.white_list_board[to] = pos.nWhiteKnights++;
                break;
            default:
                break;
            }
        }

        break;
    }

    case WR: {
        int i = pos.white_list_board[from];
        pos.white_rooks_list[i] = to;
        pos.white_list_board[to] = i;
        pos.white_list_board[from] = -1;
        break;
    }

    case WN: {
        int i = pos.white_list_board[from];
        pos.white_knights_list[i] = to;
        pos.white_list_board[to] = i;
        pos.white_list_board[from] = -1;
        break;
    }

    case WB: {
        int i = pos.white_list_board[from];
        pos.white_bishops_list[i] = to;
        pos.white_list_board[to] = i;
        pos.white_list_board[from] = -1;
        break;
    }

    case WQ: {
        int i = pos.white_list_board[from];
        pos.white_queens_list[i] = to;
        pos.white_list_board[to] = i;
        pos.white_list_board[from] = -1;
        break;
    }

    case WK: {
        int i = pos.white_list_board[from];
        pos.white_kings_list[i] = to;
        pos.white_list_board[to] = i;
        pos.white_list_board[from] = -1;
        break;
    }

    case BP: {
        int i = pos.black_list_board[from];

        if (promotion == EMP) {
            pos.black_pawns_list[i] = to;
            pos.black_list_board[to] = i;
            pos.black_list_board[from] = -1;
        } else {
            square = pos.black_pawns_list[pos.nBlackPawns - 1];
            pos.nBlackPawns--;
            pos.black_pawns_list[i] = square;
            pos.black_list_board[square] = i;
            pos.black_list_board[from] = -1;

            switch (promotion) {
            case BQ:
                pos.black_queens_list[pos.nBlackQueens] = to;
                pos.black_list_board[to] = pos.nBlackQueens++;
                break;
            case BR:
                pos.black_rooks_list[pos.nBlackRooks] = to;
                pos.black_list_board[to] = pos.nBlackRooks++;
                break;
            case BB:
                pos.black_bishops_list[pos.nBlackBishops] = to;
                pos.black_list_board[to] = pos.nBlackBishops++;
                break;
            case BN:
                pos.black_knights_list[pos.nBlackKnights] = to;
                pos.black_list_board[to] = pos.nBlackKnights++;
                break;
            default:
                break;
            }
        }

        break;
    }

    case BR: {
        int i = pos.black_list_board[from];
        pos.black_rooks_list[i] = to;
        pos.black_list_board[to] = i;
        pos.black_list_board[from] = -1;
        break;
    }

    case BN: {
        int i = pos.black_list_board[from];
        pos.black_knights_list[i] = to;
        pos.black_list_board[to] = i;
        pos.black_list_board[from] = -1;
        break;
    }

    case BB: {
        int i = pos.black_list_board[from];
        pos.black_bishops_list[i] = to;
        pos.black_list_board[to] = i;
        pos.black_list_board[from] = -1;
        break;
    }

    case BQ: {
        int i = pos.black_list_board[from];
        pos.black_queens_list[i] = to;
        pos.black_list_board[to] = i;
        pos.black_list_board[from] = -1;
        break;
    }

    case BK: {
        int i = pos.black_list_board[from];
        pos.black_kings_list[i] = to;
        pos.black_list_board[to] = i;
        pos.black_list_board[from] = -1;
        break;
    }
 }

    Piece placedPiece = moving;

    if (promotion != EMP)
        placedPiece = promotion;

    pos.board[to] = placedPiece;
    pos.board[from] = EMP;


    pos.enPassantSquare = -1;

    if (moving == WP && to == from - 16)

        pos.enPassantSquare = from - 8;

    if (moving == BP && to == from + 16)

        pos.enPassantSquare = from + 8;


        bool castlingWhiteKingsidePossible = (moving == WK && from == 60 && to == 62);
        bool castlingBlackKingsidePossible = (moving == BK && from == 4 && to == 6);
        
        bool castlingWhiteQueensidePossible = (moving == WK && from == 60 && to == 58);
        bool castlingBlackQueensidePossible = (moving == BK && from == 4 && to == 2);

        if (castlingWhiteKingsidePossible == true && pos.hasWhiteKingMoved == false) {
            int rookIndex = pos.white_list_board[63];

            pos.white_rooks_list[rookIndex] = 61;
            pos.white_list_board[61] = rookIndex;
            pos.white_list_board[63] = -1;

            pos.board[61] = WR;
            pos.board[63] = EMP;
        }

        if (castlingBlackKingsidePossible == true && pos.hasBlackKingMoved == false) {
            int rookIndex = pos.black_list_board[7];

            pos.black_rooks_list[rookIndex] = 5;
            pos.black_list_board[5] = rookIndex;
            pos.black_list_board[7] = -1;

            pos.board[5] = BR;
            pos.board[7] = EMP;
        }

        if (castlingWhiteQueensidePossible == true && pos.hasWhiteKingMoved == false) {
            int rookIndex = pos.white_list_board[56];

            pos.white_rooks_list[rookIndex] = 59;
            pos.white_list_board[59] = rookIndex;
            pos.white_list_board[56] = -1;

            pos.board[59] = WR;
            pos.board[56] = EMP;
        }

        if (castlingBlackQueensidePossible == true && pos.hasBlackKingMoved == false) {
            int rookIndex = pos.black_list_board[0];

            pos.black_rooks_list[rookIndex] = 3;
            pos.black_list_board[3] = rookIndex;
            pos.black_list_board[0] = -1;

            pos.board[3] = BR;
            pos.board[0] = EMP;
        }


        if (moving == WK) {
            pos.hasWhiteKingMoved = true;
        }
        if (moving == BK) {
            pos.hasBlackKingMoved = true;
        }


        // CASTLING CHECK
        // A rook moving off its starting square forfeits castling on that side,
        // even if the king itself never moves.
        if (moving == WR && from == 63)
            pos.hasWhiteKingsideRookMoved = true;

        if (moving == WR && from == 56)
            pos.hasWhiteQueensideRookMoved = true;

        if (moving == BR && from == 7)
            pos.hasBlackKingsideRookMoved = true;

        if (moving == BR && from == 0)
            pos.hasBlackQueensideRookMoved = true;


        pos.whiteToMove = !pos.whiteToMove;
}


void unmakeMove(Move& move, boardState& pos) {

    int from = move.from;
    int to = move.to;
    Piece captured = move.captured;
    Piece promotion = move.promotion;
    

    Piece moving = pos.board[to];

    bool undoWhiteKingside  = (moving == WK && from == 60 && to == 62);
    bool undoWhiteQueenside = (moving == WK && from == 60 && to == 58);
    bool undoBlackKingside  = (moving == BK && from == 4  && to == 6);
    bool undoBlackQueenside = (moving == BK && from == 4  && to == 2);

    if (undoWhiteKingside) {
        int rookIndex = pos.white_list_board[61];
        pos.white_rooks_list[rookIndex] = 63;
        pos.white_list_board[63] = rookIndex;
        pos.white_list_board[61] = -1;
        pos.board[63] = WR;
        pos.board[61] = EMP;
    }

    if (undoWhiteQueenside) {
        int rookIndex = pos.white_list_board[59];
        pos.white_rooks_list[rookIndex] = 56;
        pos.white_list_board[56] = rookIndex;
        pos.white_list_board[59] = -1;
        pos.board[56] = WR;
        pos.board[59] = EMP;
    }

    if (undoBlackKingside) {
        int rookIndex = pos.black_list_board[5];
        pos.black_rooks_list[rookIndex] = 7;
        pos.black_list_board[7] = rookIndex;
        pos.black_list_board[5] = -1;
        pos.board[7] = BR;
        pos.board[5] = EMP;
    }

    if (undoBlackQueenside) {
        int rookIndex = pos.black_list_board[3];
        pos.black_rooks_list[rookIndex] = 0;
        pos.black_list_board[0] = rookIndex;
        pos.black_list_board[3] = -1;
        pos.board[0] = BR;
        pos.board[3] = EMP;
    }

    if (promotion != EMP) {

        switch (promotion) {

        case WQ: {
            pos.nWhiteQueens--;
            int i = pos.white_list_board[to];
            pos.white_queens_list[i] = pos.white_queens_list[pos.nWhiteQueens];
            pos.white_list_board[pos.white_queens_list[i]] = i;
            pos.white_list_board[to] = -1;
            break;
        }

        case WR: {
            pos.nWhiteRooks--;
            int i = pos.white_list_board[to];
            pos.white_rooks_list[i] = pos.white_rooks_list[pos.nWhiteRooks];
            pos.white_list_board[pos.white_rooks_list[i]] = i;
            pos.white_list_board[to] = -1;
            break;
        }

        case WB: {
            pos.nWhiteBishops--;
            int i = pos.white_list_board[to];
            pos.white_bishops_list[i] = pos.white_bishops_list[pos.nWhiteBishops];
            pos.white_list_board[pos.white_bishops_list[i]] = i;
            pos.white_list_board[to] = -1;
            break;
        }

        case WN: {
            pos.nWhiteKnights--;
            int i = pos.white_list_board[to];
            pos.white_knights_list[i] = pos.white_knights_list[pos.nWhiteKnights];
            pos.white_list_board[pos.white_knights_list[i]] = i;
            pos.white_list_board[to] = -1;
            break;
        }

        case BQ: {
            pos.nBlackQueens--;
            int i = pos.black_list_board[to];
            pos.black_queens_list[i] = pos.black_queens_list[pos.nBlackQueens];
            pos.black_list_board[pos.black_queens_list[i]] = i;
            pos.black_list_board[to] = -1;
            break;
        }

        case BR: {
            pos.nBlackRooks--;
            int i = pos.black_list_board[to];
            pos.black_rooks_list[i] = pos.black_rooks_list[pos.nBlackRooks];
            pos.black_list_board[pos.black_rooks_list[i]] = i;
            pos.black_list_board[to] = -1;
            break;
        }

        case BB: {
            pos.nBlackBishops--;
            int i = pos.black_list_board[to];
            pos.black_bishops_list[i] = pos.black_bishops_list[pos.nBlackBishops];
            pos.black_list_board[pos.black_bishops_list[i]] = i;
            pos.black_list_board[to] = -1;
            break;
        }

        case BN: {
            pos.nBlackKnights--;
            int i = pos.black_list_board[to];
            pos.black_knights_list[i] = pos.black_knights_list[pos.nBlackKnights];
            pos.black_list_board[pos.black_knights_list[i]] = i;
            pos.black_list_board[to] = -1;
            break;
        }

        default:
            break;
        }

        if (promotion == WQ || promotion == WR || promotion == WB || promotion == WN)
            moving = WP;

        if (promotion == BQ || promotion == BR || promotion == BB || promotion == BN)
            moving = BP;


        if (moving == WP) {
            pos.white_pawns_list[pos.nWhitePawns] = to;
            pos.white_list_board[to] = pos.nWhitePawns;
            pos.nWhitePawns++;
        }

        if (moving == BP) {
            pos.black_pawns_list[pos.nBlackPawns] = to;
            pos.black_list_board[to] = pos.nBlackPawns;
            pos.nBlackPawns++;
        }
    }


    int restoreSquare = to;

    if (move.isEnPassant)
    {
        if (moving == WP)
            restoreSquare = to + 8;
        else
            restoreSquare = to - 8;
    }

    if (captured != EMP) {
        switch (captured) {

        case WP: {
            int lastIdx = pos.nWhitePawns;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.white_pawns_list[move.capturedIndex];
                pos.white_pawns_list[lastIdx] = displacedSquare;
                pos.white_list_board[displacedSquare] = lastIdx;
            }

            pos.white_pawns_list[move.capturedIndex] = restoreSquare;
            pos.white_list_board[restoreSquare] = move.capturedIndex;

            pos.nWhitePawns++;
            break;
        }

        case WR: {
            int lastIdx = pos.nWhiteRooks;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.white_rooks_list[move.capturedIndex];
                pos.white_rooks_list[lastIdx] = displacedSquare;
                pos.white_list_board[displacedSquare] = lastIdx;
            }

            pos.white_rooks_list[move.capturedIndex] = to;
            pos.white_list_board[to] = move.capturedIndex;

            pos.nWhiteRooks++;
            break;
        }

        case WN: {
            int lastIdx = pos.nWhiteKnights;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.white_knights_list[move.capturedIndex];
                pos.white_knights_list[lastIdx] = displacedSquare;
                pos.white_list_board[displacedSquare] = lastIdx;
            }

            pos.white_knights_list[move.capturedIndex] = to;
            pos.white_list_board[to] = move.capturedIndex;

            pos.nWhiteKnights++;
            break;
        }

        case WB: {
            int lastIdx = pos.nWhiteBishops;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.white_bishops_list[move.capturedIndex];
                pos.white_bishops_list[lastIdx] = displacedSquare;
                pos.white_list_board[displacedSquare] = lastIdx;
            }

            pos.white_bishops_list[move.capturedIndex] = to;
            pos.white_list_board[to] = move.capturedIndex;

            pos.nWhiteBishops++;
            break;
        }

        case WQ: {
            int lastIdx = pos.nWhiteQueens;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.white_queens_list[move.capturedIndex];
                pos.white_queens_list[lastIdx] = displacedSquare;
                pos.white_list_board[displacedSquare] = lastIdx;
            }

            pos.white_queens_list[move.capturedIndex] = to;
            pos.white_list_board[to] = move.capturedIndex;

            pos.nWhiteQueens++;
            break;
        }

        case WK: {
            int lastIdx = pos.nWhiteKings;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.white_kings_list[move.capturedIndex];
                pos.white_kings_list[lastIdx] = displacedSquare;
                pos.white_list_board[displacedSquare] = lastIdx;
            }

            pos.white_kings_list[move.capturedIndex] = to;
            pos.white_list_board[to] = move.capturedIndex;

            pos.nWhiteKings++;
            break;
        }

        case BP: {
            int lastIdx = pos.nBlackPawns;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.black_pawns_list[move.capturedIndex];
                pos.black_pawns_list[lastIdx] = displacedSquare;
                pos.black_list_board[displacedSquare] = lastIdx;
            }

            pos.black_pawns_list[move.capturedIndex] = restoreSquare;
            pos.black_list_board[restoreSquare] = move.capturedIndex;

            pos.nBlackPawns++;
            break;
        }

        case BR: {
            int lastIdx = pos.nBlackRooks;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.black_rooks_list[move.capturedIndex];
                pos.black_rooks_list[lastIdx] = displacedSquare;
                pos.black_list_board[displacedSquare] = lastIdx;
            }

            pos.black_rooks_list[move.capturedIndex] = to;
            pos.black_list_board[to] = move.capturedIndex;

            pos.nBlackRooks++;
            break;
        }

        case BN: {
            int lastIdx = pos.nBlackKnights;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.black_knights_list[move.capturedIndex];
                pos.black_knights_list[lastIdx] = displacedSquare;
                pos.black_list_board[displacedSquare] = lastIdx;
            }

            pos.black_knights_list[move.capturedIndex] = to;
            pos.black_list_board[to] = move.capturedIndex;

            pos.nBlackKnights++;
            break;
        }

        case BB: {
            int lastIdx = pos.nBlackBishops;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.black_bishops_list[move.capturedIndex];
                pos.black_bishops_list[lastIdx] = displacedSquare;
                pos.black_list_board[displacedSquare] = lastIdx;
            }

            pos.black_bishops_list[move.capturedIndex] = to;
            pos.black_list_board[to] = move.capturedIndex;

            pos.nBlackBishops++;
            break;
        }

        case BQ: {
            int lastIdx = pos.nBlackQueens;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.black_queens_list[move.capturedIndex];
                pos.black_queens_list[lastIdx] = displacedSquare;
                pos.black_list_board[displacedSquare] = lastIdx;
            }

            pos.black_queens_list[move.capturedIndex] = to;
            pos.black_list_board[to] = move.capturedIndex;

            pos.nBlackQueens++;
            break;
        }

        case BK: {
            int lastIdx = pos.nBlackKings;

            if (move.capturedIndex != lastIdx) {
                int displacedSquare = pos.black_kings_list[move.capturedIndex];
                pos.black_kings_list[lastIdx] = displacedSquare;
                pos.black_list_board[displacedSquare] = lastIdx;
            }

            pos.black_kings_list[move.capturedIndex] = to;
            pos.black_list_board[to] = move.capturedIndex;

            pos.nBlackKings++;
            break;
        }

}

}

    switch(moving) {

    case WP: {
        int i = pos.white_list_board[to];
        pos.white_pawns_list[i] = from;
        pos.white_list_board[from] = i;
        pos.white_list_board[to] = -1;
        break;
    }

    case WR: {
        int i = pos.white_list_board[to];
        pos.white_rooks_list[i] = from;
        pos.white_list_board[from] = i;
        pos.white_list_board[to] = -1;
        break;
    }

    case WN: {
        int i = pos.white_list_board[to];
        pos.white_knights_list[i] = from;
        pos.white_list_board[from] = i;
        pos.white_list_board[to] = -1;
        break;
        }

    case WB: {
        int i = pos.white_list_board[to];
        pos.white_bishops_list[i] = from;
        pos.white_list_board[from] = i;
        pos.white_list_board[to] = -1;
        break;
    }

    case WQ: {
        int i = pos.white_list_board[to];
        pos.white_queens_list[i] = from;
        pos.white_list_board[from] = i;
        pos.white_list_board[to] = -1;
        break;
    }

    case WK: {
        int i = pos.white_list_board[to];
        pos.white_kings_list[i] = from;
        pos.white_list_board[from] = i;
        pos.white_list_board[to] = -1;
        break;
    }

    case BP: {
        int i = pos.black_list_board[to];
        pos.black_pawns_list[i] = from;
        pos.black_list_board[from] = i;
        pos.black_list_board[to] = -1;
        break;
    }

    case BR: {
        int i = pos.black_list_board[to];
        pos.black_rooks_list[i] = from;
        pos.black_list_board[from] = i;
        pos.black_list_board[to] = -1;
        break;
    }

    case BN: {
        int i = pos.black_list_board[to];
        pos.black_knights_list[i] = from;
        pos.black_list_board[from] = i;
        pos.black_list_board[to] = -1;
        break;
    }

    case BB: {
        int i = pos.black_list_board[to];
        pos.black_bishops_list[i] = from;
        pos.black_list_board[from] = i;
        pos.black_list_board[to] = -1;
        break;
    }

    case BQ: {
        int i = pos.black_list_board[to];
        pos.black_queens_list[i] = from;
        pos.black_list_board[from] = i;
        pos.black_list_board[to] = -1;
        break;
    }

    case BK: {
        int i = pos.black_list_board[to];
        pos.black_kings_list[i] = from;
        pos.black_list_board[from] = i;
        pos.black_list_board[to] = -1;
        break;
     }
    }


    pos.board[from] = moving;

    if (restoreSquare == to) {
        pos.board[to] = captured;
    } else {
        pos.board[to] = EMP;
        pos.board[restoreSquare] = captured;
    }

    pos.enPassantSquare = move.oldEnPassant;


    pos.hasWhiteKingMoved = move.oldWhiteKingMoved;
    pos.hasBlackKingMoved = move.oldBlackKingMoved;
    pos.hasWhiteKingsideRookMoved = move.oldWhiteKingsideRookMoved;
    pos.hasWhiteQueensideRookMoved = move.oldWhiteQueensideRookMoved;
    pos.hasBlackKingsideRookMoved = move.oldBlackKingsideRookMoved;
    pos.hasBlackQueensideRookMoved = move.oldBlackQueensideRookMoved;

    pos.whiteToMove = move.oldWhiteToMove;

}
