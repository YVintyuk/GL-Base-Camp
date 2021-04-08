#include "ProcessInfo.h"
#include "SystemInfo.h"
#include "model_view/Runnable.h"
#include "model_view/Client.h"
#include <dirent.h>
#include <cstring>
#include <algorithm>

std::ostream& operator<<(std::ostream& os, const ProcessInfo& process) {
    os << "Process info: \n"
       << "Process id: " << process.pid << "\n";
}

ProcessInfo::ProcessInfo(pid_t _pid) {
    pid = _pid;
};

/**
 * open "/proc"
 * read all files in proc one by one
 * check is it directory
 * is every character of the name a digit?
 * add info about process to list of process
 * @return
 */
std::vector <ProcessInfo> getProcessInfoVector() {
    std::vector <ProcessInfo> ret;
#ifdef __linux__
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
#else
    // Get the list of process identifiers.
    DWORD aProcesses[MAX_PROCESS_COUNT], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
    {
        return ret;
    }

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    for (i = 0; i < cProcesses; i++)
    {
        if (aProcesses[i] != 0)
        {
            ret.push_back(getProcessInfo(aProcesses[i]));
        }
    }
#endif
    return ret;
}