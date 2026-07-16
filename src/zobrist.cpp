#include <iostream>
#include <random>
#include "zobrist.hpp"

uint64_t Zobrist::zobristTable[19*19][2];
void Zobrist::Init(){
    // std::random_device rd; // seed source for random number engine
    std::mt19937_64 gen(2565398);
    std::uniform_int_distribution<uint64_t> dist;
    for (int i=0; i < 19 * 19; i++){
        zobristTable[i][0]=dist(gen); // random no. for black stone at index i
        zobristTable[i][1]=dist(gen); // random no. for white stone at index i
    }
}