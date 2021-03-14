#pragma once

#include <cstddef>
#include <vector>
#include <ostream>

#define USER_SIZE 255
#define EXE_NAME_SIZE 255
#define COMMAND_LINE_SIZE 4096

#define PORT 10000

/**
 * Structure to store
 */
struct processInfo_t {
    size_t pid;
    char user [USER_SIZE];
    char exeName [EXE_NAME_SIZE];
    char commandLine [COMMAND_LINE_SIZE];

    /**
    * Print process info with streams
    * @param os
    * @return
    */
    friend std::ostream&operator<<(std::ostream& os, const processInfo_t& p);
};

