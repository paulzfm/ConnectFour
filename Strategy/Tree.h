//
//  Tree.h
//  Strategy
//
//  Search Tree for MCTS.
//
//  Created by Paul on 15/5/8.
//

#ifndef Strategy_Tree_h
#define Strategy_Tree_h

#include <vector>

struct TreeNode
{
    int payoff;  // number of wins - number of lose
    int count;   // number of simulations
    std::vector<int> succ; // successors
    
    int player; // player
    Point move; // action
};


struct Tree
{
    std::vector<TreeNode> nodes;
};

#endif