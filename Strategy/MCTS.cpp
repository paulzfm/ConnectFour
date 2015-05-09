//
//  MCTS.cpp
//  Strategy
//
//  Created by Paul on 15/5/8.
//

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "MCTS.h"

MCTS::MCTS(const int M, const int N, int** board, int notX, int notY) :
    _board(M, N, board, notX, notY), _backup_board(M, N, board, notX, notY)
{
    srand(time(NULL));
    _tree.push_back(TreeNode(Point(0, 0), EMPTY));
    std::cout << "Not pos: " << Point(notX, notY) << std::endl;
//    std::cout << "### Current state ###\n" << _board;
}

void MCTS::printNode(int node, int indent)
{
    for (int i = 0; i < indent; i++) {
        std::cout << " ";
    }
    std::cout << _tree[node] << std::endl;
    for (int i = 0; i < _tree[node].succ.size(); i++) {
        printNode(_tree[node].succ[i], indent + 4);
    }
}

Point MCTS::decision()
{
    for (int k = 0; k < ITER; k++) {
        _board = _backup_board;
        
        // initialize
        int node = ROOT;
        int result = Board::CONTINUE;
        
        // tree policy: selection and expansion
        while (result == Board::CONTINUE) {
            if (!_tree[node].expanded ||
                _tree[node].succ.size() < _tree[node].allSucc.size()) {
                // not expanded or not fully expanded
                node = expand(node);
                result = _board.applyMove(_tree[node].move);
                break;
            } else { // fully expanded
                node = select(node);
                result = _board.applyMove(_tree[node].move);
            }
        }
        
        if (result == Board::CONTINUE) { // continue to simulation
            result = simulate(node);
        }
        
        // backpropagation
        for (; node != EMPTY; node = _tree[node].pred) {
            // update the statistics of tree nodes traversed
            _tree[node].update(result);
        }
        
//        std::cout << "#" << k << std::endl;
    }
    
//    printNode(ROOT, 0);
//    return bestMove(ROOT);
    return _tree[select(ROOT)].move;
//    return _tree[selectPrint(ROOT)].move;
}

int MCTS::select(int node)
{
    int n = _tree[node].succ.size();
    
    if (n < 1) {
        printf("***Error: select: n == 0!\n");
        exit(1);
    }
    
    float max = (float)_tree[0].payoff / _tree[0].count +
    C * sqrt(2 * log(_tree[node].count) / (float)_tree[0].count);
    int index = 0;
    for (int i = 1; i < n; i++) {
        int curr = _tree[node].succ[i];
        float value = _tree[curr].payoff / (float)_tree[curr].count +
            C * sqrt(2 * log(_tree[node].count) / (float)_tree[curr].count);
        if (value > max) {
            max = value;
            index = curr;
        }
    }
    
    return index;
}

int MCTS::selectPrint(int node)
{
    int n = _tree[node].succ.size();
    
    if (n < 1) {
        printf("***Error: select: n == 0!\n");
        exit(1);
    }
    
    float max = (float)_tree[0].payoff / _tree[0].count +
    C * sqrt(2 * log(_tree[node].count) / (float)_tree[0].count);
    std::cout << max << " ";
    int index = 0;
    for (int i = 1; i < n; i++) {
        int curr = _tree[node].succ[i];
        float value = _tree[curr].payoff / (float)_tree[curr].count +
        C * sqrt(2 * log(_tree[node].count) / (float)_tree[curr].count);
        std::cout << value << " ";
        if (value > max) {
            max = value;
            index = curr;
        }
    }
    
    return index;
}


int MCTS::expand(int node)
{
    if (!_tree[node].expanded) { // first time to expand this node
        std::vector<Point> moves = _board.getSuccessors(); // obtain candidate moves
        for (int i = 0; i < moves.size(); i++) {
            _tree.push_back(TreeNode(moves[i], node));  // add to tree
            _tree[node].allSucc.push_back(_tree.size() - 1);   // update successors
        }
        _tree[node].expanded = true; // finish expanding
    }
    
    int i = _tree[node].succ.size(); // expand the i-th node
    _tree[node].succ.push_back(_tree[node].allSucc[i]);
    
    return _tree[node].allSucc[i];
}

int MCTS::simulate(int node)
{
    while (true) {
//        std::cout << _board << std::endl;
        std::vector<Point> moves = _board.getSuccessors(); // obtain candidate moves
        int index = rand() % moves.size(); // randomly select one of them
        int result = _board.applyMove(moves[index]); // apply this move
        if (result == Board::CONTINUE) { // continue to play
            continue;
        } else {  // until reach an end
//            std::cout << "result is " << result << "\n";
            return result;
        }
    }
}

Point MCTS::bestMove(int node)
{
    int n = _tree[node].succ.size();
    float max = -1.0;  // maximum value
    int best = 0;     // best node index
    for (int i = 0; i < n; i++) {
        int curr = _tree[node].succ[i];
        float value = (float)_tree[curr].payoff / _tree[curr].count;
        if (value > max) {
            max = value;
            best = curr;
        }
    }
    
    return _tree[best].move;
}