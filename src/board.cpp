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

void Board::FindLiberties(const int& n, const std::optional<Stone>& colour, std::unordered_set<int>& liberties, std::unordered_set<int>& visitedPositions){
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

void Board::FindGroup(const int&n, const std::optional<Stone>& colour, std::unordered_set<int>& groupPositions, std::unordered_set<int>& visitedPositions){
    if(colour==std::nullopt) return;
    if(!visitedPositions.insert(n).second) return; // check if node is visited
    groupPositions.insert(n);
    for (int neighbourPos:neighbours[n]){ // check neighbouring stones
        if (board[neighbourPos]==colour){
            groupPositions.insert(neighbourPos);
            FindGroup(neighbourPos, colour, groupPositions, visitedPositions);
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

std::unordered_set<int> Board::GetGroup(const int& n){
    std::unordered_set<int> groupPositions;
    std::unordered_set<int> visitedPositions;
    std::optional<Stone> colour = board[n];
    FindGroup(n, colour, groupPositions, visitedPositions);
    return groupPositions;
}

bool Board::ValidMove(const int& n, std::optional<Stone> colour){ // check if valid move with a copy of the board
    std::vector<std::optional<Stone>> temp_board = board;
    if(n<0 && n > boardSize*boardSize-1) return false;
    if(temp_board[n]!=std::nullopt) return false;
    temp_board[n] = colour; // temporary placement
    for(int neighbourPos:neighbours[n]){
        //check capture
        if(temp_board[neighbourPos]!=std::nullopt && temp_board[neighbourPos]!=colour && CountLiberties(neighbourPos, temp_board)==0){
            for(int opponentGroup:GetGroup(neighbourPos, temp_board)){
                temp_board[opponentGroup]=std::nullopt;
            }
        }
    }
    if(CountLiberties(n)>0) return true;
    else return false;
};

void Board::PlayStone(const int& n, std::optional<Stone> colour){ // update the board with the valid move and captures
    if(ValidMove(n, colour)) {
        board[n] = colour;
    }
};