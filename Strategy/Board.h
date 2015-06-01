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
#include <iostream>

class Board
{
public:
    Board(const int M, const int N, int** board, const int* top, int notX, int notY);
    ~Board();
    
    // copy constructor
    void operator = (Board& that);
    
    // stream
    friend std::ostream& operator << (std::ostream& out, Board& board);
    
    // get available moves on current board
    std::vector<Point> getSuccessors();
    
    // apply a move
    // assume it is legal
    int applyMove(Point& pos);
    // return one of the following results:
    const static int LOSE = 1;
    const static int WIN = 2;
    const static int TIE = 3;
    const static int CONTINUE = 4;
    
    // current player
    int player() const;
    // return one of the following results:
    const static int ME = 2;
    const static int OPPONENT = 1;
    // Important! This may be confusing sometimes.
    // Please note that when I say ME, I mean myself, which is MACHINE in the original
    // project. When I say OPPONENT, I mean my opponent, which is USER in the original
    // project.
    
private:
    int _M;
    int _N;
    int** _board;
    int* _top;
    int _player;
    Point _notPos;
};

#endif
