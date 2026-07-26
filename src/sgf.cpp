#include "sgf.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

std::vector<SGF> ParseSGF(const std::string& filepath, int boardSize){
    std::vector<SGF> parsedMoves;
    std::fstream file(filepath);
    if (!file.is_open()){
        std::cerr << "failed to open SGF file" << std::endl;
        return parsedMoves;
    }
    else{
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string sgfData = buffer.str();
        std::regex moveRegex(";([BW])\\[([a-z]{0,2})\\]"); // look for ;B[xy] or ;W[xy]
        auto words_begin = std::sregex_iterator(sgfData.begin(), sgfData.end(), moveRegex);
        auto words_end = std::sregex_iterator();
        for (std::sregex_iterator i = words_begin; i != words_end; ++i)
        {
            std::smatch match = *i;
            Stone colour_move = (match[1].str()=="B") ? Stone::BLACK : Stone::WHITE;
            std::string coordinate = match[2].str();
            if (coordinate.empty()||(coordinate=="tt"&&boardSize<=19)){
                parsedMoves.push_back({colour_move, -1}); // passed move
                continue;
            }
            int x = coordinate[0] - 'a';
            int y = coordinate[1] - 'a';
            int n = y * boardSize + x;
            parsedMoves.push_back({colour_move, n});
        }
    }
    return parsedMoves;
}