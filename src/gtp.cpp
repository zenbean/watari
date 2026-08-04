#include "gtp.hpp"
#include "mcts.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <random>

GTP::GTP():board(9), currentSize(9){}

void GTP::SendResponse(const std::string& response){
    std::cout << "= " << response << "\n\n";
}

void GTP::SendError(const std::string& error){
    std::cout << "? " << error << "\n\n";
}

void GTP::ProcessLine(const std::string& line){
    std::istringstream ss(line);
    std::string command;
    double komi;
    ss>>command;
    if (command == "protocol_version") SendResponse("2");
    else if (command == "name") SendResponse("Watari");
    else if (command == "version") SendResponse("0.1");
    else if (command == "boardsize"){
        ss >> currentSize;
        board = Board(currentSize);
        SendResponse("");
    }
    else if (command == "clear_board"){
        board = Board(currentSize);
        SendResponse("");
    }
    else if (command == "play"){
        GTP::Play(ss);
    }
    else if (command == "genmove"){
        GTP::GenMove(ss, komi);
    }
    else if (command == "undo"){
        SendError("cannot undo");
    }
    else if (command == "list_commands"){
        GTP::ListCommands();
    }
    else if (command == "komi"){
        SendResponse("");
        ss >> komi;
    }
    else {
        SendError("unknown command");
    }
}

void GTP::Play(std::istringstream& ss){
    std::string colour;
    std::string pos;
    ss >> colour >> pos;
    Stone stone_colour = (colour=="b" || colour == "B")?Stone::BLACK:Stone::WHITE;

    if(pos=="pass"){
        board.Pass(stone_colour);
        SendResponse("");
        return;
    }

    int x = std::toupper(pos[0]) - 'A';
    if (std::toupper(pos[0]) > 'I') x--;
    int y = currentSize - std::stoi(pos.substr(1));
    int index = board.CoordinateToIndex(x,y);
    board.PlayStone(index, stone_colour);
    SendResponse("");
}

int randomMove(std::vector<int> legalMoves){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, legalMoves.size() - 1);
    return legalMoves[dist(gen)];
}

void GTP::GenMove(std::istringstream& ss, const double& komi) {
    std::string colour;
    std::vector<int> legalMoves;
    ss >> colour;
    Stone stone_colour = (colour == "b" || colour == "B") ? Stone::BLACK : Stone::WHITE;

    for (int i = 0; i < currentSize * currentSize; i++) {
        if (board.ValidMove(i, stone_colour)) {
            legalMoves.push_back(i);
        }
    }
    legalMoves.push_back(-1);
    if (legalMoves.empty()) {
        board.Pass(stone_colour);
        SendResponse("pass");
        return;
    }
    MCTS algo;
    int chosen_index = algo.Search(-1, stone_colour, komi, board);
    board.PlayStone(chosen_index, stone_colour);
    if (chosen_index == -1){
        SendResponse("pass");
    }
    int x = chosen_index % currentSize;
    int y = chosen_index / currentSize;
    
    char x_char = 'A' + x;
    if (x_char >= 'I') {
        x_char++; // skip I
    }
    int sabaki_y = currentSize - y;
    std::string sabaki_coord = std::string(1, x_char) + std::to_string(sabaki_y);
    SendResponse(sabaki_coord);
}

void GTP::ListCommands(){
    std::vector<std::string> commands = {"name", "version","protocol_version","play","boardsize","clear_board","genmode","quit"};
    std::cout << "= ";
    for(std::string command : commands){
        std::cout << command << "\n";
    }
    std::cout << "\n";
}