#include "board.hpp"
#include "common.hpp"

int Board::CoordinateToIndex(const int& x, const int& y){
    return y * Board::boardSize + x;
}

int Board::CountLiberties(const int& x, const int& y){
    
}