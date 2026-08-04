#include <gtest/gtest.h>
#include "mcts.hpp"

TEST(AreaScoringTest, EmptyBoard) {
    Board board(5);
    MCTS mcts;
    mcts.engineColour = Stone::BLACK;
    double score = mcts.AreaScoring(7.5, board);
    // Black:0. White: 7.5 komi
    EXPECT_DOUBLE_EQ(score, 0.0);
}

TEST(AreaScoringTest, BlackStoneArea) {
    Board board(5);
    MCTS mcts;
    mcts.engineColour = Stone::BLACK;
    for (int y = 0; y < 5; y++) {
        board.PlayStone(board.CoordinateToIndex(2, y), Stone::BLACK);
    }
    double score = mcts.AreaScoring(7.5, board);
    // Black: 25 points. White: 7.5 komi
    EXPECT_DOUBLE_EQ(score, 1.0);
}

TEST(AreaScoringTest, NeutralArea) {
    Board board(5);
    MCTS mcts;
    mcts.engineColour = Stone::BLACK;
    for (int y = 0; y < 5; y++) {
        board.PlayStone(board.CoordinateToIndex(1, y), Stone::BLACK);
    }
    for (int y = 0; y < 5; y++) {
        board.PlayStone(board.CoordinateToIndex(3, y), Stone::WHITE);
    }
    double score = mcts.AreaScoring(7.5, board);
    // Black: 10 points. White: 10 points + 7.5 komi
    EXPECT_DOUBLE_EQ(score, 0.0);
}