#include "mcts.hpp"
#include <cmath>
#include <random>
#include <queue>
#include <thread>

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

void MCTS::SearchThread(int iterations, const double& komi, Board& board, std::array<std::atomic<int>, NUMBER_OF_MOVES>& global_visits){
    std::optional<Stone> opponentColour = (engineColour == Stone::BLACK)?Stone::WHITE:Stone::BLACK;
    Node root(nullptr, -1, opponentColour, board); // root node
    for (int i=0; i<iterations; i++){
        Node* current = &root;
        Board tempBoard = board;
        int turn = 0;

        // select
        while(current->legalMoves.empty()&&!current->children.empty()){
            // calculate and store uct value of each child
            std::vector<double> uct_values;
            double maxVal = -1;
            int childIndex=0;
            double C = sqrt(2);
            double log_Ns=log(current->visited);
            for (auto& child:current->children){
                double Q_sa = (child->reward / child->visited);
                double N_sa=child->visited;
                double uct = Q_sa + ( C * sqrt( log_Ns / N_sa ) );
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
            int expandMove = current->legalMoves.back();
            current->legalMoves.pop_back();
            std::optional<Stone> expandColour = current->nextColour;

            if (expandMove == -1) {
                tempBoard.Pass(expandColour);
            } else {
                tempBoard.PlayStone(expandMove, expandColour);
            }
            current->children.push_back(std::make_unique<Node>(current, expandMove, expandColour, tempBoard));
            current = current->children.back().get();
        }

        //simulate
        double simReward = Simulate(current, komi, tempBoard);

        //backpropagate
        while(current!=nullptr){
            current->reward+=simReward; 
            current->visited++;
            current = current->parent;
        }
    }
    int passIndex = board.GetBoardSize() * board.GetBoardSize();
    for(const auto& child:root.children){
        int index=child->moveIndex;
        if(index==-1){
            index = passIndex;
        }
        global_visits[index].fetch_add(child->visited, std::memory_order_relaxed);
    }
}

int MCTS::ParallelSearch(int move, double komi, Board& board){
    std::array<std::atomic<int>, NUMBER_OF_MOVES> global_visits;
    int threadIterations = searchIterations/numThreads;
    std::vector<std::thread> searchWorkers;
    for(auto& counter:global_visits){
        counter.store(0, std::memory_order_relaxed);
    }
    for (int i = 0; i < numThreads; i++){
        searchWorkers.emplace_back(&MCTS::SearchThread, this, threadIterations, komi, std::ref(board), std::ref(global_visits));
    }
    for (auto& worker:searchWorkers){
        worker.join();
    }

    // final action
    int bestMove = -1;
    int mostVisited = -1;
    int passIndex = board.GetBoardSize() * board.GetBoardSize();
    for(int i = 0; i <= passIndex; i++){
        int totalVisits = global_visits[i].load(std::memory_order_relaxed);
        if (totalVisits>mostVisited){
            bestMove=(i==passIndex)?-1:i;
            mostVisited=totalVisits;
        }
    }
    return bestMove;
}

// simulate on a temporary board
double MCTS::Simulate(Node* &node, const double& komi, Board board){
    static std::atomic<uint64_t> thread_seed{2565398};
    static thread_local std::mt19937_64 gen(thread_seed.fetch_add(1, std::memory_order_relaxed));
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