//
//  TreeNode.cpp
//  Strategy
//
//  Created by Paul on 15/5/9.
//

#include "TreeNode.h"

std::ostream& operator << (std::ostream& out, TreeNode& node)
{
    out << "(" << node.move << ", " << node.payoff << "/" << node.count << ")";
    return out;
}