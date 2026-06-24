#pragma once
#include "common.hpp"
#include <vector>
#include <unordered_set>
#include <optional>

typedef struct BoardInfo{
    std::unordered_set<int> liberties;
    std::unordered_set<int> groupPositions;
} BoardInfo;

class Board{
    private:
        std::vector<std::optional<Stone>> board;
        std::vector<std::vector<int>> neighbours;
        int boardSize;

        std::vector<int> parent; // store parent value of each position index
        std::vector<std::unordered_set<int>> groupLiberties; // track liberties for each group
        std::vector<std::vector<int>> stonesInGroup; // track stones in each group

        int Find(const int& x);
        void Union(const int&x, const int& y);
    public:
        Board(int boardSize);
        int CoordinateToIndex(const int& x, const int& y);
        bool ValidMove(const int& n, std::optional<Stone> colour);
        void PlayStone(const int& n, std::optional<Stone> colour);

        // for tests
        int GetParent(int x) { return Find(x); }
        int GetGroupSize(int x) { return stonesInGroup[Find(x)].size(); }
        int GetLibertiesCount(int x) { return groupLiberties[Find(x)].size(); }
        std::optional<Stone> GetStone(int x) { return board[x]; }
};
