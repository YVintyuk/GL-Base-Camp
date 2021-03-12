cmake_minimum_required(VERSION 3.12)
project(yuliana_vintyuk)

set(CMAKE_CXX_STANDARD 11)

add_executable(yuliana_vintyuk main.cpp)

add_executable(server Server/main.cpp)

add_executable(client Client/main.cpp Client/processInfo.cpp Client/processInfo.h)