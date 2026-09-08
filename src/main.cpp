#include <iostream>
#include <fstream>
#include "BoardState.h"
#include "Piece.h"
#include "Perft.h"
#include "Zobrist.h"
#include "TranspositionTable.h"

#include "UCI.h"

int main() {

        
        initZobrist();
        initTT();

        uciLoop();
        return 0;

}
