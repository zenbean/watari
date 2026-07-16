#pragma once
#include <cstdint>

class Zobrist{
    public:
        static uint64_t zobristTable[19*19][2]; // max board size
        static void Init();
};