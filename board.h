#ifndef _BOARD_H_
#define _BOARD_H_
#include <bitset>

enum class Turn{black=1, white, none};
class Board{
private:
    std::bitset<64> black;
    std::bitset<64> white;
    Turn turn;
    void change_turn();
    // std::bitset<64> *own=&black;
    // std::bitset<64> *opposite=&white;
    std::bitset<64> get_flipped_pieces(std::bitset<64> bit_move);
    std::bitset<64> get_flipped_pieces_left(std::bitset<64> bit_move, std::bitset<64> mask, int offset);
    std::bitset<64> get_flipped_pieces_right(std::bitset<64> bit_move, std::bitset<64> mask, int offset);
    std::bitset<64> get_continuous_opposite_left(std::bitset<64> own, std::bitset<64> opposite, std::bitset<64> mask, int offset); //own may be a bit_move or exist pieces
    std::bitset<64> get_continuous_opposite_right(std::bitset<64> own, std::bitset<64> opposite, std::bitset<64> mask, int offset);
    
public:
    Board();
    std::bitset<64> get_black();
    std::bitset<64> get_white();
    std::bitset<64> get_legal_bit_moves();
    std::bitset<64> get_legal_bit_moves(std::bitset<64> black, std::bitset<64> white);
    bool move(std::bitset<64> bit_move);
    bool move(int int_move);
    Turn get_player();
    std::string get_player_str();
    Turn get_result();
    std::string get_result_str();
    bool check_end();
};

#endif