#include <iostream>
#include "board.h"
#include <bitset>
using namespace std;

#define BOARD_SIZE 8
#define BOARD_LENGTH 8*8
#define BIT bitset<64>

BIT left_right_mask (0x7e7e7e7e7e7e7e7e);
BIT top_bottom_mask (0x00ffffffffffff00);
BIT corner_mask (left_right_mask&top_bottom_mask);
BIT START_POSITION (0x8000000000000000);
const int directions[] = {1, 9, 8, 7};
const BIT masks[] = {left_right_mask, corner_mask, top_bottom_mask, corner_mask};

Board::Board(){
    black = BIT (0x0000000810000000);
    white = BIT (0x0000001008000000);
    turn = Turn::black;
}

BIT Board::get_black(){
    return black;
}

BIT Board::get_white(){
    return white;
}

void Board::change_turn(){
    if (turn == Turn::black) turn = Turn::white;
    else turn = Turn::black;
}

//Make a move and swap own and opposite, own is always the next player.
bool Board::move(BIT bit_move){
    // dicide own is black or white
    BIT *own, *opposite;
    if (turn == Turn::black){
        own = &black;
        opposite = &white;
    }
    else{
        own = &white;
        opposite = &black;
    }

    if (bit_move != 0ULL /*pass this turn*/){
        // If legal or not
        if ((bit_move & get_legal_bit_moves()) == 0) {
            cerr << "Not a legal move!" << endl;
            return false;
        }
        else{
            BIT flipped_pieces = get_flipped_pieces(bit_move);
            *own |= (flipped_pieces | bit_move);
            *opposite &= ~flipped_pieces;
        }
    }

    change_turn();
    return true;
}

bool Board::move(int int_move){
    BIT bit_move = (START_POSITION >> int_move);
    return move(bit_move);
}


// Find flipped continuous pieces of left side from one move
BIT Board::get_flipped_pieces_left(BIT bit_move, BIT mask, int offset){
    // dicide own and opposite
    BIT *own, *opposite;
    if (turn == Turn::black){
        own = &black;
        opposite = &white;
    }
    else{
        own = &white;
        opposite = &black;
    }

    BIT flipped_stones = get_continuous_opposite_left(bit_move, *opposite, mask, offset);
    if ((*own & (flipped_stones << offset)) == 0){
        return BIT(0ULL);
    }
    else return flipped_stones;
}

// Find flipped continuous pieces of right side from one move
BIT Board::get_flipped_pieces_right(BIT bit_move, BIT mask, int offset){
    // dicide own and opposite
    BIT *own, *opposite;
    if (turn == Turn::black){
        own = &black;
        opposite = &white;
    }
    else{
        own = &white;
        opposite = &black;
    }

    BIT flipped_stones = get_continuous_opposite_right(bit_move, *opposite, mask, offset);
    if ((*own & (flipped_stones >> offset)) == 0){
        return BIT(0ULL);
    }
    else return flipped_stones;
}

// Get all flipped_pieces
BIT Board::get_flipped_pieces(BIT bit_move){
    BIT flipped_pieces (0ULL);
    for (int i=0;i<4;i++){
        flipped_pieces |= get_flipped_pieces_left(bit_move, masks[i], directions[i]);
        flipped_pieces |= get_flipped_pieces_right(bit_move, masks[i], directions[i]);
    }
    return flipped_pieces;
}

// Find continuous pieces of left side
BIT Board::get_continuous_opposite_left(BIT own_, BIT opposite_, BIT mask, int offset){
    BIT masked_opposite = opposite_ & mask;
    BIT continuous_opposite = masked_opposite & (own_ << offset);
    for (int i=0;i<5;i++){
        continuous_opposite |= masked_opposite & (continuous_opposite << offset);
    }
    return continuous_opposite;
}

// Find continuous of right side
BIT Board::get_continuous_opposite_right(BIT own_, BIT opposite_, BIT mask, int offset){
    BIT masked_opposite = opposite_ & mask;
    BIT continuous_opposite = masked_opposite & (own_ >> offset);
    for (int i=0;i<5;i++){
        continuous_opposite |= masked_opposite & (continuous_opposite >> offset);
    }
    return continuous_opposite;
}

// Return bit-viewed legal moves
BIT Board::get_legal_bit_moves(){
    // dicide own and opposite
    BIT *own, *opposite;
    if (turn == Turn::black){
        own = &black;
        opposite = &white;
    }
    else{
        own = &white;
        opposite = &black;
    }
    return get_legal_bit_moves(*own, *opposite);
}

BIT Board::get_legal_bit_moves(BIT black, BIT white){
    BIT legal_moves (0ULL);
    // Expand one move from each direction
    for (int i=0;i<4;i++){
        legal_moves |= (get_continuous_opposite_left(black, white, masks[i], directions[i]) << directions[i]);
        legal_moves |= (get_continuous_opposite_right(black, white, masks[i], directions[i]) >> directions[i]);
    }
    // Exclude exist pieces
    legal_moves &= ~(black | white);
    return legal_moves;
}

Turn Board::get_player() {
    return turn;
} 

string Board::get_player_str(){
    if (turn == Turn::black) return "Black";
    else return "White";
}

Turn Board::get_result(){
    size_t black_sum = black.count();
    size_t white_sum = white.count();
    if (black_sum > white_sum) return Turn::black;
    else if (black_sum < white_sum) return Turn::white;
    else return Turn::none;
}

string Board::get_result_str(){
    auto result = get_result();
    switch (result)
    {
    case Turn::black :{
        return "Black WIN!";
        break;
    }
    case Turn::white :{
        return "White WIN!";
        break;
    }
    case Turn::none :{
        return "Draw";
        break;
    }
    default:
        return "ERROR RESULT";
        break;
    }
}

bool Board::check_end(){
    if ((get_legal_bit_moves(black, white).any()) | (get_legal_bit_moves(white, black).any())) return false;
    else return true;
}
