#pragma once

#include <cstddef>
#include <vector>
#include <ostream>

#define USER_SIZE           255
#define EXE_NAME_SIZE       255
#define COMMAND_LINE_SIZE   4096

#ifdef __linux__
#define PID_TYPE size_t
#else
#define PID_TYPE unsigned long
#endif

/**
 * Structure to store
 */
struct processInfo_t {
    PID_TYPE pid;
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

