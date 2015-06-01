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
#include <iostream>
#include "Point.h"
#include "Board.h"

struct TreeNode
{
    int payoff;  // number of wins - number of loses
    int count;   // number of simulations
    Point move;  // action from parent to this node
    int player;  // which player?

    bool expanded;
    std::vector<int> succ; // successors
    std::vector<int> allSucc; // all successors
    int pred;   // predecessor
    
    TreeNode(const Point& p, int parent, int player)
        : move(p), player(player)
    {
        this->payoff = this->count = 0;
        this->pred = parent;
        this->expanded = false;
    }
    
    friend std::ostream& operator << (std::ostream& out, TreeNode& node);

    void update(int result)
    {
        count++;
        if (result == player) {
            payoff += 1;
//            payoff++;
        } else if (result == Board::TIE) {
//            payoff += 1;
        } else { // result != player
            payoff -= 1;
        }
    }
};

#endif // Strategy_TreeNode_h