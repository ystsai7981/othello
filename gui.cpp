#include <iostream>
#include <bitset>
#include <string>
#include "gui.h"

using namespace std;
#define BIT bitset<64>
#define BOARD_GUI_OFFSET 3

string BOARD_TOP = "┌─┬─┬─┬─┬─┬─┬─┐";
string BOARD_MID = "├─┼─┼─┼─┼─┼─┼─┤";
string BOARD_BOT = "└─┴─┴─┴─┴─┴─┴─┘";
string X_TITLE = "  A B C D E F G H\n";
string Y_TITLE = "12345678";
string BLACK_SIGN = "●";
string WHITE_SIGN = "○";
string LEGAL_SIGN = "×";

gui::gui(){
    board = BOARD_TOP;
    for (int i=0;i<6;i++){
        board+=BOARD_MID;
    }
    board+=BOARD_BOT;
}

void gui::set(BIT black, BIT white, BIT legal){
    black_or_legal = black | legal;
    white_or_legal = white | legal;
}

void gui::print(){
    size_t board_pos = 0;
    string::iterator Y_it = Y_TITLE.begin();
    cout << X_TITLE;
    cout << *Y_it << ' ';
    Y_it++;
    for(int i=63;i>=0;i--){
        if (black_or_legal.test(i) & white_or_legal.test(i)) cout << LEGAL_SIGN;
        else if (black_or_legal.test(i) & ~white_or_legal.test(i)) cout << BLACK_SIGN;
        else if (~black_or_legal.test(i) & white_or_legal.test(i)) cout << WHITE_SIGN;
        else cout << board.substr(board_pos, BOARD_GUI_OFFSET);
        board_pos+=3;
        if (i%8==0) {
            cout << endl;
            cout << *Y_it << ' ';
            Y_it++;
        }
        else{
            cout << board.substr(board_pos, BOARD_GUI_OFFSET);
            board_pos+=BOARD_GUI_OFFSET;
        }
    }
}

void gui::print(std::bitset<64> bit_move){
    size_t board_pos = 0;
    string::iterator Y_it = Y_TITLE.begin();
    cout << X_TITLE;
    cout << *Y_it << ' ';
    Y_it++;
    for(int i=63;i>=0;i--){
        if ((bit_move).test(i)) cout << BLACK_SIGN;
        else cout << board.substr(board_pos, BOARD_GUI_OFFSET);
        board_pos+=3;
        if (i%8==0) {
            cout << endl;
            cout << *Y_it << ' ';
            Y_it++;
        }
        else{
            cout << board.substr(board_pos, BOARD_GUI_OFFSET);
            board_pos+=BOARD_GUI_OFFSET;
        }
    }
}

void gui::print(Board board){
    set(board.get_black(), board.get_white(), board.get_legal_bit_moves());
    print();
}

void printboard(Board board){
    gui board_gui;
    board_gui.print(board);
}

void printboard(BIT bit_move){
    gui board_gui;
    board_gui.print(bit_move);
}