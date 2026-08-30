#include "MoveGen.h"
#include "Evaluation.h"
#include "MoveMake.h"
#include "Attacks.h"
#include "Zobrist.h"
#include "TranspositionTable.h"
#include "TimeManager.h"
#include <algorithm>
#include <cstdint>
#include <random>

static std::mt19937_64 rng(std::random_device{}());

const int oo = 1000000;
const int MATE = 999000;

std::vector<uint64_t> gameHistory;

void resetGameHistory() {
    gameHistory.clear();
}

static bool isRepetitionDraw() {
    if (gameHistory.empty())
        return false;

    uint64_t current = gameHistory.back();
    int occurrences = 0;

    for (size_t i = 0; i + 1 < gameHistory.size(); i++) {
        if (gameHistory[i] == current) {
            occurrences++;
            if (occurrences >= 2)
                return true;
        }
    }

    return false;
}

int pieceValue(int piece)
{
    switch (piece)
    {
    case WP: case BP: return 100;
    case WN: case BN: return 320;
    case WB: case BB: return 330;
    case WR: case BR: return 500;
    case WQ: case BQ: return 900;
    case WK: case BK: return 20000;
    default: return 0;
    }
}

int moveOrderScore(const Move& move, const boardState& pos)
{
    int score = 0;

    if (move.captured != EMP || move.isEnPassant)
    {
        int victim = move.isEnPassant ? BP : move.captured;
        int attacker = pos.board[move.from];

        score += 100000;
        score += pieceValue(victim) * 10;
        score -= pieceValue(attacker);
    }

    if (move.promotion != EMP)
        score += 80000 + pieceValue(move.promotion);

    return score;
}


static bool isSameMove(const Move& a, const Move& b)
{
    return a.from == b.from && a.to == b.to && a.promotion == b.promotion;
}

int quiescence(boardState& pos, int alpha, int beta) {

    if (isSearchTimeUp())
        throw SearchTimeout{};

    int standPat = evaluate(pos);

    if (standPat >= beta)
        return beta;

    if (standPat > alpha)
        alpha = standPat;

    generateLegalMoves(pos);

    Move moves[256];
    int count = moveCount;

    for (int i = 0; i < count; i++)
        moves[i] = movelist[i];

    std::sort(moves, moves + count,
    [&](const Move& a, const Move& b)
    {
        return moveOrderScore(a, pos) > moveOrderScore(b, pos);
    });

    for (int i = 0; i < count; i++) {

        Move move = moves[i];

        if (move.captured == EMP &&
            !move.isEnPassant &&
            move.promotion == EMP)
            continue;

        makeMove(move, pos);

        int score = -quiescence(pos, -beta, -alpha);

        unmakeMove(move, pos);

        if (score >= beta)
            return beta;

        if (score > alpha)
            alpha = score;
    }

    return alpha;
}

int negaMax(boardState& pos, int depth, int alpha, int beta, int ply);

Move searchBestMove(boardState& pos, int depth, int alpha, int beta, int* outScore) {
    generateLegalMoves(pos);

    Move moves[256];
    int count = moveCount;

        if (count == 0)
            return Move();

    for (int i = 0; i < count; i++)
        moves[i] = movelist[i];

    uint64_t rootKey = zobristHash(pos);
    Move ttMove = getTTMove(rootKey);

    std::sort(moves, moves + count,
    [&](const Move& a, const Move& b)
    {
        int scoreA = moveOrderScore(a, pos) + (isSameMove(a, ttMove) ? 1000000 : 0);
        int scoreB = moveOrderScore(b, pos) + (isSameMove(b, ttMove) ? 1000000 : 0);
        return scoreA > scoreB;
    });

    constexpr int VARIETY_MARGIN_CP = 15; // 15 centipawns

    int scores[256];
    int bestScore = -oo;

    for (int i = 0; i < count; i++) {
        makeMove(moves[i], pos);
        gameHistory.push_back(zobristHash(pos));

        scores[i] = -negaMax(pos, depth - 1, -beta, -alpha, 1);

        gameHistory.pop_back();
        unmakeMove(moves[i], pos);

        if (scores[i] > bestScore)
            bestScore = scores[i];
    }


    int varietyMargin = VARIETY_MARGIN_CP;
    if (bestScore >= 900000 || bestScore <= -900000)
        varietyMargin = 0;
    Move bestMove = moves[0];
    int tieCount = 0;

    for (int i = 0; i < count; i++) {
        if (scores[i] >= bestScore - varietyMargin) {
            tieCount++;
            std::uniform_int_distribution<int> dist(1, tieCount);
            if (dist(rng) == tieCount)
                bestMove = moves[i];
        }
    }

    storeTT(rootKey, depth, bestScore, 0, TT_EXACT, bestMove);

    if (outScore)
        *outScore = bestScore;

    return bestMove;
}

