//
//  Board.h
//  Strategy
//
//  Created by Paul on 15/5/8.
//  Copyright (c) 2015年 Yongfeng Zhang. All rights reserved.
//

#ifndef Strategy_Board_h
#define Strategy_Board_h

class Board
{
public:
    Board(int** currentBoard);
    ~Board();
    
private:
    int** _board;
};

#endif
