#include <MoveGen.h>
#include <MoveMake.h>
#include <Attacks.h>


    void generateLegalMoves(boardState& pos) {
        generateMove(pos);

        Move legalMoves[256];
        int legalMoveCount = 0;

        for (int i = 0; i < moveCount; i++) {
            Move move = movelist[i];

            makeMove(move, pos);

            int kingSquare;
            bool inCheck;

            if (!pos.whiteToMove) {
                kingSquare = pos.white_kings_list[0];
                inCheck = isSquareAttacked(kingSquare, false, pos);
            } else {
                kingSquare = pos.black_kings_list[0];
                inCheck = isSquareAttacked(kingSquare, true, pos);
            }

            if (!inCheck) {
                legalMoves[legalMoveCount++] = move;
            }

            unmakeMove(move, pos);
        }

        for (int i = 0; i < legalMoveCount; i++) {
            movelist[i] = legalMoves[i];
        }

        moveCount = legalMoveCount;
    }
