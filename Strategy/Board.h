//
//  Board.h
//  Strategy
//
//  Abstraction of board and basic game logic.
//
//  Created by Paul on 15/5/8.
//

#ifndef Strategy_Board_h
#define Strategy_Board_h

#include "Point.h"
#include <vector>

class Board
{
public:
    Board(const int M, const int N, const int* top, int** board);
    ~Board();
    
    // get available moves on current board
    std::vector<Point> getSuccessors();
    
    // apply a move
    // assume it is legal
    void applyMove(Point& pos);
    // return one of the following results:
    const static int USER_WIN = 1;
    const static int MACHINE_WIN = 2;
    const static int IS_TIE = 3;
    const static int CONTINUE = 4;
    
    // current player
    int player() const;
    // return one of the following:
    const static int USER = 1;
    const static int MACHINE = 2;
    
private:
    int** _board;
    int* _top;
    int _player;
};

#endif
