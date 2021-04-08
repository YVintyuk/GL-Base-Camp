#pragma once

#include <vector>
#include "ProcessInfo.h"
#include <dirent.h>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <sys/sysctl.h>
#include <sys/sysinfo.h>
#include <chrono>
#include <iostream>

class SystemInfo {
private:
    std::vector <ProcessInfo> processInfoVector;
    size_t usedMemory;
    size_t usedCPU;
    size_t freeMemory;
    size_t readFreeMemoryFromSystem();

    std::vector <ProcessInfo> readProcessListFromSystem();

public:
    friend std::ostream& operator<<(std::ostream& os, const SystemInfo& dt);
    std::chrono::time_point <std::chrono::system_clock> start;
    size_t getFreeMemory();;

    size_t getUsedMemory();
    size_t getUsedCPU();
    std::vector <ProcessInfo> getProcessInfo();
    SystemInfo();
};

