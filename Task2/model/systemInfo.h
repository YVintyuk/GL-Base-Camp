#pragma once

#include <vector>
#include "model/processInfo.h"

/*
 getFreeMemory(): size_t
 getUsedMemory(): size_t
 getUsedCPU(): size_t
 getProcessInfo(): vector<processInfo>
 */

class systemInfo {
private:
    std::vector <processInfo> processInfoVector;
    size_t usedMemory;
    size_t usedCPU;
    size_t freeMemory;

public:
    size_t getFreeMemory();
    size_t getUsedMemory();
    size_t getUsedCPU();
    std::vector <processInfo> getProcessInfo();
};

