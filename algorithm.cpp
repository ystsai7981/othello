#include <iostream>
#include <cstdlib>
#include <bitset>
#include <vector>
#include <cassert>
#include "algorithm.h"
#include "board.h"
#include "gui.h"

using namespace std;

#define BIT bitset<64>
BIT PASS (0);

vector<BIT> get_each_legal_moves(Board board){
    BIT legal_moves = board.get_legal_bit_moves();
    size_t legal_count = legal_moves.count();
    vector<BIT> legal_moves_list (legal_count);
    if (legal_count == 0) return legal_moves_list;
    int idx=0;
    BIT bit_move (1);
    do{
        if (legal_moves.test(0)){
            legal_moves_list[idx] = bit_move;
            idx++;
        }
        legal_moves >>= 1;
        bit_move <<= 1;
    } while(legal_moves.any());
    return legal_moves_list;
}

Node::Node(Board board){
    state = board;
    win = 0;
    loss = 0;
    total = 0;
    parent = NULL;
}

Node::Node(Board board, BIT bit_move, Node *parent_node){
    assert(((bit_move & board.get_legal_bit_moves()).any()) |
           !((bit_move.any()) && !(board.get_legal_bit_moves().any()))); // PASS
    win = 0;
    loss = 0;
    total = 0;
    state = board;
    state.move(bit_move);
    pos = bit_move;
    parent = parent_node;
}

Node::~Node(){
    for (auto it:children){
        delete it;
    }
    children.clear();
}

void Node::grow_tree(){
    if (state.check_end()) return;
    size_t legal_count = state.get_legal_bit_moves().count();
    if (legal_count == 0) {
        Node * new_child = new Node(state, PASS, this);
        children.push_back(new_child);
    }
    else{
        vector<BIT> legal_moves_list = get_each_legal_moves(state);
        for (auto it:legal_moves_list){
            Node * new_child = new Node(state, it, this);
            new_child->parent = this;
            children.push_back(new_child);
        }
    }
}

Turn roll_out(Board board){
    while (!board.check_end())
    {
        // printboard(board);
        auto bit_move = random_move(board);
        board.move(bit_move);
    }
    auto result = board.get_result();
    // cout << board.get_result_str() << endl;
    return result;
}

void Node::update(Turn player, Turn result){
    if (result == player) win+=1;
    else if (result != Turn::none && result != player) loss+=1;
    total+=1;
}

float Node::get_winrate(){
    if (total == 0) return 0;
    else return ((float)win/(float)total);
}

/* algorithm */
BIT random_move(Board board){
    BIT legal_moves = board.get_legal_bit_moves();
    size_t legal_count = legal_moves.count();
    if (legal_count == 0) return legal_moves;
    else{
        vector<BIT> legal_moves_list = get_each_legal_moves(board);
        return legal_moves_list[rand()%legal_count];
    }
}

BIT MCS(Board board, size_t max_num){
    BIT best_move;
    if (board.get_legal_bit_moves().count() == 0) return best_move;

    Node *root = new Node (board);
    float max_winrate = -1;
    root->grow_tree();
    for (size_t i=0;i<max_num;i++){
        auto chosed_child = root->children[rand()%root->children.size()];
        auto result = roll_out(chosed_child->state);
        chosed_child->update(root->state.get_player(), result);
    }
    for (auto it:root->children){
        // cout << "win num: " << it->win;
        // cout << " loss num: " << it->loss;
        // cout << " total num: " << it->total << endl;
        auto winrate = it->get_winrate();
        if (winrate > max_winrate) {
            max_winrate = winrate;
            best_move = it->pos;
        }
    }
    // Node *node_now = root;
    // node_now->grow_tree();
    // board_gui.print(node_now->state);
    // while (!(node_now->children.empty()))
    // {
    //     auto legal_count = node_now->state.get_legal_bit_moves().count();
    //     size_t randid;
    //     if(legal_count == 0) randid = 0;
    //     else randid = rand()%legal_count;
    //     board_gui.print(node_now->children[randid]->state);
    //     node_now = node_now->children[randid];
    //     node_now->grow_tree();
    // }

    delete root;
    
    return best_move;
}