#include "common.hpp"
#include <vector>
#include <unordered_set>
#include <optional>

class Board{
    private:
        int boardSize;
        std::vector<std::optional<Stone>> board;
        std::unordered_set<long long> previousBoardHash;
    public:
    Board(int boardSize): boardSize(boardSize), board(boardSize*boardSize, std::nullopt){}
    int CoordinateToIndex(const int& x, const int& y);
    int CountLiberties(const int& x, const int& y);
};