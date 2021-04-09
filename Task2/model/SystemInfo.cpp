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

size_t SystemInfo::getFreeMemory() {
    return freeMemory;
}

std::vector<ProcessInfo> SystemInfo::getProcessInfo() {
    return processInfoVector;
}

void SystemInfo::init() {
    start = std::chrono::system_clock::now();
    processInfoVector = readProcessListFromSystem();
    freeMemory = readFreeMemoryFromSystem();
}
