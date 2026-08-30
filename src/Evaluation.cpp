#include "Evaluation.h"
#include "BoardState.h"

#include <algorithm>
#include <cstdlib>

    

const int pawnTable[64] = {
     0,0,0,0,0,0,0,0,
    50,50,50,50,50,50,50,50,
    10,10,20,30,30,20,10,10,
     5,5,10,25,25,10,5,5,
     0,0,0,20,20,0,0,0,
     5,-5,-10,0,0,-10,-5,5,
     5,10,10,-20,-20,10,10,5,
     0,0,0,0,0,0,0,0
};

const int knightTable[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,0,0,0,0,-20,-40,
    -30,0,10,15,15,10,0,-30,
    -30,5,15,20,20,15,5,-30,
    -30,0,15,20,20,15,0,-30,
    -30,5,10,15,15,10,5,-30,
    -40,-20,0,5,5,0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

const int bishopTable[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,5,0,0,0,0,5,-10,
    -10,10,10,10,10,10,10,-10,
    -10,0,10,10,10,10,0,-10,
    -10,5,5,10,10,5,5,-10,
    -10,0,5,10,10,5,0,-10,
    -10,0,0,0,0,0,0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

const int rookTable[64] = {
     0,0,0,5,5,0,0,0,
    -5,0,0,0,0,0,0,-5,
    -5,0,0,0,0,0,0,-5,
    -5,0,0,0,0,0,0,-5,
    -5,0,0,0,0,0,0,-5,
    -5,0,0,0,0,0,0,-5,
     5,10,10,10,10,10,10,5,
     0,0,0,0,0,0,0,0
};

const int queenTable[64] = {
    -20,-10,-10,-5,-5,-10,-10,-20,
    -10,0,0,0,0,0,0,-10,
    -10,0,5,5,5,5,0,-10,
    -5,0,5,5,5,5,0,-5,
    0,0,5,5,5,5,0,-5,
    -10,5,5,5,5,5,0,-10,
    -10,0,5,0,0,0,0,-10,
    -20,-10,-10,-5,-5,-10,-10,-20
};

const int kingTable[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20,20,0,0,0,0,20,20,
     20,30,10,0,0,10,30,20
};


inline int mirror(int sq)
{
    return sq ^ 56; // flips the board vertically
} 

    inline int squareDistance(int a, int b) {
        int fileA = a % 8, rankA = a / 8;
        int fileB = b % 8, rankB = b / 8;
        return std::max(abs(fileA - fileB), abs(rankA - rankB));
    }

    inline int distanceToCenter(int sq) {
        int file = sq % 8, rank = sq / 8;
        int df = std::min(file, 7 - file);
        int dr = std::min(rank, 7 - rank);
        return std::min(df, dr);
    }

int evaluate(boardState& pos)
{
    int score = 0;

    for (int square = 0; square < 64; square++)
    {
        switch (pos.board[square])
        {
        case WP:
            score += 100 + pawnTable[square];
            break;

        case WN:
            score += 320 + knightTable[square];
            break;

        case WB:
            score += 330 + bishopTable[square];
            break;

        case WR:
            score += 500 + rookTable[square];
            break;

        case WQ:
            score += 900 + queenTable[square];
            break;

        case WK:
            score += kingTable[square];
            break;

        case BP:
            score -= 100 + pawnTable[mirror(square)];
            break;

        case BN:
            score -= 320 + knightTable[mirror(square)];
            break;

        case BB:
            score -= 330 + bishopTable[mirror(square)];
            break;

        case BR:
            score -= 500 + rookTable[mirror(square)];
            break;

        case BQ:
            score -= 900 + queenTable[mirror(square)];
            break;

        case BK:
            score -= kingTable[mirror(square)];
            break;
        }
    }


    int whiteMaterial = 0, blackMaterial = 0;
    int whiteKingSq = -1, blackKingSq = -1;

for (int sq = 0; sq < 64; sq++) {
    switch (pos.board[sq]) {
        case WQ: whiteMaterial += 900; break;
        case WR: whiteMaterial += 500; break;
        case WB: whiteMaterial += 330; break;
        case WN: whiteMaterial += 320; break;
        case WK: whiteKingSq = sq; break;

        case BQ: blackMaterial += 900; break;
        case BR: blackMaterial += 500; break;
        case BB: blackMaterial += 330; break;
        case BN: blackMaterial += 320; break;
        case BK: blackKingSq = sq; break;
    }
}

if (whiteMaterial >= blackMaterial + 500 && blackMaterial <= 500) {
    score += 4 * (3 - distanceToCenter(blackKingSq));
    score += 4 * (14 - squareDistance(whiteKingSq, blackKingSq));
}
else if (blackMaterial >= whiteMaterial + 500 && whiteMaterial <= 500) {
    score -= 4 * (3 - distanceToCenter(whiteKingSq));
    score -= 4 * (14 - squareDistance(whiteKingSq, blackKingSq));
}

return pos.whiteToMove ? score : -score;
}