#include "SystemInfo.h"
#include "ProcessInfo.h"
#include "model_view/Client.h"
#include "model_view/Runnable.h"

std::ostream& operator<<(std::ostream& os, const SystemInfo& systemInfo )
{
    std::time_t start_time = std::chrono::system_clock::to_time_t(systemInfo.start);
    os << "System info:\n"
       << "Time: " << std::ctime(&start_time) << "\n"
       << "Process count: " << systemInfo.processInfoVector.size() << "\n"
       << "Free memory: " << systemInfo.freeMemory << "\n";
    for (const auto &process:systemInfo.processInfoVector) {
        os << process;
    }
    return os;
}

size_t SystemInfo::readFreeMemoryFromSystem() {
    struct sysinfo info;

    if (sysinfo(&info) < 0)
        return 0;

    return info.freeram;
}

std::vector<ProcessInfo> SystemInfo::readProcessListFromSystem() {
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

size_t SystemInfo::getFreeMemory() {
    return freeMemory;
}

std::vector<ProcessInfo> SystemInfo::getProcessInfo() {
    return processInfoVector;
}

SystemInfo::SystemInfo() {
    start = std::chrono::system_clock::now();
    processInfoVector = readProcessListFromSystem();
    freeMemory = readFreeMemoryFromSystem();
}
