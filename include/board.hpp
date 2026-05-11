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
        int boardSize;
        std::vector<std::optional<Stone>> board;
        std::vector<std::vector<int>> neighbours;
        std::unordered_set<long long> previousBoard;

        void ExploreBoard(const int&n, const std::optional<Stone>& colour, BoardInfo& boardInfo, std::vector<char>& visitedPositions);
        bool ValidMove(const int& n);
        std::vector<std::pair<int, std::optional<Stone>>> SimulateMove(const int& n, std::optional<Stone> colour);
    public:
        Board(int boardSize): boardSize(boardSize), board(boardSize*boardSize, std::nullopt), neighbours(boardSize*boardSize){};
        int CoordinateToIndex(const int& x, const int& y);
        BoardInfo GetInfo(const int& n);
        void PlayStone(const int& n, std::optional<Stone> colour);
};