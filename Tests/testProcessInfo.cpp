#include <string>
#include <gtest/gtest.h>
#include <processInfoCommon.h>

// #include "serverHelper.h"

// FIXME: in reality should not be a copy, should have utility library
bool hasEnding(std::string const& fullString, std::string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

#ifdef __linux__
#define EXE
#else
#define EXE ".exe"
#endif
PID_TYPE findProcessforKilling(const std::vector<processInfo_t>& processInfoVector) {
    for (const auto& p : processInfoVector) {
        if (hasEnding(p.exeName, "sleep" EXE)) {
            return p.pid;
        }
    }
    return 0;
}

TEST(testProcessInfo, testHasEnding) {
    ASSERT_TRUE(hasEnding("C:/path/to/sleep.exe", "sleep.exe"));
    ASSERT_FALSE(hasEnding("C:/path/to/other.exe", "sleep.exe"));

    ASSERT_TRUE(hasEnding("my_string_ends_with", "ends_with"));
    ASSERT_FALSE(hasEnding("some_other_string_without_ends_with1", "ends_with"));
}

TEST(testProcessInfo, testFindProcess) {
    // Set up
    PID_TYPE expected_pid = 123;
    std::vector<processInfo_t> data;

    //TODO: add some processes to the data vector

    // Execution
    auto pid = findProcessforKilling(data);

    ASSERT_EQ(pid, expected_pid);
}