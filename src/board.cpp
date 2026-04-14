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

int Board::CountLiberties(const int& x, const int& y, int& liberties){
    int start = CoordinateToIndex(x, y); // starting stone 1D index
    auto colour = board[start]; // colour to search liberties for
    visitedPositions.insert(start); // visited the starting node

    for (int n:neighbours[start]){ // check neighbouring stones
        if (visitedPositions.find(n)==visitedPositions.end()){ // not in visisted
            if (board[n]==std::nullopt){ // empty spot?
                liberties++;
            }
            else if (board[n]==colour){
                visitedPositions.insert(n);
                CountLiberties(n%boardSize,(int)(n/boardSize));
            }
            else{
                visitedPositions.insert(n);
            }
        }
    }
}