#ifndef _GUI_H_
#define _GUI_H_
#include <bitset>
#include "board.h"

class gui{
private:
    std::string board; // board gui
    std::bitset<64> black_or_legal;
    std::bitset<64> white_or_legal;
    void set(std::bitset<64> black, std::bitset<64> white, std::bitset<64> legal);
    void print();
public:
    gui();
    void print(Board baord);
    void print(std::bitset<64> bit_move);
};
void printboard(Board board);
void printboard(std::bitset<64> bit_move);

#endif