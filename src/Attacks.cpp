#include "Attacks.h"
#include <cstdlib> // for abs()

    bool isSquareAttacked(int square, bool byWhite, boardState& pos) {

        // pawn
        if (byWhite) {
            int left = square + 7;
            int right = square + 9;

            if (0 <= left && left < 64 && abs((left % 8) - (square % 8)) == 1 && pos.board[left] == WP)
                return true;

            if (0 <= right && right < 64 && abs((right % 8) - (square % 8)) == 1 && pos.board[right] == WP)
            // abs((left % 8) - (square % 8)) == 1 | checks whether left and square are on adjacent files (columns).
                return true;
        } else {
            int left = square - 7;
            int right = square - 9;

            if (0 <= left && left < 64 && abs((left % 8) - (square % 8)) == 1 && pos.board[left] == BP)
                return true;

            if (0 <= right && right < 64 && abs((right % 8) - (square % 8)) == 1 && pos.board[right] == BP)
                return true;
        }

        // knight attacks
        int knightOffsets[] = {-17, -15, -10, -6, 6, 10, 15, 17};

        for (int offset : knightOffsets) {
            int attackSquare = square + offset;

            if (0 <= attackSquare && attackSquare < 64) {
                int fileDiff = abs((attackSquare % 8) - (square % 8));
                int rankDiff = abs((attackSquare / 8) - (square / 8));
                // file = column || rank = row
            if ((fileDiff == 1 && rankDiff == 2) || (fileDiff == 2 && rankDiff == 1)) {
                if (byWhite) {
                    if (pos.board[attackSquare] == WN)
                        return true;
                } else {
                    if (pos.board[attackSquare] == BN)
                        return true;
                }
            }
        }
    }
    
        // bishop attacks
        int bishopDirs[] = {-9, -7, 7, 9};

        for (int dir : bishopDirs) {
            int attackSquare = square + dir;

            while (0 <= attackSquare && attackSquare < 64 &&
                abs((attackSquare % 8) - ((attackSquare - dir) % 8)) == 1) { 

                if (pos.board[attackSquare] != EMP) {
                    if (byWhite) {
                        if (pos.board[attackSquare] == WB || pos.board[attackSquare] == WQ)
                            return true;
                    } else {
                        if (pos.board[attackSquare] == BB || pos.board[attackSquare] == BQ)
                            return true;
                    }
                    break;
                }

                attackSquare += dir;
            }
        }

        // rook attacks
        int rookDirs[] = {-8, 8, -1, 1};

        for (int dir : rookDirs) {
            int attackSquare = square + dir;

            while (0 <= attackSquare && attackSquare < 64 &&
                (dir == -8 || dir == 8 ||
                    abs((attackSquare % 8) - ((attackSquare - dir) % 8)) == 1)) {

                    if (pos.board[attackSquare] != EMP) {
                        if (byWhite) {
                            if (pos.board[attackSquare] == WR || pos.board[attackSquare] == WQ)
                                return true;
                        } else {
                            if (pos.board[attackSquare] == BR || pos.board[attackSquare] == BQ)
                                return true;
                        }
                        break;
                    }

                attackSquare += dir;
            }
        }
        
        // king attacks
        int kingOffsets[] = {-9, -8, -7, -1, 1, 7, 8, 9};

        for (int offset : kingOffsets) {
            int attackSquare = square + offset;

            if (0 <= attackSquare && attackSquare < 64) {
                int fileDiff = abs((attackSquare % 8) - (square % 8));
                int rankDiff = abs((attackSquare / 8) - (square / 8));
                // file = column || rank = row
            if (fileDiff <= 1 && rankDiff <= 1) {
                if (byWhite) {
                    if (pos.board[attackSquare] == WK)
                        return true;
                } else {
                    if (pos.board[attackSquare] == BK)
                        return true;
                }
            }
        }
    }

        return false;
    }
