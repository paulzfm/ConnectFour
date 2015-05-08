//
//  Board.cpp
//  Strategy
//
//  Created by Paul on 15/5/8.
//

#include "Board.h"
#include "Judge.h"
#include <stdlib.h>

Board::Board(const int M, const int N, int** board, int notX, int notY) :
    _notPos(notX, notY), _M(M), _N(N), _player(USER)
{
    _board = new int*[M];
    for (int i = 0; i < M; i++) {
        _board[i] = new int[N];
        for (int j = 0; j < N; j++) {
            _board[i][j] = board[i][j];
        }
    }
    
    _top = new int[N];     // do not consider limitation
    for (int i = 0; i < N; i++) { // for each column
        int j = M - 1;
        for (; j >= 0; j--) { // from bottom to top
            if (_board[j][i] == 0) { // find a blank grid
                break;
            }
        }
        _top[i] = j + 1;
    }
}

Board::~Board()
{
    delete[] _top;
    for (int i = 0; i < _M; i++) {
        delete[] _board[i];
    }
    delete[] _board;
}

std::vector<Point> Board::getSuccessors()
{
    std::vector<Point> succ;
    for (int i = 0; i < _N; i++) {
        if (_top[i] != 0 && _top[i] - 1 != _notPos.x && i != _notPos.y) {
            succ.push_back(Point(_top[i] - 1, i));
        }
    }
    
    return succ;
}

int Board::applyMove(Point &pos)
{
    // update board
    _board[pos.x][pos.y] = _player;
    _top[pos.y]--;
    
    // game is over?
    if (_player == USER && userWin(pos.x, pos.y, _M, _N, _board)) {
        return USER_WIN;
    }
    if (_player == MACHINE && machineWin(pos.x, pos.y, _M, _N, _board)) {
        return MACHINE_WIN;
    }
    if (isTie(_N, _top)) { // TODO: call isTie(_N, _top, _notPos) in future!
        return IS_TIE;
    }
    
    // generate next random notPos
    std::vector<Point> empty;
    for (int i = 0; i < _M; i++) {
        for (int j = 0; j < _N; j++) {
            if (_board[i][j] == 0) {
                empty.push_back(Point(i, j));
            }
        }
    }
    _notPos = empty[rand() % empty.size()];
    
    // switch player
    _player = _player == USER ? MACHINE : USER;
    
    return CONTINUE;
}

int Board::player() const
{
    return _player;
}
