#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_
#include <bitset>
#include <vector>
#include "board.h"

class Node{  
public:
    Board state;
    std::bitset<64> pos;
    u_int32_t win;
    u_int32_t loss;
    u_int32_t total;
    std::vector <Node *> children;
    Node *parent;
    Node();
    Node(Board board);
    Node(Board board, std::bitset<64> bit_move, Node *parent_node);
    ~Node();
    void grow_tree();
    void update(Turn player, Turn result);
    float get_winrate();
};

std::bitset<64> random_move(Board board);
Turn roll_out(Board board);
std::bitset<64> MCS(Board board, size_t max_num);

#endif