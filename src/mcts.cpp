#include "mcts.hpp"
#include <cmath>
#include <random>
#include <queue>

Node::Node(Node* parentNode, int move, std::optional<Stone> colour, Board& board){
    parent = parentNode;
    moveIndex = move;
    nextColour=(colour == Stone::BLACK)?Stone::WHITE:Stone::BLACK;
    for (int i = 0; i < board.GetBoardSize() * board.GetBoardSize(); i++) {
        if (board.ValidMove(i, nextColour)) {
            legalMoves.push_back(i);
        }
    }
    legalMoves.push_back(-1);
}

int MCTS::Search(int move, std::optional<Stone> colour, const double& komi, Board& board){
    engineColour = colour;
    opponentColour = (colour == Stone::BLACK)?Stone::WHITE:Stone::BLACK;
    Node root(nullptr, move, opponentColour, board); // root node
    for (int i=0; i<search_iterations; i++){
        Node* current = &root;
        Board tempBoard = board;
        int turn = 0;

        // select
        while(current->legalMoves.empty()&&!current->children.empty()){
            // calculate and store uct value of each child
            std::vector<double> uct_values;
            double maxVal = -1;
            int childIndex;
            for (auto& child:current->children){
                double Q_sa = (child->reward / child->visited);
                double C = sqrt(2);
                double N_s=current->visited;
                double N_sa=child->visited;
                double uct = Q_sa + ( C * sqrt( log( N_s ) / N_sa ) );
                uct_values.push_back(uct);
            }
            // find the node with maximum uct value
            for (int i = 0; i < uct_values.size(); i++){
                if (uct_values[i] > maxVal){
                    maxVal = uct_values[i];
                    childIndex = i;
                }
            }
            std::optional<Stone> selectColour = current->nextColour;
            current = current->children[childIndex].get();
            int move = current->moveIndex;
            if (move==-1){
                tempBoard.Pass(selectColour);
            }
            else{
                tempBoard.PlayStone(current->moveIndex, selectColour);
            }
        } 

        // expand
        if (!current->legalMoves.empty()){
            current->children.push_back(std::make_unique<Node>(current, current->legalMoves.back(), engineColour, board));
            current->legalMoves.pop_back();
            current = current->children.back().get();
        }

        //simulate
        double simReward = Simulate(current, komi, board);

        //backpropagate
        while(current!=nullptr){
            current->reward+=simReward; 
            current->visited++;
            current = current->parent;
        }
    }
    // final action
    int bestMove = -1;
    int mostVisited = -1;
    for(auto& child:root.children){
        if (child->visited>mostVisited){
            bestMove=child->moveIndex;
            mostVisited=child->visited;
        }
    }
    return bestMove;
}

// simulate on a temporary board
double MCTS::Simulate(Node* &node, const double& komi, Board board){
    std::mt19937_64 gen(2565398);
    int consecutivePasses = 0;
    std::optional<Stone> currentColour = node->nextColour;

    while(consecutivePasses<2){
        std::vector<int> currentLegalMoves = GenerateLegalMoves(currentColour, board);
        std::uniform_int_distribution<int> dist(0, currentLegalMoves.size()-1);
        auto randIndex = dist(gen);
        int simMove = currentLegalMoves[randIndex];
        if(simMove == -1){
            board.Pass(currentColour);
            consecutivePasses++;
        }
        else{
            board.PlayStone(simMove, currentColour);
            consecutivePasses = 0;
        }
        currentColour = (currentColour==Stone::BLACK)?Stone::WHITE:Stone::BLACK;
    }
    return AreaScoring(komi, board);
}

std::vector<int> MCTS::GenerateLegalMoves(std::optional<Stone> currentColour, Board& board){
    std::vector<int> legalMoves;
    for (int i = 0; i < board.GetBoardSize() * board.GetBoardSize(); i++) {
        if (board.ValidMove(i, currentColour)) {
            legalMoves.push_back(i);
        }
    }
    legalMoves.push_back(-1);
    return legalMoves;
}

double MCTS::AreaScoring(const double& komi, Board& board){
    int rowSize = board.GetBoardSize();
    int boardSize = rowSize*rowSize;
    std::vector<bool> visited(boardSize, false);
    double pointB = 0.0;
    double pointW = 0.0;
    for (int i = 0; i < boardSize; i++){
        if (visited[i]) continue;
        std::optional<Stone> colour = board.GetStone(i);
        if (colour==Stone::BLACK){
            pointB++;
            visited[i]=true;
        }
        else if (colour==Stone::WHITE){
            pointW++;
            visited[i]=true;
        }
        else {
            std::queue<int> empty;
            empty.push(i);
            visited[i] = true;
            int regionSize = 0;
            bool neighbourB = false;
            bool neighbourW = false;
            
            while(!empty.empty()){
                int current = empty.front();
                empty.pop();
                regionSize++;
                std::vector<int> neighbours;

                if(current-rowSize>=0){
                    neighbours.push_back(current-rowSize);
                }
                if(current+rowSize < boardSize){
                    neighbours.push_back(current+rowSize);
                }
                if(current%rowSize!=0){
                    neighbours.push_back(current-1);
                }
                if(current%rowSize!=(rowSize-1)){
                    neighbours.push_back(current+1);
                }
                for(int neighbour:neighbours){
                    std::optional<Stone> neighbourStone = board.GetStone(neighbour);
                    if(neighbourStone==Stone::BLACK){
                        neighbourB=true;
                    }
                    else if (neighbourStone==Stone::WHITE){
                        neighbourW=true;
                    }
                    else if (neighbourStone==std::nullopt&&!visited[neighbour]){
                        visited[neighbour]=true;
                        empty.push(neighbour);
                    }
                }
            }
            if(neighbourB && !neighbourW){
                pointB+=regionSize;
            }
            else if (neighbourW && !neighbourB){
                pointW+=regionSize;
            }
        }
    }
    pointW += komi;

    if (engineColour == Stone::BLACK) {
        if (pointB > pointW) return 1.0;
        else if (pointB == pointW) return 0.5;
        else return 0.0;
    } else {
        if (pointW > pointB) return 1.0;
        else if (pointB == pointW) return 0.5;
        else return 0.0;
    }
}