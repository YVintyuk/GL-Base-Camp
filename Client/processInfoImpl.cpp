#include "processInfoImpl.h"
#include "processInfoImpl.h"

#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <cerrno>
#include <dirent.h>
#include <fstream>
#include <string>
#include <streambuf>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

std::string doReadlink(std::string const &path) {
    char buff[PATH_MAX];
    ssize_t len = ::readlink(path.c_str(), buff, sizeof(buff)-1);
    if (len != -1) {
        buff[len] = '\0';
        return std::string(buff);
    }
}

/**
 * retrieve info about 1 process
 * @param pid of the process of the process for lookup
 * @return
 */

processInfo_t getProcessInfo (size_t pid) {
    processInfo_t ret{};
    ret.pid = pid;

    std::string pathToProcessCmdFile = "/proc/" + std::to_string(pid) + "/cmdline";
    std::ifstream streamToReadProcessCmdLineFrom(pathToProcessCmdFile.c_str());
    std::string processCommandLineString(
            (std::istreambuf_iterator<char>(streamToReadProcessCmdLineFrom)),
            std::istreambuf_iterator<char>());

    /**
     * Copy from std::string in the buffer in the processInfo structure
     * COMMAND_SIZE - maximal size for copy
     * Everything longer will be truncated
     */
    strncpy(ret.commandLine, processCommandLineString.c_str(), EXE_NAME_SIZE);

    /**
     * get user_name and copying to return
     */
    struct stat info{};
    if (stat(pathToProcessCmdFile.c_str(), &info) == 0) {
        struct passwd *pw = getpwuid(info.st_uid);
        strncpy(ret.user, pw->pw_name, USER_SIZE);
    }

    std::string pathToExe = "/proc/" + std::to_string(pid) + "/exe";
    std::string exeNameString = doReadlink(pathToExe);
    strncpy(ret.exeName, exeNameString.c_str(), EXE_NAME_SIZE);
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
