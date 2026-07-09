#include <gtest/gtest.h>
#include "board.hpp"
#include "common.hpp"

TEST(CaptureMechanic, SingleStoneCapture) {
    Board board(9);
    int blackPos = board.CoordinateToIndex(0,0);
    int whitePos0 = board.CoordinateToIndex(1,0);
    int whitePos1 = board.CoordinateToIndex(0,1);

    board.PlayStone(blackPos, Stone::BLACK);
    board.PlayStone(whitePos0, Stone::WHITE);
    board.PlayStone(whitePos1, Stone::WHITE);

    EXPECT_EQ(board.GetStone(blackPos), std::nullopt);
    EXPECT_EQ(board.GetLibertiesCount(whitePos0), 3); 
    EXPECT_EQ(board.GetLibertiesCount(whitePos1), 3);
}

TEST(CaptureMechanic, LargeGroupCapture) {
    Board board(9);
    int blackPos0 = board.CoordinateToIndex(0,0);
    int blackPos1 = board.CoordinateToIndex(1,0);
    int whitePos0 = board.CoordinateToIndex(2,0);
    int whitePos1 = board.CoordinateToIndex(1,1);
    int whitePos2 = board.CoordinateToIndex(0,1);

    board.PlayStone(blackPos0, Stone::BLACK);
    board.PlayStone(blackPos1, Stone::BLACK);
    board.PlayStone(whitePos0, Stone::WHITE);
    board.PlayStone(whitePos1, Stone::WHITE);
    board.PlayStone(whitePos2, Stone::WHITE);

    EXPECT_EQ(board.GetStone(blackPos0), std::nullopt);
    EXPECT_EQ(board.GetStone(blackPos1), std::nullopt);
    EXPECT_GT(board.GetLibertiesCount(whitePos0), 3); 
    EXPECT_GT(board.GetLibertiesCount(whitePos1), 5); 
    EXPECT_GT(board.GetLibertiesCount(whitePos2), 5); 

}
