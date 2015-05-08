//
//  MonteCarlo.cpp
//  Strategy
//
//  Created by Paul on 15/4/29.
//  Copyright (c) 2015年 Yongfeng Zhang. All rights reserved.
//

#include <stdlib.h>
#include <vector>
#include "Strategy.h"
#include "Judge.h"
#include "MonteCarlo.h"

int monteCarlo(const int M, const int N, const int* top, int** board) {
//    for (int i = 0; i < M; i++) {
//        for (int j = 0; j < N; j++) {
//            printf("%d ", board[i][j]);
//        }
//        printf("\n");
//    }
    
    // best strategy: user win
    for (int i = 0; i < N; i++) {
        if (top[i] != 0) {
            board[top[i] - 1][i] = USER;
            if (userWin(top[i] - 1, i, M, N, board)) {
                return i;
            }
            board[top[i] - 1][i] = 0;
        }
    }
    
    // defeat strategy: machine win
    for (int i = 0; i < N; i++) {
        if (top[i] != 0) {
            board[top[i] - 1][i] = MACHINE;
            if (machineWin(top[i] - 1, i, M, N, board)) {
                printf("defeat at %d\n", i);
                return i;
            }
            board[top[i] - 1][i] = 0;
        }
    }

    int* realTop = new int[N];     // do not consider limitation
    for (int i = 0; i < N; i++) { // for each column
        int j = M - 1;
        for (; j >= 0; j--) { // from bottom to top
            if (board[j][i] == 0) { // find a blank grid
                break;
            }
        }
        realTop[i] = j + 1;
    }
    
    int* payoffs = new int[N];   // payoffs for each strategy (selecting the i-th column)
    for (int i = 0; i < N; i++)
        payoffs[i] = 0;
    int* _top = new int[N];      // copy of realTop
    int** _board = new int*[M];  // copy of board
    for (int i = 0; i < M; i++)
        _board[i] = new int[N];
    
    // search optimalizing strategy
    for (int k = 0; k < ITER; k++) {
//        printf("iter %d\n", k);
        
        // resume
        for (int i = 0; i < N; i++)
            _top[i] = realTop[i];
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                _board[i][j] = board[i][j];
        
        // play random game
        int result = playRandomGame(M, N, _top, _board, USER);
        
        // stat
        if (result == IS_TIE) // payoffs are zero
            continue;
        for (int i = 0; i < N; i++) {
            if (top[i] != 0) {
                if (_board[top[i] - 1][i] == USER && result == USER_WIN) {
                    payoffs[i] += SCORE_USER;
                } else if (_board[top[i] - 1][i] == MACHINE && result == USER_WIN) {
                    payoffs[i] -= SCORE_MACHINE;
                } else if (_board[top[i] - 1][i] == MACHINE && result == MACHINE_WIN) {
                    payoffs[i] += SCORE_MACHINE;
                } else if (_board[top[i] - 1][i] == USER && result == MACHINE_WIN) {
                    payoffs[i] -= SCORE_USER;
                }
            } else {
                payoffs[i] -= SCORE_MAX;
            }
        }
    }
    
    // make dicision
    for (int i = 0; i < N; i++) {
        printf("%d ", payoffs[i]);
    }
    printf("\n");
    
    int max = 0;
    for (int i = 0; i < N; i++)
        if (payoffs[i] > payoffs[max])
            max = i;
    
    for (int i = 0; i < M; i++)
        delete[] _board[i];
    delete[] _board;
    delete[] _top;
    delete[] payoffs;
    delete[] realTop;
    
    return max;
}

bool win(const int x, const int y, const int M, const int N,
         int* const* board, int player)
{
    if (player == USER) {
        return userWin(x, y, M, N, board);
    } else {
        return machineWin(x, y, M, N, board);
    }
}
    
int playRandomGame(const int M, const int N, int* top, int** board, int player)
{
    while (true) {
        bool flag = true;
        std::vector<int> candidates;
        for (int i = 0; i < N; i++) {
            if (top[i] != 0) {
                candidates.push_back(i);
                // current player win?
                board[top[i] - 1][i] = player;
                if (win(top[i] - 1, i, M, N, board, player)) {
                    return player == USER ? USER_WIN : MACHINE_WIN;
                }
                // defeat?
                board[top[i] - 1][i] = player == USER ? MACHINE : USER;
                if (win(top[i] - 1, i, M, N, board, player == USER ? MACHINE : USER)) {
                    flag = false;
                    top[i]--;
                    break;
                }
                // resume
                board[top[i] - 1][i] = 0;
            }
        }
        
        if (flag) { // randomize strategy
            if (candidates.size() == 0) {
                return IS_TIE;
            }
            int y = candidates[rand() % candidates.size()];
            board[top[y] - 1][y] = player;
            if (win(top[y] - 1, y, M, N, board, player)) {
                return player == USER ? USER_WIN : MACHINE_WIN;
            }
            top[y]--;
        }
        
        // continue next round
        player = player == USER ? MACHINE : USER;
    }
}