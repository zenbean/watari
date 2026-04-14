#include "common.hpp"
#include <vector>
#include <unordered_set>
#include <optional>

class Board{
    private:
        int boardSize;
        std::vector<std::optional<Stone>> board;
        std::vector<std::vector<int>> neighbours;
        std::unordered_set<int> visitedPositions;

        std::unordered_set<long long> previousBoardHash;
        int CountLiberties(const int& x, const int& y);
    public:
        Board(int boardSize): boardSize(boardSize), board(boardSize*boardSize, std::nullopt), neighbours(boardSize*boardSize){}
        int CoordinateToIndex(const int& x, const int& y);
};