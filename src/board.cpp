#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include "zobrist.hpp"

Board::Board(int boardSize):
  boardSize(boardSize), 
  board(boardSize*boardSize, std::nullopt), 
  neighbours(boardSize*boardSize),
  parent(boardSize*boardSize),
  groupLiberties(boardSize*boardSize),
  stonesInGroup(boardSize*boardSize)
{
  Zobrist::Init();
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
  uint64_t predictedHash = PredictHash(n, colour);
  if(hashRecord.count(predictedHash)>0){
    return false;
  }
  hashRecord.insert(currentHash);
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

void Board::CaptureLogic(const int& n){
  std::optional<Stone> opponentColour = board[n];
  int int_colour = static_cast<int>(opponentColour.value());
  // restore liberties if friendly colour
  for(int stone:stonesInGroup[n]){
    for (int neighbour:neighbours[stone]){
      if (board[neighbour]!=opponentColour && board[neighbour]!=std::nullopt){
        int friendlyParent = Find(neighbour);
        groupLiberties[friendlyParent].insert(stone);
      }
    }
  }
  // clear captured stone from the board
  for (int stone:stonesInGroup[n]){
    currentHash ^= Zobrist::zobristTable[stone][int_colour];
    board[stone]=std::nullopt;
  }
  // clear DSU
  stonesInGroup[n].clear();
  groupLiberties[n].clear();
} 

void Board::PlayStone(const int& n, std::optional<Stone> colour){
  if (!ValidMove(n, colour)) return;
  board[n] = colour;
  parent[n] = n;
  stonesInGroup[n].clear();
  groupLiberties[n].clear();
  stonesInGroup[n].push_back(n);
  int int_colour = static_cast<int>(colour.value());
  currentHash ^= Zobrist::zobristTable[n][int_colour] ^ Zobrist::Bturn;

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
      groupLiberties[opponentParent].erase(n); // reduce liberties
      if (groupLiberties[opponentParent].size()==0){
        CaptureLogic(opponentParent);
      }
    }
  }
}

void Board::Pass(const std::optional<Stone>& Stone){
  currentHash^=Zobrist::Bturn;
}

uint64_t Board::PredictHash(const int& n, std::optional<Stone> colour){
  uint64_t nextHash = currentHash;
  int int_colour = static_cast<int>(colour.value());
  nextHash ^= Zobrist::zobristTable[n][int_colour] ^ Zobrist::Bturn;
  std::unordered_set<int> processedGroup;
  for(int neighbour:neighbours[n]){
    if(board[neighbour]!=std::nullopt && board[neighbour]!=colour){
      int opponentParent = Find(neighbour);
      if(groupLiberties[opponentParent].size()==1){
        if(processedGroup.count(opponentParent)==0){
          int int_oppColour = static_cast<int>(board[neighbour].value());
          for(int stone:stonesInGroup[opponentParent]){
            nextHash ^= Zobrist::zobristTable[stone][int_oppColour];
          }
          processedGroup.insert(opponentParent);
        }
      }
    }
  }
  return nextHash;
}