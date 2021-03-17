cmake_minimum_required(VERSION 3.12)
project(yuliana_vintyuk)

set(CMAKE_CXX_STANDARD 11)

include_directories(Common)

add_executable(server
        Server/main.cpp
        Common/processInfoCommon.cpp
        Common/communication.cpp
        )

add_executable(client
        Client/main.cpp
        Client/processInfoImpl.cpp
        Common/processInfoCommon.cpp
        Common/communication.cpp)
