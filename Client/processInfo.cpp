#include "processInfo.h"

#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <cerrno>
#include <dirent.h>
#include <fstream>
#include <string>
#include <streambuf>

/**
 * retrieve info about 1 process
 * @param pid of the process of the process for lookup
 * @return
 */

processInfo_t getProcessInfo (size_t pid) {
    processInfo_t ret{};
    ret.pid = pid;
    strcpy (ret.command, "Hello"); //tmp value
        std::string cmdPath = "proc/"+std::to_string(pid)+"/cmdline";
    std::ifstream cmdFile(cmdPath.c_str());
    std::string cmdStr((std::istreambuf_iterator<char>(cmdFile)),
                    std::istreambuf_iterator<char>());
    strncpy(ret.command, cmdStr.c_str(), COMMAND_SIZE);
    return ret;
}

/**
 * open "/proc"
 * read all files in proc one by one
 * check is it directory
 * is every character of the name a digit?
 * add info about process to list of process
 * @return
 */
std::vector <processInfo_t> getProcessInfoVector() {
    std::vector <processInfo_t> ret;
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
        ret.push_back(getProcessInfo(atoi(dirp->d_name)));
    }
    if(closedir(dir)) {
        throw std::runtime_error(std::strerror(errno));
    }
    return ret;
}
