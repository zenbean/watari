#pragma once
#include <vector>
#include <string>
#include "common.hpp"

struct SGF{
    Stone colour;
    int n;
};

std::vector<SGF> ParseSGF(const std::string& filepath, int boardSize);