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

void Board::ExploreBoard(const int&n, const std::optional<Stone>& colour, BoardInfo& boardInfo, std::unordered_set<int>& visitedPositions, std::vector<std::optional<Stone>>& local_board){
    if(colour==std::nullopt) return;
    if(!visitedPositions.insert(n).second) return; // check if node is visited
    boardInfo.groupPositions.insert(n);
    for (int neighbourPos:neighbours[n]){ // check neighbouring stones
        if (local_board[neighbourPos]==std::nullopt){ // empty spot?
            boardInfo.liberties.insert(neighbourPos);
        }
        else if (local_board[neighbourPos]==colour){
            ExploreBoard(neighbourPos, colour, boardInfo, visitedPositions, local_board);
        }
    }
}

BoardInfo Board::GetInfo(const int& n, std::vector<std::optional<Stone>>& local_board){
    BoardInfo boardInfo;
    std::unordered_set<int> visitedPositions;
    std::optional<Stone> colour = local_board[n];
    ExploreBoard(n, colour, boardInfo, visitedPositions, local_board);
    return boardInfo;
}

// simulate move regardless of legality
std::vector<std::optional<Stone>> Board::SimulateMove(const int& n, std::optional<Stone> colour, std::vector<std::optional<Stone>> local_board){
    local_board[n]=colour;
    std::unordered_set<int> visitedPos;
    for(int neighbourPos:neighbours[n]){
        // only check neighbouring stones with opposite colour for capture opportunity
        if (local_board[neighbourPos]==std::nullopt || local_board[neighbourPos]==colour || visitedPos.find(neighbourPos)!=visitedPos.end()){
            continue;
        }
        BoardInfo neighbourInfo = GetInfo(neighbourPos, local_board);
        for (int pos:neighbourInfo.groupPositions){
            visitedPos.insert(pos);
        }
        if (neighbourInfo.liberties.empty()){
            for (int pos:neighbourInfo.groupPositions){
                local_board[pos] = std::nullopt; // capture the whole group
            }
        }
    }
    return local_board;
}

// validate simulated move
bool Board::ValidMove(const int& n, std::vector<std::optional<Stone>>& local_board){ // check if valid move with a copy of the board
    if(GetInfo(n, local_board).liberties.size()>0) return true;
    else return false;
}

void Board::PlayStone(const int& n, std::optional<Stone> colour){ // update the board with the valid move and captures
    if(n < 0 || n >= boardSize*boardSize) return;
    if(board[n]!=std::nullopt) return;
    std::vector<std::optional<Stone>> temp_board = SimulateMove(n, colour, board);
    if (ValidMove(n, temp_board)){
        board = temp_board;
    }
};