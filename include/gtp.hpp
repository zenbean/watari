#pragma once
#include "board.hpp"
#include <string>

class GTP{
    private:
        Board board;
        int currentSize;
        void SendResponse(const std::string& response);   
        void SendError(const std::string& error);
        void Play(std::istringstream& ss);
        void GenMove(std::istringstream& ss, const double& komi);
        void ListCommands();
    public:
        GTP();
        void ProcessLine(const std::string& line);
};