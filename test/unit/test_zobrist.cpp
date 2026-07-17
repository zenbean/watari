#include <gtest/gtest.h>
#include <cstdlib>
#include "common.hpp"
#include "board.hpp"
#include "zobrist.hpp"

TEST(ZobristUnit, DifferentHash){
    Board board(9);
    EXPECT_EQ(board.GetCurrentHash(), 0);
    board.PlayStone(0, Stone::BLACK);
    EXPECT_NE(board.GetCurrentHash(), 0);
}

TEST(ZobristUnit, SameHash){
    Board board(9);
    EXPECT_EQ(board.GetCurrentHash(), 0);
    int blackPos0 = board.CoordinateToIndex(0, 1);
    int blackPos1 = board.CoordinateToIndex(1,0);
    int blackPos2 = board.CoordinateToIndex(2, 1);
    int blackPos3 = board.CoordinateToIndex(1, 2);
    int whitePos0 = board.CoordinateToIndex(2, 0);
    int whitePos1 = board.CoordinateToIndex(3, 1);
    int whitePos2 = board.CoordinateToIndex(2, 2);
    int whitePos3 = board.CoordinateToIndex(1, 1);
    board.PlayStone(blackPos0, Stone::BLACK);
    board.PlayStone(whitePos0, Stone::WHITE);
    board.PlayStone(blackPos1, Stone::BLACK);
    board.PlayStone(whitePos1, Stone::WHITE);
    board.PlayStone(blackPos2, Stone::BLACK);
    board.PlayStone(whitePos2, Stone::WHITE);
    board.PlayStone(blackPos3, Stone::BLACK);
    // 1
    uint64_t state0 = board.GetCurrentHash();
    EXPECT_NE(board.GetCurrentHash(), 0);
    // 2
    board.PlayStone(whitePos3, Stone::WHITE);
    uint64_t state1 = board.GetCurrentHash();
    EXPECT_NE(board.GetCurrentHash(), state0);
    // 3
    board.PlayStone(blackPos2, Stone::BLACK);
    EXPECT_EQ(board.GetCurrentHash(), state0);
}

TEST(BoardUnit, PassMoveMechanics) {
    Board board(13);
    uint64_t initial = board.GetCurrentHash();
    
    board.Pass(Stone::BLACK); 
    uint64_t pass0 = board.GetCurrentHash();
    EXPECT_NE(initial, pass0);
    
    board.Pass(Stone::WHITE);
    uint64_t pass1 = board.GetCurrentHash();
    EXPECT_EQ(initial, pass1);
    
    for(int i = 0; i < 81; i++) {
        EXPECT_EQ(board.GetStone(i), std::nullopt);
    }
}