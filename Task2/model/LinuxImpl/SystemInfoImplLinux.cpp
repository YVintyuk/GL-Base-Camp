#include "SystemInfoImplLinux.h"
#include "ProcessInfoImplLinux.h"

#include <cstring>
#include <sys/sysctl.h>
#include <sys/sysinfo.h>
#include <stdexcept>
#include <dirent.h>
#include <vector>
#include <algorithm>


size_t SystemInfoImplLinux::readFreeMemoryFromSystem() {
    struct sysinfo info;

    if (sysinfo(&info) < 0)
        return 0;

    return info.freeram;
}


std::vector<std::shared_ptr<ProcessInfo>> SystemInfoImplLinux::readProcessListFromSystem() {
    std::vector <std::shared_ptr<ProcessInfo>> ret;
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
        std::shared_ptr<ProcessInfo> process = std::make_shared<ProcessInfoImplLinux>(atoi(dirp->d_name));
        ret.push_back(process);
    }
    if(closedir(dir)) {
        throw std::runtime_error(std::strerror(errno));
    }
    return ret;
}
