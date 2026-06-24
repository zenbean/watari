#include "board.hpp"
#include "common.hpp"

Board::Board(int boardSize):
  boardSize(boardSize), 
  board(boardSize*boardSize, std::nullopt), 
  neighbours(boardSize*boardSize),
  parent(boardSize*boardSize),
  groupLiberties(boardSize*boardSize),
  stonesInGroup(boardSize*boardSize)
{
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

// finding the parent of an object
int Board::Find(const int& x){
  if (parent[x]!=x){
    parent[x] = Find(parent[x]); // path compression
    return parent[x];
  }
  else return x;
}

// union depending on number of stones in a group
void Board::Union(const int& x, const int& y){
  int parentX = Find(x);
  int parentY = Find(y);

  if(parentX == parentY) return;

  if(stonesInGroup[parentX].size() < stonesInGroup[parentY].size()){
    std::swap(parentX, parentY);
  }

  // parentY < parentX
  parent[parentY] = parentX;

  for(int liberty:groupLiberties[parentY]){
    groupLiberties[parentX].insert(liberty);
  }
  groupLiberties[parentY].clear();
  
  for(int stone:stonesInGroup[parentY]){
    stonesInGroup[parentX].push_back(stone);
  }
  stonesInGroup[parentY].clear();
}

bool Board::ValidMove(const int& n, std::optional<Stone> colour){
  if(n<0 || n>=boardSize*boardSize || board[n]!=std::nullopt) return false;

  for(int neighbour:neighbours[n]){
    if(board[neighbour]==std::nullopt){
      return true;
    }
    else if(board[neighbour]==colour){
      int friendlyParent = Find(neighbour);
      if (groupLiberties[friendlyParent].size()>=2){
        return true;
      }
    }
    else {
      int opponentParent = Find(neighbour);
      if (groupLiberties[opponentParent].size()==1){
        return true;
      }
    }
  }
  return false;
}

void Board::PlayStone(const int& n, std::optional<Stone> colour){
  if (!ValidMove(n, colour)) return;
  board[n] = colour;
  parent[n] = n;
  stonesInGroup[n].clear();
  groupLiberties[n].clear();
  stonesInGroup[n].push_back(n);

  for (int neighbour:neighbours[n]){
    if(board[neighbour]==std::nullopt){
      groupLiberties[n].insert(neighbour);
    }
    else if(board[neighbour]==board[n]){ // same colour neighbour
      int friendlyParent = Find(neighbour);
      groupLiberties[friendlyParent].erase(n);
      Union(n, neighbour);
    }
    else{ // opposite colour neighbour
      int opponentParent=Find(neighbour);
      groupLiberties[opponentParent].erase(n);
    }
  }
}
