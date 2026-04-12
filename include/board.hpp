#include "common.hpp"
#include <vector>
#include <unordered_set>
#include <optional>

class Board{
    private:
        int boardSize;
        std::vector<std::optional<Stone>> board;
        std::vector<std::vector<int>> neighbours;
        std::vector<int> groupIDs;
        std::unordered_set<int> libertyPosition;
        std::unordered_set<long long> previousBoardHash;
    public:
        Board(int boardSize): boardSize(boardSize), board(boardSize*boardSize, std::nullopt), neighbours(boardSize*boardSize){}
        int CoordinateToIndex(const int& x, const int& y);
        int CountLiberties(const int& x, const int& y, const Stone& colour);
};"