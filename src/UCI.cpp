#include "UCI.h"
#include "negaMax.h"
#include "BoardState.h"
#include "MoveGen.h"
#include "MoveMake.h"
#include "Move.h"
#include "Zobrist.h"
#include "TimeManager.h"

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <thread>
#include <atomic>

static std::thread searchThread;
static std::atomic<bool> searchRunning{false};

static void joinSearchThread() {
    if (searchThread.joinable())
        searchThread.join();
}

void uciLoop() {

    std::ios::sync_with_stdio(false);
    std::cout.setf(std::ios::unitbuf);

    boardState pos;
    initStartPosition(pos);

    std::string line;

    while (std::getline(std::cin, line)) {

        if (line == "uci") {
            std::cout << "id name MyEngine\n";
            std::cout << "id author Tonislav\n";
            std::cout << "uciok\n";
        }

        else if (line == "isready") {
            std::cout << "readyok\n";
        }

        else if (line == "ucinewgame") {
            initStartPosition(pos);
            resetGameHistory();
            gameHistory.push_back(zobristHash(pos));
        }

        else if (line.rfind("position", 0) == 0) {
            if (searchThread.joinable()) {
                requestSearchStop();
                joinSearchThread();
            }
            std::istringstream iss(line);
            std::string cmd, type;
            iss >> cmd >> type;

            if (type == "startpos") {
                initStartPosition(pos);
                resetGameHistory();
                gameHistory.push_back(zobristHash(pos));

                std::string movesWord;
                if (iss >> movesWord && movesWord == "moves") {
                    std::string moveStr;

                    while (iss >> moveStr) {
                        generateLegalMoves(pos);

                        for (int i = 0; i < moveCount; i++) {
                            if (moveToString(movelist[i]) == moveStr) {
                                makeMove(movelist[i], pos);
                                gameHistory.push_back(zobristHash(pos));
                                break;
                            }
                        }
                    }
                }
            }
            
            else if (type == "fen") {
                std::string fenStr;
                std::string token;
                bool sawMoves = false;

                while (iss >> token) {
                    if (token == "moves") {
                        sawMoves = true;
                        break;
                    }
                    if (!fenStr.empty())
                        fenStr += ' ';
                    fenStr += token;
                }

                loadFEN(pos, fenStr);
                resetGameHistory();
                gameHistory.push_back(zobristHash(pos));

                if (sawMoves) {
                    std::string moveStr;

                    while (iss >> moveStr) {
                        generateLegalMoves(pos);

                        for (int i = 0; i < moveCount; i++) {
                            if (moveToString(movelist[i]) == moveStr) {
                                makeMove(movelist[i], pos);
                                gameHistory.push_back(zobristHash(pos));
                                break;
                            }
                        }
                    }
                }
            }
        }

        else if (line.rfind("go", 0) == 0) {

            if (searchThread.joinable()) {
                requestSearchStop();
                joinSearchThread();
            }

            std::istringstream iss(line);
            std::string cmd, token;
            iss >> cmd;

            int depth = -1;
            long long movetime = -1;
            long long wtime = -1, btime = -1;
            long long winc = 0, binc = 0;
            int movestogo = -1;
            bool infinite = false;

            while (iss >> token) {

                if (token == "depth") {
                    iss >> depth;
                }

                else if (token == "movetime") {
                    iss >> movetime;
                }

                else if (token == "wtime") {
                    iss >> wtime;
                }

                else if (token == "btime") {
                    iss >> btime;
                }

                else if (token == "winc") {
                    iss >> winc;
                }

                else if (token == "binc") {
                    iss >> binc;
                }

                else if (token == "movestogo") {
                    iss >> movestogo;
                }

                else if (token == "infinite") {
                    infinite = true;
                }

            }

            int searchDepth;
            long long timeLimitMs = 0;

            if (infinite) {
                searchDepth = 8;
                timeLimitMs = 5000;
            }
            else if (depth > 0) {
                searchDepth = depth;
            }
            else if (movetime > 0) {
                searchDepth = 64;
                timeLimitMs = movetime - 50;
                if (timeLimitMs < 20)
                    timeLimitMs = 20;
            }
            else if (wtime > 0 || btime > 0) {
                long long myTime = pos.whiteToMove ? wtime : btime;
                long long myInc  = pos.whiteToMove ? winc  : binc;

                int assumedMovesToGo = (movestogo > 0) ? movestogo : 30;

                long long allocated = myTime / assumedMovesToGo + myInc;

                allocated = std::min(allocated, (myTime * 9) / 10);
                allocated -= 50;
                if (allocated < 20)
                    allocated = 20;

                searchDepth = 64;
                timeLimitMs = allocated;
            }
            else {
                searchDepth = 4;
            }

            clearSearchStop();
            searchRunning.store(true);
            searchThread = std::thread([&pos, searchDepth, timeLimitMs]() {
                Move best = iterativeDeepening(pos, searchDepth, timeLimitMs);
                std::cout << "bestmove " << moveToString(best) << std::endl;
                searchRunning.store(false);
            });
        }

        else if (line == "stop") {
            if (searchThread.joinable()) {
                requestSearchStop();
                joinSearchThread();
            }
        }

        else if (line == "quit") {
            if (searchThread.joinable()) {
                requestSearchStop();
                joinSearchThread();
            }
            break;
        }
    }

    if (searchThread.joinable()) {
        requestSearchStop();
        joinSearchThread();
    }
}