#pragma once

#include <cstddef>
#include <vector>

#define USER_SIZE 255
#define COMMAND_SIZE 255
#define ARGV_SIZE 4096

struct processInfo_t {
    size_t pid;
    char user [USER_SIZE];
    char command [COMMAND_SIZE];
    char argv [ARGV_SIZE];
};

std::vector <processInfo_t> getProcessInfoVector();
