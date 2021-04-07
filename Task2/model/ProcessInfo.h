#pragma once

#include <cstddef>
#include <string>
#include <list>
#include <iostream>

class ProcessInfo {
private:
    size_t usedMemory;
    size_t usedCPU;
    std::string processName;
    pid_t pid;

public:
    size_t getUsedMemory();
    size_t getUsedCPU();
    ProcessInfo(pid_t _pid) {
        pid = _pid;
    };
    std::string getProcessName();
    friend std::ostream& operator<<(std::ostream& os, const ProcessInfo& dt);
};

