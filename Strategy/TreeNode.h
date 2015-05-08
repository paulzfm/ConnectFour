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
#include "Board.h"

struct TreeNode
{
    int payoff;  // number of wins * 2 + number of ties * 1
    int count;   // number of simulations
    Point move;  // action from parent to this node

    std::vector<int> succ; // successors
    int pred;   // predecessor
    
    TreeNode(const Point& p, int parent) : move(p)
    {
        this->payoff = count = 0;
        this->pred = parent;
    }

    bool isLeaf()
    {
        return succ.empty();
    }
    
    void update(int result)
    {
        count++;
        if (result == Board::USER_WIN) {
            payoff += 2;
        } else if (result == Board::IS_TIE) {
            payoff += 1;
        }
    }
};

#endif // Strategy_TreeNode_h