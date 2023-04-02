#include <iostream>
#include <bitset>
#include <string>
#include <time.h>
#include <vector>
#include "board.h"
#include "gui.h"
#include "algorithm.h"

using namespace std;

#define BOARD_SIZE 8
#define BIT bitset<64>

enum class Player{human=1, random, MCS};

Player get_player(int input){
    Player player;
    switch (input)
    {
        case 1:
        {
            player = Player::human;
            break;
        }
        case 2:
        {
            player = Player::random;
            break;
        }
        case 3:
        {
            player = Player::MCS;
            break;
        }
        default:{
            player = Player::human;
            break;
        }
    }
    return player;
}

string bit2strpos(BIT bit_pos){
    string str_pos = bit_pos.to_string();
    size_t pos = str_pos.find('1');
    size_t row = pos/8;
    size_t col = pos%8;
    col+='A';
    return to_string(row+1)+(char)col;
}

BIT human_move(){
    string input_str;
    size_t row, col, pos;
    BIT bit_pos;

    cin >> input_str;
    if (input_str.length() == 1) {
        switch (input_str[0])
        {
        case 'p':
            break;

        default:
            break;
        }
    }
    else{
        row = input_str[0]-'1';
        col = toupper(input_str[1])-'A';
        pos = row*BOARD_SIZE + col;
        bit_pos.set(63-pos);
    }
    return bit_pos;
}

void print_single_move_info(BIT bit_pos){
    if (bit_pos.any()){
        cout << bit2strpos(bit_pos) << "\n\n";
    }
    else cout << "PASS\n\n";
}

BIT move(Board board, Player player){
    BIT bit_pos;
    switch (player)
    {
        case Player::human:
        {
            return human_move();
            break;
        }
        case Player::random:
        {
            bit_pos = random_move(board);
            // print_single_move_info(bit_pos);
            return bit_pos;
            break;
        }
        case Player::MCS:
        {
            bit_pos = MCS(board, 500);
            // print_single_move_info(bit_pos);
            return bit_pos;
            break;
        }
        default:
        {
            return human_move();
            break;
        }
    }
}

Player switch_player(Player player_now, Player player1, Player player2){
    if (player_now == player1) return player2;
    else return player1;
}

void othello_game(Player player1, Player player2){
    Board board;
    printboard(board);
    BIT bit_move;
    Player player_now = player1;
    while(!board.check_end()){
        try
        {
            cout << board.get_player_str() << " turn: ";
            bit_move = move(board, player_now);
            if (player_now != Player::human) print_single_move_info(bit_move);
            if (board.get_legal_bit_moves().any() & !bit_move.any()){
                cerr << "You can't pass this turn!" << endl;
                continue;
            }
            board.move(bit_move);
            printboard(board);
            if (board.check_end()) cout << board.get_result_str() << endl;
            player_now = switch_player(player_now, player1, player2);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}
void play_muiti_game(Player player1, Player player2, size_t game_num){
    size_t player1_win=0, player2_win=0, draw=0;
    for (size_t i=0;i<game_num;i++){
        Board board;
        BIT bit_move;
        Player player_now = player1;
        while(!board.check_end()){
            try
            {
                bit_move = move(board, player_now);
                if (board.get_legal_bit_moves().any() & !bit_move.any()){
                    cerr << "You can't pass this turn!" << endl;
                    continue;
                }
                board.move(bit_move);
                if (board.check_end()){
                    auto winner = board.get_result();
                    switch (winner)
                    {
                    case Turn::black:{
                        player1_win+=1;
                        break;
                    }
                    case Turn::white:{
                        player2_win+=1;
                        break;
                    }
                    case Turn::none:{
                        draw+=1;
                    }
                    default:
                        break;
                    }
                    break;
                }
                player_now = switch_player(player_now, player1, player2);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        cout << "Player 1 win: " << player1_win << " Player 2 win: " << player2_win << " Draw: " << draw << endl;
    }
    
}

int main(void){
    srand( time(NULL) );
    int input1, input2;
    Player player1, player2;
    cout << "Please select a player from the list below" << endl;
    cout << " Human player\t(" << 1 << ")" << endl;
    cout << " Random player\t(" << 2 << ")" << endl;
    cout << " MCS player\t(" << 3 << ")" << endl;
    cout << "Enter player 1: ";
    cin >> input1;
    cout << "Enter player 2: ";
    cin >> input2;
    player1 = get_player(input1);
    player2 = get_player(input2);
    othello_game(player1, player2);
    // play_muiti_game(player1, player2, 1000);

    // gui board_gui;
    // Board board;
    // BIT bit_move = MCS(board, 500);
    // Node *test = new Node(board, bit_move);
    // board_gui.print(test->state);
    // test->get_child();
    // for (auto it:test->child){
    //     board_gui.print(it->pos);
    // }

    return 0;
}
