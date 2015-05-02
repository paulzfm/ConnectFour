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
    int realTop[MAX_N];     // do not consider limitation
    for (int i = 0; i < N; i++) { // for each column
        int j = M - 1;
        for (; j >= 0; j--) { // from bottom to top
            if (board[j][i] == 0) { // find a blank grid
                break;
            }
        }
        realTop[i] = j + 1;
    }
    
    int payoffs[MAX_N];         // payoffs for each strategy (selecting the i-th column)
    for (int i = 0; i < N; i++)
        payoffs[i] = 0;
    int _top[MAX_N];            // copy of realTop
    int** _board = new int*[M];  // copy of board
    for (int i = 0; i < M; i++)
        _board[i] = new int[N];
    
    
    for (int i = 0; i < N; i++) {
//        printf("solving i=%d...\n", i);
        if (top[i] != 0) { // available candidate strategy
            for (int k = 0; k < ITER; ++k) { // run random game iterately
//                printf("iter %i...\n", i);
                for (int i = 0; i < N; i++)
                    _top[i] = realTop[i];
                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        _board[i][j] = board[i][j];
                
                
                int result = playRandomGame(M, N, _top, _board, realTop[i] - 1, i);
                switch (result) {
                    case USER_WIN:
                        payoffs[i]++;
                        break;
                    case MACHINE_WIN:
                        payoffs[i]--;
                        break;
                    default:
                        break;
                }
            }
        } else {
            payoffs[i] = -1 - ITER; // never make this decision
        }
    }
    
    for (int i = 0; i < M; i++)
        delete[] _board[i];
    delete[] _board;
    
    // make dicision
    int max = 0;
    for (int i = 0; i < N; i++)
        if (payoffs[i] > payoffs[max])
            max = i;
    
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
    
int playRandomGame(const int M, const int N, int* top, int** board,
                   int x, int y)
{
//    printf("random game...\n");
    
    int player = USER; // user's turn
    
    while (true) {
//        printf("player:%d, try(%d, %d)\n", player, x, y);
        
        if (player == USER) {
            if (userWin(x, y, M, N, board)) {
                return USER_WIN;
            }
        } else { // player == MACHINE
            if (machineWin(x, y, M, N, board)) {
                return MACHINE_WIN;
            }
        }
        
//        printf("apply (%d, %d)\n", x, y);
        
        // apply move
        board[x][y] = player;
        top[y]--;
        
//        printf("done.\n");
        
        if (isTie(N, top)) {
            return IS_TIE;
        }
        
//        for (int i = 0; i < M; i++) {
//            for (int j = 0; j < N; j++) {
//                printf("%d ", board[i][j]);
//            }
//            printf("\n");
//        }
//        printf("\n");
        
        // the other player make a choice
        player = player == USER ? MACHINE : USER;
        bool flag = true;
        std::vector<int> candidates;
        for (int i = 0; i < N; i++) {
            if (top[i] != 0) {
                candidates.push_back(i);
                if (win(top[i] - 1, i, M, N, board, player)) {
                    flag = true;
                    x = top[i] - 1;
                    y = i;
                    break;
                }
            }
        }
        
        if (flag) { // random
//            for (int i = 0; i < N; i++)
//                printf("%d ", top[i]);
//            printf("\n");
            y = candidates[rand() % candidates.size()];
            x = top[y] - 1;
        }
    }
}