#include "utils/processinfo.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("ProcessInfoTest::IsProcessRunning")
{
    CHECK(ProcessInfo::isProcessRunning("test_common_catch"));
    CHECK_FALSE(ProcessInfo::isProcessRunning("this_is_not_running"));
}
