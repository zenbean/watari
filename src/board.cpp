#include "board.hpp"
#include "common.hpp"

Board::Board(int boardSize):boardSize(boardSize), board(boardSize*boardSize, std::nullopt), neighbours(boardSize*boardSize){
    // compute neighbours for each position
    for (int i = 0; i < boardSize*boardSize; i++){
        //up
        if (i < boardSize*(boardSize-1)){
            neighbours[i].push_back(i + boardSize);
        }
        //down
        if (i >= boardSize){
            neighbours[i].push_back(i-boardSize);
        }
        //left
        if (i % boardSize != 0){
            neighbours[i].push_back(i-1);
        }
        //right
        if (i % boardSize != boardSize-1){
            neighbours[i].push_back(i+1);
        }
    }
}

int Board::CoordinateToIndex(const int& x, const int& y){
    return y * Board::boardSize + x;
}

void Board::FindLiberties(const int& n, std::optional<Stone> colour, std::unordered_set<int>& liberties, std::unordered_set<int>& visitedPositions){
    if(colour==std::nullopt) return;
    if(!visitedPositions.insert(n).second) return; // check if node is visited

    for (int neighbourPos:neighbours[n]){ // check neighbouring stones
        if (board[neighbourPos]==std::nullopt){ // empty spot?
            liberties.insert(neighbourPos);
        }
        else if (board[neighbourPos]==colour){
            FindLiberties(neighbourPos, colour, liberties, visitedPositions);
        }
    }
}

int Board::CountLiberties(const int& n){
    std::unordered_set<int> liberties;
    std::unordered_set<int> visitedPositions;
    std::optional<Stone> colour = board[n];
    FindLiberties(n, colour, liberties, visitedPositions);
    return liberties.size();
}

bool Board::ValidMove(const int& n, std::optional<Stone> colour){ // check if valid move
    if( n < 0 || n > 80) return false; // surely this is not possible if the grid is fixed and stones are only to be placed inside
    // temporarily place the stone then check liberties - store the old colour while we are checking for valid move (maybe this can happen in PlayStone)
    if(CountLiberties(n)==0) return false;
    return true;
};

void Board::PlayStone(const int& n, std::optional<Stone> colour){ // update the board with the valid move
    if(ValidMove(n, colour)) {
        board[n] = colour;
    }
};