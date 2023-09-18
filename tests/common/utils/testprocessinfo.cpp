#include "utils/processinfo.h"
#include <gtest/gtest.h>

TEST(ProcessInfoTest, IsProcessRunning)
{
    EXPECT_TRUE(ProcessInfo::isProcessRunning("test_common"));
    EXPECT_FALSE(ProcessInfo::isProcessRunning("this_is_not_running"));
}
