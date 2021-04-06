#pragma once

#include <vector>
#include "ProcessInfo.h"
#include <dirent.h>
#include <cstring>
#include <algorithm>
#include <stdexcept>

class SystemInfo {
private:
    std::vector <ProcessInfo> processInfoVector;
    size_t usedMemory;
    size_t usedCPU;
    size_t freeMemory;
    std::vector <ProcessInfo> getProcessInfoVector() {
        std::vector <ProcessInfo> ret;
        DIR* dir;
        if(!(dir = opendir("/proc"))) {
            throw std::runtime_error(std::strerror(errno));
        }
        while(dirent* dirp = readdir(dir))
        {
            if(dirp->d_type != DT_DIR) {
                continue;
            }
            if(!std::all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name),
                            [](char c){ return std::isdigit(c); })) {
                continue;
            }
            ret.push_back(ProcessInfo(atoi(dirp->d_name)));
        }
        if(closedir(dir)) {
            throw std::runtime_error(std::strerror(errno));
        }
        return ret;
    }

public:
    size_t getFreeMemory() {
            return freeMemory;
    };

    size_t getUsedMemory();
    size_t getUsedCPU();
    std::vector <ProcessInfo> getProcessInfo() {
        return processInfoVector;
    }
    SystemInfo() {
        processInfoVector = getProcessInfoVector();
        freeMemory = 42;
    }
};

