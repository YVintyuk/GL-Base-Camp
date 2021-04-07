#include "ProcessInfo.h"
#include "model_view/Client.h"
#include "model_view/Runnable.h"

/*
 usedMemory: size_t
 usedCPU: size_t
 processInfoVector: vector<processInfo>
 freeMemory: size_t

getFreeMemory(): size_t
 getUsedMemory(): size_t
 getUsedCPU(): size_t
 getProcessInfo(): vector<processInfo>
 */
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
