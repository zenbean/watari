#include <gtest/gtest.h>
#include "gtp.hpp"
#include "board.hpp"

TEST(GtpUnit, NameCommand) {
    GTP gtp;
    testing::internal::CaptureStdout();
    gtp.ProcessLine("name");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "= Watari\n\n");
}

TEST(GtpUnit, PlayCommand) {
    GTP gtp;
    testing::internal::CaptureStdout();
    gtp.ProcessPosition("play b D3");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "= \n\n");
}