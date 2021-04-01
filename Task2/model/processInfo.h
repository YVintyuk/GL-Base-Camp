#pragma once

#include <cstddef>
#include <string>
#include <list>

class processInfo {
private:
    size_t usedMemory;
    size_t usedCPU;
    std::string processName;

public:
    size_t getUsedMemory();
    size_t getUsedCPU();
    processInfo(pid_t) { };
    std::string getProcessName();
};

