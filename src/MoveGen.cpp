#include "MoveGen.h"
#include "Attacks.h"
#include <cstdlib> // for abs()

Move movelist[256];
int moveCount = 0;


void generateMove(boardState& pos) {

    moveCount = 0;

    const int knightDirections[8] { 
        6, 15, 17, 10,
        -6, -15, -17, -10
    };

    const int rookDirections[4] {
        -8, 8, -1, 1
    };

    const int bishopDirections[4] {
        -9, -7, 9, 7
    };

    const int queenDirections[8] {
        -8, 8, -1, 1,
        -9, -7, 9, 7
    };

    const int kingDirections[8] = {
        -9, -8, -7, -1,
        1, 7, 8, 9
    };

    if (pos.whiteToMove) {

    // white knights
    for (int i = 0; i < pos.nWhiteKnights; i++) {
        int from = pos.white_knights_list[i];


        for (int direction : knightDirections) {
            int to = from + direction;

            if (!(0 <= to && to < 64)) 
            continue;
            
                int fromRow = from / 8; 
                int fromCol = from % 8;
                int toRow = to / 8;
                int toCol = to % 8;

                int rowDiff = abs(fromRow - toRow);
                int colDiff = abs(fromCol - toCol);

                if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)))
                continue;
                
                Piece target = pos.board[to];

                if (target >= WP && target <= WK)
                continue;

            movelist[moveCount++] = {from, to, pos.board[to]};
        }
    }

    // white rooks
    for (int i = 0; i < pos.nWhiteRooks; i++) {
        int from = pos.white_rooks_list[i];

        for (int direction : rookDirections) {
            int to = from + direction;
            
            while (0 <= to && to < 64) {

                if((direction == 1 || direction == -1) && to / 8 != from / 8)
                break;

            Piece target = pos.board[to];

            if (target == EMP) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                to += direction;
                continue;
            }

            if (target >= BP && target <= BK) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                break;
            }
            
            break;
        }
    }
}

    // white bishop
    for (int i = 0; i < pos.nWhiteBishops; i++) {
        int from = pos.white_bishops_list[i];

        for (int direction : bishopDirections) {
            int to = from + direction;
            
            while (0 <= to && to < 64) {

                int prev = to - direction;

                if (abs((to % 8) - (prev % 8)) != 1)
                    break;

            Piece target = pos.board[to];

            if (target == EMP) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                to += direction;
                continue;
            }

            if (target >= BP && target <= BK) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                break;
            }
            
            break;
        }
     }
    }

    // white queen
    for (int i = 0; i < pos.nWhiteQueens; i++) {
        int from = pos.white_queens_list[i];

        for (int direction : queenDirections) {
            int to = from + direction;

            while(0 <= to && to < 64) {

            if((direction == 1 || direction == -1) && to / 8 != from / 8)
            break;


            if (direction == -9 || direction == 9 || direction == -7 || direction == 7) {

            int prev = to - direction;

            if (abs((to % 8) - (prev % 8)) != 1)
                    break;
            }

            Piece target = pos.board[to];

            if (target == EMP) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                to += direction;
                continue;
            }

            if (target >= BP && target <= BK) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                break;
            }

                break;
            }
        }
    }

    // white king
    for (int i = 0; i < pos.nWhiteKings; i++) {
        int from = pos.white_kings_list[i];

        for (int direction : kingDirections) {
            int to = from + direction;

            if (!(0 <= to && to < 64)) 
            continue;

            int fromRow = from / 8;
            int fromCol = from % 8;
            int toRow = to / 8;
            int toCol = to % 8;


            if (abs(fromRow - toRow) > 1 || abs(fromCol - toCol) > 1)
            continue;

            Piece target = pos.board[to];

                if (target >= WP && target <= WK)
                continue;

            movelist[moveCount++] = {from, to, pos.board[to]};
        }

    }

    // white pawns
    for (int i = 0; i < pos.nWhitePawns; i++) {
    int from = pos.white_pawns_list[i];

    int oneForward = from - 8;

    if (0 <= oneForward && oneForward < 64 && pos.board[oneForward] == EMP) {

        if (oneForward / 8 == 0) {
            for (Piece promo : {WQ, WR, WB, WN}) {
                Move m{from, oneForward};
                m.promotion = promo;
                movelist[moveCount++] = m;
            }
        } else {
            movelist[moveCount++] = {from, oneForward, EMP};
        }
    }


    int twoForward = from - 16;

    if (from / 8 == 6 && pos.board[oneForward] == EMP && pos.board[twoForward] == EMP) {
        movelist[moveCount++] = {from, twoForward, EMP};
    }

  
    int captures[2] = {-9, -7};

    for (int direction : captures) {
        int to = from + direction;

        if (!(0 <= to && to < 64))
        continue;

        int prev = to - direction;


    if (abs((to % 8) - (prev % 8)) != 1)
        continue;

        Piece target = pos.board[to];

    if (target >= BP && target <= BK) {

        if (to / 8 == 0) {
            for (Piece promo : {WQ, WR, WB, WN}) {
                Move m{from, to, target};
                m.promotion = promo;
                movelist[moveCount++] = m;
            }
        } else {
            movelist[moveCount++] = {from, to, target};
        }
    }

    if (to == pos.enPassantSquare && target == EMP && pos.board[to + 8] == BP) {
         movelist[moveCount++] = {from, to, BP};
    }

    }
}

        
// ROOK SIDE
    if (pos.hasWhiteKingMoved == false && pos.hasWhiteKingsideRookMoved == false && pos.board[61] == EMP && pos.board[62] == EMP
        && !isSquareAttacked(60, false, pos) && !isSquareAttacked(61, false, pos) && !isSquareAttacked(62, false, pos) && pos.board[63] == WR) {
        movelist[moveCount++] = {60, 62, EMP};
    }

    // QUEENSIDE
        if (pos.hasWhiteKingMoved == false && pos.hasWhiteQueensideRookMoved == false && pos.board[57] == EMP && pos.board[58] == EMP && pos.board[59] == EMP
    && !isSquareAttacked(60, false, pos) && !isSquareAttacked(59, false, pos) && !isSquareAttacked(58, false, pos) && pos.board[56] == WR) {
        movelist[moveCount++] = {60, 58, EMP};
    }

    } else {

    // black knight
    for (int i = 0; i < pos.nBlackKnights; i++) {
        int from = pos.black_knights_list[i];

        for (int direction : knightDirections) {
            int to = from + direction;

            if (!(0 <= to && to < 64))
                continue;

            int fromRow = from / 8;
            int fromCol = from % 8;
            int toRow = to / 8;
            int toCol = to % 8;

            int rowDiff = abs(fromRow - toRow);
            int colDiff = abs(fromCol - toCol);

            if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)))
                continue;

            Piece target = pos.board[to];

            if (target >= BP && target <= BK)
                continue;

            movelist[moveCount++] = {from, to, pos.board[to]};
        }
    }

    // black rook
    for (int i = 0; i < pos.nBlackRooks; i++) {
        int from = pos.black_rooks_list[i];

        for (int direction : rookDirections) {
            int to = from + direction;
            
            while (0 <= to && to < 64) {

                if((direction == 1 || direction == -1) && to / 8 != from / 8)
                break;

            Piece target = pos.board[to];

            if (target == EMP) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                to += direction;
                continue;
            }

            if (target >= WP && target <= WK) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                break;
            }
            
            break;
        }
    }
 }
    
    // black bishop
    for (int i = 0; i < pos.nBlackBishops; i++) {
        int from = pos.black_bishops_list[i];

        for (int direction : bishopDirections) {
            int to = from + direction;
            
            while (0 <= to && to < 64) {

                int prev = to - direction;

                if (abs((to % 8) - (prev % 8)) != 1)
                    break;

            Piece target = pos.board[to];

            if (target == EMP) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                to += direction;
                continue;
            }

            if (target >= WP && target <= WK) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                break;
            }
            
            break;
        }
     }
    }

    // black queen
    for (int i = 0; i < pos.nBlackQueens; i++) {
        int from = pos.black_queens_list[i];

        for (int direction : queenDirections) {
            int to = from + direction;

            while(0 <= to && to < 64) {

            if((direction == 1 || direction == -1) && to / 8 != from / 8)
            break;


            if (direction == -9 || direction == 9 || direction == -7 || direction == 7) {

            int prev = to - direction;

            if (abs((to % 8) - (prev % 8)) != 1)
                    break;
            }

            Piece target = pos.board[to];

            if (target == EMP) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                to += direction;
                continue;
            }

            if (target >= WP && target <= WK) {
                movelist[moveCount++] = {from, to, pos.board[to]};
                break;
            }

                break;
            }
        }
    }

    // black king
    for (int i = 0; i < pos.nBlackKings; i++) {
        int from = pos.black_kings_list[i];

        for (int direction : kingDirections) {
            int to = from + direction;

            if (!(0 <= to && to < 64)) 
            continue;

            int fromRow = from / 8;
            int fromCol = from % 8;
            int toRow = to / 8;
            int toCol = to % 8;

            if (abs(fromRow - toRow) > 1 || abs(fromCol - toCol) > 1)
            continue;

            Piece target = pos.board[to];

                if (target >= BP && target <= BK)
                continue;

            movelist[moveCount++] = {from, to, pos.board[to]};
        }

    }

    // black pawn
    for (int i = 0; i < pos.nBlackPawns; i++) {
    int from = pos.black_pawns_list[i];

    int oneForward = from + 8;

    if (0 <= oneForward && oneForward < 64 && pos.board[oneForward] == EMP) {

        if (oneForward / 8 == 7) {
            for (Piece promo : {BQ, BR, BB, BN}) {
                Move m{from, oneForward};
                m.promotion = promo;
                movelist[moveCount++] = m;
            }
        } else {
            movelist[moveCount++] = {from, oneForward, EMP};
        }
    }

    int twoForward = from + 16;

    if (from / 8 == 1 && pos.board[oneForward] == EMP && pos.board[twoForward] == EMP) {
        movelist[moveCount++] = {from, twoForward, EMP};
    }

    int captures[2] = {7, 9};

    for (int direction : captures) {
        int to = from + direction;

        if (!(0 <= to && to < 64))
        continue;

        int prev = to - direction;

    if (abs((to % 8) - (prev % 8)) != 1)
        continue;

        Piece target = pos.board[to];

    if (target >= WP && target <= WK) {

        if (to / 8 == 7) {
            for (Piece promo : {BQ, BR, BB, BN}) {
                Move m{from, to, target};
                m.promotion = promo;
                movelist[moveCount++] = m;
            }
        } else {
            movelist[moveCount++] = {from, to, target};
        }
    }

    if (to == pos.enPassantSquare && target == EMP && pos.board[to - 8] == WP) {
         movelist[moveCount++] = {from, to, WP};
    }

    }
}

        // rook side
    if (pos.hasBlackKingMoved == false && pos.hasBlackKingsideRookMoved == false && pos.board[5] == EMP && pos.board[6] == EMP
        && !isSquareAttacked(4, true, pos) && !isSquareAttacked(5, true, pos) && !isSquareAttacked(6, true, pos) && pos.board[7] == BR) {
        movelist[moveCount++] = {4, 6, EMP};
    }
    // queen side
    if (pos.hasBlackKingMoved == false && pos.hasBlackQueensideRookMoved == false && pos.board[1] == EMP && pos.board[2] == EMP && pos.board[3] == EMP
    && !isSquareAttacked(4, true, pos) && !isSquareAttacked(3, true, pos) && !isSquareAttacked(2, true, pos) && pos.board[0] == BR) {
        movelist[moveCount++] = {4, 2, EMP};
    }

    }

}
