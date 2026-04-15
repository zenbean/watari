#include "common.hpp"
#include <vector>
#include <unordered_set>
#include <optional>

class Board{
    private:
        int boardSize;
        std::vector<std::optional<Stone>> board;
        std::vector<std::vector<int>> neighbours;
        std::unordered_set<long long> previousBoardHash;

        void FindLiberties(const int& n, std::optional<Stone> colour, std::unordered_set<int>& liberties, std::unordered_set<int>& visitedPositions);
    public:
        Board(int boardSize): boardSize(boardSize), board(boardSize*boardSize, std::nullopt), neighbours(boardSize*boardSize){};
        int CoordinateToIndex(const int& x, const int& y);
        int CountLiberties(const int& n);
        bool ValidMove(const int& n, std::optional<Stone> colour);
        void PlayStone(const int& n, std::optional<Stone> colour);
};