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
    // return final result of the game
    int simulate(int node);
    
    // update payoff from the given node up to tree root
    // by given the simulation result
    void backpropagation(int node, int result);
    
    // search tree
    Tree tree;
    
    // game board
    Board* board;
};

#endif // Strategy_MCTS_h
