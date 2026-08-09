#include "Move.h"

std::string moveToString(const Move& move) {

    if (move.from < 0 || move.to < 0)
        return "0000";
        
    std::string s;

    s += 'a' + (move.from % 8);
    s += '8' - (move.from / 8);

    s += 'a' + (move.to % 8);
    s += '8' - (move.to / 8);

    if (move.promotion != EMP) {
        switch (move.promotion) {
            case WQ:
            case BQ: s += 'q'; break;
            case WR:
            case BR: s += 'r'; break;
            case WB:
            case BB: s += 'b'; break;
            case WN:
            case BN: s += 'n'; break;
            default: break;
        }
    }

    return s;
}