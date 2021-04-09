#pragma once

#include <vector>
#include <chrono>
#include <iostream>
#include <memory>

#include "ProcessInfo.h"

class SystemInfo {
private:
    std::vector <std::shared_ptr<ProcessInfo>> processInfoVector;
    size_t usedMemory;
    size_t usedCPU;
    size_t freeMemory;
protected:
    virtual size_t readFreeMemoryFromSystem() = 0;
    virtual std::vector <std::shared_ptr<ProcessInfo>> readProcessListFromSystem() = 0;

public:
    friend std::ostream& operator<<(std::ostream& os, const SystemInfo& dt);
    std::chrono::time_point <std::chrono::system_clock> start;
    size_t getFreeMemory();;

    size_t getUsedMemory();
    size_t getUsedCPU();
    std::vector <std::shared_ptr<ProcessInfo>> getProcessInfo();
    void init();
};

