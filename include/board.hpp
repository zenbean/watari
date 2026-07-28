#pragma once
#include "common.hpp"
#include <vector>
#include <unordered_set>
#include <optional>
#include <cstdint>

class Board{
    private:
        std::vector<std::optional<Stone>> board;
        std::vector<std::vector<int>> neighbours;
        int boardSize;

        std::vector<int> parent; // store parent value of each position index
        std::vector<std::unordered_set<int>> groupLiberties; // track liberties for each group
        std::vector<std::vector<int>> stonesInGroup; // track stones in each group

        uint64_t currentHash = 0; // track board state
        std::unordered_set<uint64_t> hashRecord;

        int Find(const int& x);
        void Union(const int&x, const int& y);
    public:
        Board(int boardSize);
        int CoordinateToIndex(const int& x, const int& y);
        bool ValidMove(const int& n, std::optional<Stone> colour);
        void PlayStone(const int& n, std::optional<Stone> colour);
        void CaptureLogic(const int&);
        void Pass(const std::optional<Stone>& colour);
        uint64_t PredictHash(const int& n, std::optional<Stone> colour);

        // for tests
        int GetParent(int x) { return Find(x); }
        int GetGroupSize(int x) { return stonesInGroup[Find(x)].size(); }
        int GetLibertiesCount(int x) { return groupLiberties[Find(x)].size(); }
        std::optional<Stone> GetStone(int x) { return board[x]; }
        uint64_t GetCurrentHash(){return currentHash;}
};
