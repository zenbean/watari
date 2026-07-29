#include <gtest/gtest.h>
#include "gtp.hpp"
#include "board.hpp"
#include <regex>

TEST(GtpMechanisms, NameCommand) {
    GTP gtp;
    testing::internal::CaptureStdout();
    gtp.ProcessLine("name");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "= Watari\n\n");
}

TEST(GTPMechanisms, CoordinateTranslation) {
    GTP gtp;
    
    testing::internal::CaptureStdout();
    gtp.ProcessLine("boardsize 19");
    std::string out1 = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out1, "= \n\n");
    
    testing::internal::CaptureStdout();
    gtp.ProcessLine("play B K10");
    std::string out2 = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out2, "= \n\n");
    
    testing::internal::CaptureStdout();
    gtp.ProcessLine("play w a19");
    std::string out3 = testing::internal::GetCapturedStdout();
    EXPECT_EQ(out3, "= \n\n");
}


TEST(GTPMechanisms, GenMoveOutputFormatting) {
    GTP gtp;
    testing::internal::CaptureStdout();
    gtp.ProcessLine("genmove B");
    std::string output = testing::internal::GetCapturedStdout();
    std::regex gtp_regex("^= ([A-HJ-Z][1-9][0-9]?|pass)\n\n$");
    EXPECT_TRUE(std::regex_match(output, gtp_regex)); 
}