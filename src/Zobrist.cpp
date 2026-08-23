#include "Zobrist.h"
#include "Piece.h"
#include <random>

static uint64_t pieceKeys[12][64];

static uint64_t sideToMoveKey;

static uint64_t castlingKeys[4];

static uint64_t enPassantFileKeys[8];

void initZobrist()
{
    std::mt19937_64 rng(0xC0FFEEULL);

    for (int piece = 0; piece < 12; piece++)
        for (int square = 0; square < 64; square++)
            pieceKeys[piece][square] = rng();

    sideToMoveKey = rng();

    for (int i = 0; i < 4; i++)
        castlingKeys[i] = rng();

    for (int file = 0; file < 8; file++)
        enPassantFileKeys[file] = rng();
}

uint64_t zobristHash(const boardState& pos)
{
    uint64_t hash = 0;

    for (int square = 0; square < 64; square++)
    {
        Piece piece = pos.board[square];
        if (piece != EMP)
            hash ^= pieceKeys[piece][square];
    }

    if (!pos.whiteToMove)
        hash ^= sideToMoveKey;

    if (!pos.hasWhiteKingMoved && !pos.hasWhiteKingsideRookMoved)  hash ^= castlingKeys[0];
    if (!pos.hasWhiteKingMoved && !pos.hasWhiteQueensideRookMoved) hash ^= castlingKeys[1];
    if (!pos.hasBlackKingMoved && !pos.hasBlackKingsideRookMoved)  hash ^= castlingKeys[2];
    if (!pos.hasBlackKingMoved && !pos.hasBlackQueensideRookMoved) hash ^= castlingKeys[3];

    if (pos.enPassantSquare != -1)
        hash ^= enPassantFileKeys[pos.enPassantSquare % 8];

    return hash;
}
