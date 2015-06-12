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

#include <assert.h>

MCTS::MCTS(const int M, const int N, int** board, const int* top, int notX, int notY) :
    _board(M, N, board, top, notX, notY), _backup_board(M, N, board, top, notX, notY),
    _backup_board_another(M, N, board, top, notX, notY)
{
    _tree.push_back(TreeNode(Point(0, 0), EMPTY, _board.player()));
//    std::cout << "### Current state ###\n" << _board << "==================\n";
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

bool MCTS::naiveMove(Point& move)
{
    std::vector<Point> succ = _board.getSuccessors();
    
    // a) best of all: current player will win
    for (const auto& e : succ) {
        if (_board.tryMove(e, _board.player())) {
            move = e;
            return true;
        }
    }
    
    // b) have to defeat: the other player will win
    for (const auto& e : succ) {
        if (_board.tryMove(e, _board.player() ==
            Board::ME ? Board::OPPONENT : Board::ME)) {
            move = e;
            return true;
        }
    }
    
    return false;
}

Point MCTS::decision()
{
    // first part
    while (!_tree[ROOT].expanded || _tree[ROOT].succ.size() <
           _tree[ROOT].allSucc.size()) { // root node is not fully expanded
        _board = _backup_board;
        
        // expand and applymove
        int node = expand(ROOT);
        int result = _board.applyMove(_tree[node].move);
        
        // repeat ITER_FIRST times
        _backup_board_another = _board;
        for (int i = 0; i < ITER_FIRST; i++) {
            int result1 = result;
            if (result1 == Board::CONTINUE) { // continue to simulation
                _board = _backup_board_another;
                result1 = simulate(node);
            }
                
            // backpropagation
            _tree[node].update(result1);
            _tree[ROOT].update(result1);
        }
    }
    
    // second part
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
    }
    
//    printNode(ROOT, 0);
    return bestMove(ROOT);
}

int MCTS::select(int node)
{
    int n = _tree[node].succ.size();
    
    float max = -100.0;
    int index = -1;
    for (int i = 0; i < n; i++) {
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

int MCTS::expand(int node)
{
    if (!_tree[node].expanded) { // first time to expand this node
        std::vector<Point> moves = _board.getSuccessors(); // obtain candidate moves
        for (const auto& move : moves) {
            _tree.push_back(TreeNode(move, node, _board.player())); // add to tree
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
        // check naive move
        Point move;
        if (naiveMove(move)) {
            int result = _board.applyMove(move);
            if (result == Board::CONTINUE) {
                continue;
            } else {
                assert(result == Board::WIN || result == Board::LOSE || result == Board::TIE);
                return result;
            }
        }
        
        // use random strategy
        std::vector<Point> moves = _board.getSuccessors(); // obtain candidate moves
        std::uniform_int_distribution<int> distribution(0, moves.size() - 1);
        int index = distribution(_generator); // randomly select one of them
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