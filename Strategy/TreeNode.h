//
//  TreeNode.h
//  Strategy
//
//  Search Tree Node for MCTS.
//
//  Created by Paul on 15/5/8.
//

#ifndef Strategy_TreeNode_h
#define Strategy_TreeNode_h

#include <vector>
#include "Point.h"

struct TreeNode
{
    int payoff;  // number of wins - number of lose
    int count;   // number of simulations
    std::vector<int> succ; // successors
    
    int player; // player
    Point move; // action
    
    TreeNode(const Point& p) : move(p) {}

    bool isLeaf()
    {
        return succ.empty();
    }
    
    void update(int result)
    {
        
    }
};

#endif // Strategy_TreeNode_h