cmake_minimum_required(VERSION 3.12)
project(yuliana_vintyuk)

set(CMAKE_CXX_STANDARD 11)

include_directories(Common)

add_executable(server
        Server/main.cpp
        Common/processInfoCommon.cpp
        Common/communication.cpp)

add_executable(client
        Client/main.cpp
        Client/processInfoImpl.cpp
        Common/processInfoCommon.cpp
        Common/communication.cpp)

################################
# GTest
################################
ADD_SUBDIRECTORY (googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp file
add_executable( testProcessInfo Tests/testProcessInfo.cpp )
# Link test executable against gtest & gtest_main
target_link_libraries(testProcessInfo gtest gtest_main)
add_test( testProcessInfo testProcessInfo )

