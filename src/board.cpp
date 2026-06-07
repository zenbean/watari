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

void Board::ExploreBoard(const int& n, const std::optional<Stone>& colour, BoardInfo& boardInfo, std::vector<char>& visitedPositions){
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

// gives visited group positions and libterties of the stone
BoardInfo Board::GetInfo(const int& n){
    BoardInfo boardInfo;
    std::vector<char> visitedPositions(boardSize*boardSize, 0);
    std::optional<Stone> colour = board[n];
    ExploreBoard(n, colour, boardInfo, visitedPositions);
    return boardInfo;
}

// simulate move regardless of legality
bool Board::SimulateMove(const int& n, std::optional<Stone> colour)
{
    changes.clear();
    std::vector<char> visitedPos(boardSize * boardSize, 0);
    changes.push_back({n, board[n]});
    board[n] = colour;
    bool capturedSomething = false;
    for (int neighbourPos : neighbours[n]) {
        if (board[neighbourPos] == std::nullopt) continue;
        else if (board[neighbourPos] == colour) continue;
        else if (visitedPos[neighbourPos]) continue;
        else{
            BoardInfo neighbourInfo = GetInfo(neighbourPos);
            for (int pos : neighbourInfo.groupPositions)
                visitedPos[pos] = 1;
            if (neighbourInfo.liberties.empty())
            {
                capturedSomething = true;
                for (int pos : neighbourInfo.groupPositions)
                {
                    changes.push_back({pos, board[pos]});
                    board[pos] = std::nullopt;
                }
            }
        }
    }
    BoardInfo myInfo = GetInfo(n);
    if (!myInfo.liberties.empty()) return true;
    else if (capturedSomething) return true;
    return false;
}

void Board::PlayStone(const int& n, std::optional<Stone> colour)
{
    if (n < 0 || n >= boardSize * boardSize)
        return;
    if (board[n] != std::nullopt)
        return;
    if (!SimulateMove(n, colour))
    {
        for (auto& change : changes)
        {
            board[change.first] = change.second;
        }
        return;
    }
    changes.clear();
}