Move iterativeDeepening(boardState& pos, int maxDepth, long long timeLimitMs) {

    generateLegalMoves(pos);

       if (moveCount == 0)
        return Move{-1, -1};

        Move bestMove = movelist[0];

    if (timeLimitMs > 0)
        startSearchTimer(timeLimitMs);
    else
        clearSearchTimer();

    constexpr int ASPIRATION_WINDOW = 50;
    int prevScore = 0;

    for (int depth = 1; depth <= maxDepth; depth++) {
        try {
            int alpha = (depth <= 1) ? -oo : prevScore - ASPIRATION_WINDOW;
            int beta  = (depth <= 1) ?  oo : prevScore + ASPIRATION_WINDOW;
            int score = 0;
            Move result;

            int failCount = 0;
            constexpr int MAX_ASPIRATION_FAILS = 4;

            while (true) {
                result = searchBestMove(pos, depth, alpha, beta, &score);

                if (score <= alpha) {
                    failCount++;
                    if (failCount >= MAX_ASPIRATION_FAILS) {
                        alpha = -oo;
                        beta = oo;
                    } else {
                        alpha = std::max(alpha - ASPIRATION_WINDOW * (1 << failCount), -oo);
                    }
                }
                else if (score >= beta) {
                    failCount++;
                    if (failCount >= MAX_ASPIRATION_FAILS) {
                        alpha = -oo;
                        beta = oo;
                    } else {
                        beta = std::min(beta + ASPIRATION_WINDOW * (1 << failCount), oo);
                    }
                }
                else {
                    break;
                }
            }

            bestMove = result;
            prevScore = score;
        }
        catch (const SearchTimeout&) {
            break;
        }
    }

    return bestMove;
}

static bool inCheckNow(boardState& pos) {
    int kingSquare = pos.whiteToMove ? pos.white_kings_list[0] : pos.black_kings_list[0];
    return isSquareAttacked(kingSquare, !pos.whiteToMove, pos);
}

static bool hasNonPawnMaterial(boardState& pos) {
    if (pos.whiteToMove)
        return pos.nWhiteQueens + pos.nWhiteRooks + pos.nWhiteBishops + pos.nWhiteKnights > 0;
    else
        return pos.nBlackQueens + pos.nBlackRooks + pos.nBlackBishops + pos.nBlackKnights > 0;
}

int negaMax(boardState& pos, int depth, int alpha, int beta, int ply) {

    if (isSearchTimeUp())
        throw SearchTimeout{};

    if (isRepetitionDraw())
        return 0;

    if (depth == 0)
        return quiescence(pos, alpha, beta);

    int alphaOriginal = alpha;

    uint64_t hashKey = zobristHash(pos);

    Move ttMove{};
    int ttScore;

    if (probeTT(hashKey, depth, alpha, beta, ply, ttScore, ttMove)) {
        return ttScore;
    }

    const int R = 2;
    if (depth >= 3 && !inCheckNow(pos) && hasNonPawnMaterial(pos)) {
        int savedEP = pos.enPassantSquare;
        pos.enPassantSquare = -1;
        pos.whiteToMove = !pos.whiteToMove;

        int nullScore = -negaMax(pos, depth - 1 - R, -beta, -beta + 1, ply + 1);

        pos.whiteToMove = !pos.whiteToMove;
        pos.enPassantSquare = savedEP;

        if (nullScore >= beta)
            return beta;
    }

    generateLegalMoves(pos);

    Move moves[256];
    int count = moveCount;

    for (int i = 0; i < count; i++)
        moves[i] = movelist[i];

    std::sort(moves, moves + count,
    [&](const Move& a, const Move& b)
    {
        int scoreA = moveOrderScore(a, pos) + (isSameMove(a, ttMove) ? 1000000 : 0);
        int scoreB = moveOrderScore(b, pos) + (isSameMove(b, ttMove) ? 1000000 : 0);
        return scoreA > scoreB;
    });

    if (count == 0) {
        int kingSquare = -1;

        for (int sq = 0; sq < 64; sq++) {
            if ((pos.whiteToMove && pos.board[sq] == WK) ||
                (!pos.whiteToMove && pos.board[sq] == BK)) {
                kingSquare = sq;
                break;
            }
        }

        if (isSquareAttacked(kingSquare, !pos.whiteToMove, pos))
            return -MATE + ply;

        return 0;
    }

    int maxScore = -oo;
    Move bestMoveHere = moves[0];

    for (int i = 0; i < count; i++) {
        Move move = moves[i];

        makeMove(move, pos);
        gameHistory.push_back(zobristHash(pos));
        int score = -negaMax(pos, depth - 1, -beta, -alpha, ply + 1);
        gameHistory.pop_back();
        unmakeMove(move, pos);

        if (score > maxScore) {
            maxScore = score;
            bestMoveHere = move;
        }

        alpha = std::max(alpha, maxScore);

        if (alpha >= beta)
            break;
    }

    TTFlag flag;
    if (maxScore <= alphaOriginal)
        flag = TT_ALPHA;
    else if (maxScore >= beta)
        flag = TT_BETA;
    else
        flag = TT_EXACT;

    storeTT(hashKey, depth, maxScore, ply, flag, bestMoveHere);

    return maxScore;
}