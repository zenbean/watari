#include "gtp.hpp"
#include "board.hpp"
#include <iostream>
#include <sstream>

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
    ss>>command;
    if (line == "protocol_version") SendResponse("2");
    else if (command == "name") SendResponse("Watari");
    else if (command == "version") SendResponse("0.1");
    else if (command == "boardsize"){
        ss >> currentSize;
        board = Board(currentSize);
        SendResponse("");
    }
    else if (line == "clear_board"){
        Board board(9);
        SendResponse("");
    }
    else {
        SendError("unknown command");
    }
}

void GTP::ProcessPosition(const std::string& line){
    std::stringstream ss(line);
    std::string command;
    std::string colour;
    std::string pos;
    ss >> command;
    if (command == "play"){
        if(pos=="pass"){
            return;
        }
        ss >> colour >> pos;
        Stone stone_colour;
        if(colour=="b" || colour == "B"){
            stone_colour = Stone::BLACK;
        }
        else {
            stone_colour = Stone::WHITE;
        }
        int x = std::toupper(pos[0]) - 'A';
        if (std::toupper(pos[0]) > 'I') x--;
        int y = currentSize - std::stoi(pos.substr(1));
        int index = board.CoordinateToIndex(x,y);
        board.PlayStone(index, stone_colour);
        SendResponse("");
    }
}