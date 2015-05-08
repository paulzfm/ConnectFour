//
//  MCTS.cpp
//  Strategy
//
//  Created by Paul on 15/5/8.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MCTS.h"

MCTS::MCTS(const int M, const int N, int** board, int notX, int notY) :
    _board(M, N, board, notX, notY)
{
    _total = 0;
    _tree.push_back(TreeNode(Point(0, 0), EMPTY));
}

Point MCTS::decision()
{
    for (int k = 0; k < ITER; k++) {
        printf("current iter=%d\n", k);
        
        // initialize
        int node = ROOT;
        std::vector<int> visited;
        visited.push_back(ROOT);
        int result = Board::CONTINUE;
        
        // selection
        while (!_tree[node].isLeaf()) {
            node = select(node);
            visited.push_back(node);
            result = _board.applyMove(_tree[node].move);
        }
        
        if (result == Board::CONTINUE) { // continue expansion
            node = expand(node);
            visited.push_back(node);
            result = _board.applyMove(_tree[node].move);
            
            if (result == Board::CONTINUE) { // continue simulation
                result = simulate(node);
            } // else: no need to play a random game, jump to backpropagation
        } // else: no need to play a random game, jump to backpropagation
        
        // backpropagation
        for (; node != EMPTY; node = _tree[node].pred) {
            // update the statistics of tree nodes traversed
            _tree[node].update(result);
        }
    }
    
    return bestMove(ROOT);
}

int MCTS::select(int node)
{
    int n = _tree[node].succ.size();
    float max = 0.0;
    int index = 0;
    for (int i = 0; i < n; i++) {
        int curr = _tree[node].succ[i];
        float value = (float)_tree[curr].payoff / _tree[curr].count +
            C * sqrt(2 * log(_total) / (float)_tree[curr].count);
        if (value > max) {
            max = value;
            index = curr;
        }
    }
    
    return index;
}

int MCTS::expand(int node)
{
    std::vector<Point> moves = _board.getSuccessors(); // obtain candidate moves
    int index = rand() % moves.size(); // randomly select one of them
    _tree.push_back(TreeNode(moves[index], node));  // add to tree
    _tree[node].succ.push_back(_tree.size() - 1);   // update successors
    
    return _tree.size() - 1;
}

int MCTS::simulate(int node)
{
    _total++;
    while (true) {
        std::vector<Point> moves = _board.getSuccessors(); // obtain candidate moves
        int index = rand() % moves.size(); // randomly select one of them
        int result = _board.applyMove(moves[index]); // apply this move
        if (result == Board::CONTINUE) { // continue to play
            continue;
        } else {  // until reach an end
            return result;
        }
    }
}

Point MCTS::bestMove(int node)
{
    int n = _tree[node].succ.size();
    float max = 0.0;  // maximum value
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