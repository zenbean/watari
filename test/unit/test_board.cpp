#include <gtest/gtest.h>
#include "board.hpp"
#include "common.hpp"

TEST(BoardUnit, CoordinateMapping){
  Board board(9); // 9x9
  EXPECT_EQ(board.CoordinateToIndex(0, 0), 0);
  EXPECT_EQ(board.CoordinateToIndex(8, 8), 80);
  EXPECT_EQ(board.CoordinateToIndex(8, 0), 8);
  EXPECT_EQ(board.CoordinateToIndex(0, 1), 9);
}

TEST(BoardUnit, SingleMove){
  Board board(9);
  int playPos = board.CoordinateToIndex(4, 4);
  board.PlayStone(playPos, Stone::BLACK);

  EXPECT_EQ(board.GetStone(playPos), Stone::BLACK);
  EXPECT_EQ(board.GetParent(playPos), playPos);
  EXPECT_EQ(board.GetGroupSize(playPos), 1);
  EXPECT_EQ(board.GetLibertiesCount(playPos), 4);
}

TEST(BoardUnit, DS_UnionAndFind){
  Board board(9);
  int move1 = board.CoordinateToIndex(4, 4);
  int move2 = board.CoordinateToIndex(4, 5);
  board.PlayStone(move1, Stone::BLACK);
  board.PlayStone(move2, Stone::BLACK);

  EXPECT_EQ(board.GetParent(move1), board.GetParent(move2));
  EXPECT_EQ(board.GetGroupSize(move1), 2);
  EXPECT_EQ(board.GetGroupSize(move2), 2);
  EXPECT_EQ(board.GetLibertiesCount(move1), 6);
}
