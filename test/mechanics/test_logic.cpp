#include <gtest/gtest.h>
#include "board.hpp"
#include "sgf.hpp"

TEST(EngineStressTest, ParseAndPlayT01) {
    int boardSize = 19;
    Board board(boardSize);
    std::vector<SGF> moves = ParseSGF("../test_data/T01.sgf", boardSize);
    
    ASSERT_EQ(moves.size(), 146) << "Parser failed to extract all SGF";

    for (size_t i = 0; i < moves.size(); ++i) {
        const SGF& move = moves[i];
        if (move.n == -1) {
            board.Pass(move.colour);
            continue;
        }
        // if DSU or Zobrist Ko logic fails, stop the test. all moves should be legal
        ASSERT_TRUE(board.ValidMove(move.n, move.colour)) 
            << "Engine rejected move index " << i << " at board position " << move.n;
        board.PlayStone(move.n, move.colour);
    }
    SUCCEED(); 
}