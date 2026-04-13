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

int Board::CountLiberties(const int& x, const int& y){
    int start = CoordinateToIndex(x, y);
    Stone colour = board[start];
    static int liberties;
    for (int i = start; i < boardSize*boardSize; i++){
       if (Board::visitedPositions.insert(i).second==true){ // not visited yet
            for (int n:neighbours[i]){ // check neighbouring stones
                if (board[n]==std::nullopt){ // stone colour match?
                    liberties++;
                    visitedPositions.insert(n);
                    CountLiberties(n, colour);
                }
                else{
                    visitedPositions.insert(n);
                }
            }
       }
    }
}

int Board::CountLiberties(const int& n, const Stone& colour){
    for (int i = n; i < boardSize*boardSize; i++){
       if (Board::visitedPositions.insert(i).second==true){ // not visited yet
            for (int n:neighbours[i]){ // check neighbouring stones
                if (board[n]==colour){ // stone colour match?
                    liberties++;
                    visitedPositions.insert(n);
                    CountLiberties(n, colour);
                }
                else{
                    visitedPositions.insert(n);
                }
            }
       }
    }
}