#pragma once
#include "board.hpp"
#include <string>

class GTP{
    private:
        Board board;
        int currentSize;
        void SendResponse(const std::string& response);   
        void SendError(const std::string& error);
    public:
        GTP();
        void ProcessLine(const std::string& line);
        void ProcessPosition(const std::string& line);
};