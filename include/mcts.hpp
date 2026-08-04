#pragma once
#include <vector>
#include <memory>
#include <optional>
#include "common.hpp"
#include "board.hpp"

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
        int search_iterations = 500;
        std::optional<Stone> engineColour;
        std::optional<Stone> opponentColour;
        double Simulate(Node* &node, const double& komi, Board board);
        std::vector<int> GenerateLegalMoves(std::optional<Stone> currentColour, Board& board);
        double AreaScoring(const double& komi, Board& board);
    public:
        int Search(int move, std::optional<Stone> colour, const double& komi, Board& board);
};