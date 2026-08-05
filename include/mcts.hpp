#pragma once
#include <vector>
#include <memory>
#include <optional>
#include "common.hpp"
#include "board.hpp"
#include <atomic>

struct RootParallelism{
    int moveIndex;
    int visits;
};

class Node{
    friend class MCTS;
    private:
        Node* parent; 
        std::vector<std::unique_ptr<Node>> children;
        int moveIndex; // current move
        std::optional<Stone> nextColour;
        std::vector<int> legalMoves;
        int visited;
        double reward;
    public:
        Node(Node* parentNode, int move, std::optional<Stone> colour, Board& currentBoard);
};

class MCTS{
    private:
        static constexpr int searchIterations = 800;
        static constexpr int numThreads = 2;
        static constexpr int NUMBER_OF_MOVES = 82;
        const std::optional <Stone> engineColour;
        double Simulate(Node* &node, const double& komi, Board board);
        std::vector<int> GenerateLegalMoves(std::optional<Stone> currentColour, Board& board);
        double AreaScoring(const double& komi, Board& board);
        void SearchThread(int iterations, const double& komi, Board& board, std::array<std::atomic<int>, NUMBER_OF_MOVES>& global_visits);
    public:
        MCTS(std::optional<Stone> colour):engineColour{colour}{};
        int ParallelSearch(int move, double komi, Board& board);
};