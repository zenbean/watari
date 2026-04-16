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

void Board::FindLiberties(const int& n, std::optional<Stone> colour, std::unordered_set<int>& liberties, std::unordered_set<int>& visitedPositions, std::unordered_set<int>& groupPositions){
    if(colour==std::nullopt) return;
    if(!visitedPositions.insert(n).second) return; // check if node is visited

    for (int neighbourPos:neighbours[n]){ // check neighbouring stones
        if (board[neighbourPos]==std::nullopt){ // empty spot?
            liberties.insert(neighbourPos);
        }
        else if (board[neighbourPos]==colour){
            groupPositions.insert(neighbourPos);
            FindLiberties(neighbourPos, colour, liberties, visitedPositions);
        }
    }
}

int Board::CountLiberties(const int& n, std::unordered_set<int>& groupPositions){
    std::unordered_set<int> liberties;
    std::unordered_set<int> visitedPositions;
    std::optional<Stone> colour = board[n];
    FindLiberties(n, colour, liberties, visitedPositions, groupPositions);
    return liberties.size();
}

bool Board::ValidMove(const int& n, std::optional<Stone> colour){ // check if valid move
    if( n < 0 || n > (boardSize * boardSize) - 1) return false; 
    if (board[n]!=std::nullopt) return false;
    // temporarily place the stone then check liberties - if it isn't valid, turn it into empty
    board[n]=colour;
    std::unordered_set<int> groupPositions;
    if(CountLiberties(n, groupPositions)==0) {
        // check if capturing opponent pieces
        for (int neighbourPos:neighbours[n]){
            if(board[neighbourPos]!=colour && board[neighbourPos]!=std::nullopt){
                CountLiberties(n, groupPositions);
                if(groupPositions.size()==0){
                    for(int pos:groupPositions){
                        board[pos]=std::nullopt;
                    }
                }
            }
        }
    }
    return true;
};

void Board::PlayStone(const int& n, std::optional<Stone> colour){ // update the board with the valid move
    if(ValidMove(n, colour)) {
        board[n] = colour;
    }
};