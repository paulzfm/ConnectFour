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

void Board::operator = (Board &that)
{
    _M = that._M;
    _N = that._N;
    _player = that._player;
    _notPos = that._notPos;
    _board = new int*[_M];
    for (int i = 0; i < _M; i++) {
        _board[i] = new int[_N];
        for (int j = 0; j < _N; j++) {
            _board[i][j] = that._board[i][j];
        }
    }
    
    _top = new int[_N];     // do not consider limitation
    for (int i = 0; i < _N; i++) { // for each column
        int j = _M - 1;
        for (; j >= 0; j--) { // from bottom to top
            if (_board[j][i] == 0) { // find a blank grid
                break;
            }
        }
        _top[i] = j + 1;
    }
}

std::ostream& operator << (std::ostream& out, Board& board)
{
    for (int i = 0; i < board._M; i++) {
        for (int j = 0; j < board._N; j++) {
            if (board._notPos == Point(i, j)) {
                out << "X ";
            } else if (board._board[i][j] == 0) {
                out << ". ";
            } else {
                out << board._board[i][j] << " ";
            }
        }
        out << std::endl;
    }
    
    return out;
}

std::vector<Point> Board::getSuccessors()
{
    std::vector<Point> succ;
    for (int i = 0; i < _N; i++) {
        if (_top[i] != 0 && (_top[i] - 1 != _notPos.x || i != _notPos.y)) {
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
    if (isTie(_N, _top, _notPos)) {
        return IS_TIE;
    }
    
    // switch player
    _player = _player == USER ? MACHINE : USER;
    
    return CONTINUE;
}

int Board::player() const
{
    return _player;
}
