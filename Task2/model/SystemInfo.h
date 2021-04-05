#pragma once

#include <vector>
#include "model/ProcessInfo.h"

class SystemInfo {
private:
    std::vector <ProcessInfo> processInfoVector;
    size_t usedMemory;
    size_t usedCPU;
    size_t freeMemory;

public:
    size_t getFreeMemory();
    size_t getUsedMemory();
    size_t getUsedCPU();
    std::vector <ProcessInfo> getProcessInfo();
};

