//
//  MonteCarlo.h
//  Monte carlo strategy.
//
//  Created by Paul on 15/4/28.
//

#ifndef Strategy_MonteCarlo_h
#define Strategy_MonteCarlo_h

#include "Point.h"

#define MAX(x, y) (x > y ? (x) : (y))

const static int USER_WIN = 1;
const static int MACHINE_WIN = 2;
const static int IS_TIE = 3;

const static int USER = 1;
const static int MACHINE = 2;
    
const static int ITER = 2000;
const static int SCORE_USER = 1;
const static int SCORE_MACHINE = 2;
const static int SCORE_MAX = MAX(SCORE_USER, SCORE_MACHINE) + 1;

// interface
int monteCarlo(const int M, const int N, const int* top, int** board);

bool win(const int x, const int y, const int M, const int N,
         int* const* board, int player);

int playRandomGame(const int M, const int N, int* top, int** board, int player);

#endif