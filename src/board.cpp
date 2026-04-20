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

void Board::FindLiberties(const int& n, const std::optional<Stone>& colour, std::unordered_set<int>& liberties, std::unordered_set<int>& visitedPositions, std::vector<std::optional<Stone>>& local_board){
    if(colour==std::nullopt) return;
    if(!visitedPositions.insert(n).second) return; // check if node is visited

    for (int neighbourPos:neighbours[n]){ // check neighbouring stones
        if (local_board[neighbourPos]==std::nullopt){ // empty spot?
            liberties.insert(neighbourPos);
        }
        else if (local_board[neighbourPos]==colour){
            FindLiberties(neighbourPos, colour, liberties, visitedPositions, local_board);
        }
    }
}

void Board::FindGroup(const int&n, const std::optional<Stone>& colour, std::unordered_set<int>& groupPositions, std::unordered_set<int>& visitedPositions, std::vector<std::optional<Stone>>& local_board){
    if(colour==std::nullopt) return;
    if(!visitedPositions.insert(n).second) return; // check if node is visited
    groupPositions.insert(n);
    for (int neighbourPos:neighbours[n]){ // check neighbouring stones
        if (local_board[neighbourPos]==colour){
            groupPositions.insert(neighbourPos);
            FindGroup(neighbourPos, colour, groupPositions, visitedPositions, local_board);
        }
    }
}

int Board::CountLiberties(const int& n, std::vector<std::optional<Stone>>& local_board){
    std::unordered_set<int> liberties;
    std::unordered_set<int> visitedPositions;
    std::optional<Stone> colour = local_board[n];
    FindLiberties(n, colour, liberties, visitedPositions, local_board);
    return liberties.size();
}

std::unordered_set<int> Board::GetGroup(const int& n, std::vector<std::optional<Stone>>& local_board){
    std::unordered_set<int> groupPositions;
    std::unordered_set<int> visitedPositions;
    std::optional<Stone> colour = local_board[n];
    FindGroup(n, colour, groupPositions, visitedPositions, local_board);
    return groupPositions;
}

std::vector<std::optional<Stone>> Board::SimulateMove(const int& n, std::optional<Stone> colour, std::vector<std::optional<Stone>> local_board){
    local_board[n]=colour;
    for(int neighbourPos:neighbours[n]){
        //check capture
        if(local_board[neighbourPos]!=std::nullopt && local_board[neighbourPos]!=colour && CountLiberties(neighbourPos, local_board)==0){
            for(int opponentGroup:GetGroup(neighbourPos, local_board)){
                local_board[opponentGroup]=std::nullopt;
            }
        }
    }
    return local_board;
}

bool Board::ValidMove(const int& n, std::vector<std::optional<Stone>>& local_board){ // check if valid move with a copy of the board
    if(CountLiberties(n, local_board)>0) return true;
    else return false;
};

void Board::PlayStone(const int& n, std::optional<Stone> colour){ // update the board with the valid move and captures
    if(n < 0 || n >= boardSize*boardSize-1) return;
    if(board[n]!=std::nullopt) return;
    std::vector<std::optional<Stone>> temp_board = SimulateMove(n, colour, board);
    if (ValidMove(n, temp_board)){
        board = temp_board;
    }
};