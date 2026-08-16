#include "Perft.h"
#include "MoveGen.h"
#include "MoveMake.h"
#include <iostream>

long long perft(boardState& pos, int depth) {
    if (depth == 0)
        return 1;

    generateLegalMoves(pos);

    Move moves[256];
    int count = moveCount;

    for (int i = 0; i < count; i++)
        moves[i] = movelist[i];

    long long nodes = 0;

    for (int i = 0; i < count; i++) {
        Move move = moves[i];

        makeMove(move, pos);
        nodes += perft(pos, depth - 1);
        unmakeMove(move, pos);
    }

    return nodes;
}

void perftDivide(boardState& pos, int depth) {
    generateLegalMoves(pos);

    Move moves[256];
    int count = moveCount;

    for (int i = 0; i < count; i++)
        moves[i] = movelist[i];

    long long total = 0;

    for (int i = 0; i < count; i++) {
        Move move = moves[i];

        makeMove(move, pos);
        long long nodes = perft(pos, depth - 1);
        unmakeMove(move, pos);

        std::cout << move.from << " -> " << move.to << " : " << nodes << '\n';
        total += nodes;
    }

    std::cout << "Total nodes: " << total << '\n';
}