#pragma once
#include <cstdint>

class Zobrist{
    public:
        static uint64_t zobristTable[19*19][2]; // max board size
        static uint64_t Bturn; // random gen bitstring for one colour's turn
        static bool initialised;
        static void Init();
};