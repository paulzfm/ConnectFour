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

#include "TreeNode.h"
#include "Board.h"

// constant parameters
#define ROOT 0
#define EMPTY -1
#define ITER 4600
#define C 1.0 // exploration parameter

class MCTS
{
public:
    MCTS(const int M, const int N, int** board, const int* top, int notX, int notY);
    
    // call this to obtain the best move
    Point decision();
    
    // print node recursively
    void printNode(int node, int indent);

private:
    // select one of successors of the given node
    // using UCT (Upper Confidence Bound 1 applied to trees)
    // return the index of the selected successor
    int select(int node);
    int selectPrint(int node);
    
    
    // expand a node and create new successor in tree
    // return the index of the new successor
    int expand(int node);
    
    // play a random game from the given node
    // return final result of the game
    int simulate(int node);
    
    // return the best move from the given node
    Point bestMove(int node);
    
    // search tree
    std::vector<TreeNode> _tree;
    
    // game board
    Board _backup_board; // backup
    Board _board;        // working
};

#endif // Strategy_MCTS_h
