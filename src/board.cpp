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

void Board::ExploreBoard(const int&n, const std::optional<Stone>& colour, BoardInfo& boardInfo, std::vector<char>& visitedPositions){
    if(colour==std::nullopt) return;
    if(visitedPositions[n]==1) return; // skip visited node
    visitedPositions[n]=1;
    boardInfo.groupPositions.insert(n);
    for (int neighbourPos:neighbours[n]){ // check neighbouring stones
        if (board[neighbourPos]==std::nullopt){ // empty spot?
            boardInfo.liberties.insert(neighbourPos);
        }
        else if (board[neighbourPos]==colour){
            ExploreBoard(neighbourPos, colour, boardInfo, visitedPositions);
        }
    }
}

BoardInfo Board::GetInfo(const int& n){
    BoardInfo boardInfo;
    std::vector<char> visitedPositions(boardSize*boardSize, 0);
    std::optional<Stone> colour = board[n];
    ExploreBoard(n, colour, boardInfo, visitedPositions);
    return boardInfo;
}

// simulate move regardless of legality
std::vector<std::pair<int, std::optional<Stone>>> Board::SimulateMove(const int& n, std::optional<Stone> colour){
    board[n]=colour;
    std::vector<char> visitedPos(boardSize*boardSize, 0);
    std::vector<std::pair<int, std::optional<Stone>>> changedPos;
    for(int neighbourPos:neighbours[n]){
        // only check neighbouring stones with opposite colour for capture opportunity
        if (board[neighbourPos]==std::nullopt || board[neighbourPos]==colour || visitedPos[neighbourPos]==1){
            continue;
        }
        BoardInfo neighbourInfo = GetInfo(neighbourPos);
        for (int pos:neighbourInfo.groupPositions){
            visitedPos[pos]=1;
        }
        if (neighbourInfo.liberties.empty()){
            for (int pos:neighbourInfo.groupPositions){
                board[pos] = std::nullopt; // capture the whole group
                changedPos.push_back();
            }
        }
    }
    return changedPos;
}

// validate simulated move
bool Board::ValidMove(const int& n){ // check if valid move with a copy of the board
    if(GetInfo(n).liberties.size()>0) return true;
    else return false;
}

void Board::PlayStone(const int& n, std::optional<Stone> colour){ // update the board with the valid move and captures
    if(n < 0 || n >= boardSize*boardSize) return;
    if(board[n]!=std::nullopt) return;
    std::vector<int> changedPos = SimulateMove(n, colour);
    if (!ValidMove(n)){
        // undo changes
        for(int pos:changedPos){
            board[pos]=std::nullopt;
        }
        board[n]=std::nullopt;
    }
};