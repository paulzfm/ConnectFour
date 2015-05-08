//
//  MCTS.h
//  Strategy
//
//  MCTS (Monte Carlo Tree Search) algorithm.
//
//  Created by Paul on 15/5/8.
//

#ifndef Strategy_MCTS_h
#define Strategy_MCTS_h

#include "Tree.h"
#include "Board.h"
#include "Point.h"

class MCTS
{
public:
    MCTS(const int M, const int N, const int* top, int** board);
    ~MCTS();
    
    // call this to obtain the best move
    explicit Point decision();
    
private:
    // select one of successors of the given node
    // using UCT (Upper Confidence Bound 1 applied to trees)
    // return the index of the selected successor
    int select(int node);
    
    // expand the leaf node and create new successor in tree
    // return the index of the new successor
    int expand(int node);
    
    // play a random game from the given node
    int simulate(int node);
    // return one of the following results:
    const static int USER_WIN = 1;
    const static int MACHINE_WIN = 2;
    const static int IS_TIE = 3;
    
    // update payoff from the given node up to tree root
    // by given the simulation result
    void backpropagation(int node, int result);
    
    // search tree
    Tree tree;
    
    // game board
    Board* board;
};

#endif // Strategy_MCTS_h
