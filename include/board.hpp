#include "common.hpp"
#include <vector>
#include <unordered_set>
#include <optional>

class Board{
    private:
        int boardSize;
        std::vector<std::optional<Stone>> board;
        std::vector<std::vector<int>> neighbours;
        std::unordered_set<long long> previousBoard;

        void FindLiberties(const int& n, const std::optional<Stone>& colour, std::unordered_set<int>& liberties, std::unordered_set<int>& visitedPositions, std::vector<std::optional<Stone>>& local_board);
        void FindGroup(const int&n, const std::optional<Stone>& colour, std::unordered_set<int>& groupPositions, std::unordered_set<int>& visitedPositions, std::vector<std::optional<Stone>>& local_board);
        bool ValidMove(const int& n, std::vector<std::optional<Stone>>& local_board);
        std::vector<std::optional<Stone>> SimulateMove(const int& n, std::optional<Stone> colour, std::vector<std::optional<Stone>> local_board);
    public:
        Board(int boardSize): boardSize(boardSize), board(boardSize*boardSize, std::nullopt), neighbours(boardSize*boardSize){};
        int CoordinateToIndex(const int& x, const int& y);
        int CountLiberties(const int& n, std::vector<std::optional<Stone>>& local_board);
        std::unordered_set<int> GetGroup(const int& n, std::vector<std::optional<Stone>>& local_board);
        void PlayStone(const int& n, std::optional<Stone> colour);
};