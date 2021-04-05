#pragma once

#include <cstddef>
#include <string>
#include <list>

class ProcessInfo {
private:
    size_t usedMemory;
    size_t usedCPU;
    std::string processName;

public:
    size_t getUsedMemory();
    size_t getUsedCPU();
    ProcessInfo(pid_t) { };
    std::string getProcessName();
};

