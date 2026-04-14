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
    if(!visitedPositions.insert(n).second) return; // visited the starting node

    for (int neighbourPos:neighbours[n]){ // check neighbouring stones
        if (visitedPositions.find(neighbourPos)==visitedPositions.end()){ // not in visisted
            if (board[neighbourPos]==std::nullopt){ // empty spot?
                liberties.insert(neighbourPos);
            }
            else if (board[neighbourPos]==colour){
                FindLiberties(neighbourPos, colour, liberties, visitedPositions);
            }
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