#include "processInfoImpl.h"
#ifdef __linux__
#include "processInfoCommon.h"
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#else
#include "../Common/processInfoCommon.h"
#define strncpy strncpy_s
#endif

#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <cerrno>
#include <fstream>
#include <string>
#include <streambuf>
#include <sys/stat.h>
#include <limits.h>

namespace {
#ifdef __linux__
    std::string doReadlink(std::string const &path) {
        char buff[PATH_MAX];
        ssize_t len = ::readlink(path.c_str(), buff, sizeof(buff) - 1);
        if (len != -1) {
            buff[len] = '\0';
            return std::string(buff);
        }
        return "NaE";
    }
#endif

    std::string getCmdLine(const size_t pid) {
#ifdef __linux__
        /**
         * Reading cmdline info from /proc/pid/cmdline
         */
        std::string pathToProcessCmdFile = "/proc/" + std::to_string(pid) + "/cmdline";
        std::ifstream streamToReadProcessCmdLineFrom(pathToProcessCmdFile.c_str());
        std::string processCommandLineString(
            (std::istreambuf_iterator<char>(streamToReadProcessCmdLineFrom)),
            std::istreambuf_iterator<char>());
        return processCommandLineString;
#else
        return "NaC";
#endif
    }

    std::string getUserName(const size_t pid) {
#ifdef __linux__
        /**
         * get user_name and copying to return
         */
        struct stat info {};
        if (stat(pathToProcessCmdFile.c_str(), &info) == 0) {
            /**
             * No need to free, linux will take care of that.
             * https://linux.die.net/man/3/getpwuid
             */
            struct passwd* pw = getpwuid(info.st_uid);
            return pw->pw_name;
        }
        return "NaU";
#else
        return "NaU";
#endif
    }


    std::string getExeName(const size_t pid) {
#ifdef __linux__
        /**
         * Reading exe name info from /proc/pid/exe
         * Since /proc/pid/exe is symbolic link - using readlink to get real name
         */
        std::string pathToExe = "/proc/" + std::to_string(pid) + "/exe";
        return doReadlink(pathToExe);
#else
        return "NaE";
#endif
    }
    /**
     * retrieve info about 1 process
     * @param pid of the process of the process for lookup
     * @return
     */
    processInfo_t getProcessInfo(const size_t pid) {
        processInfo_t ret{};
        ret.pid = pid;

        /**
         * Copy from std::string in the buffer in the processInfo structure
         * COMMAND_SIZE - maximal size for copy
         * Everything longer will be truncated
         */
        strncpy(ret.commandLine, getCmdLine(pid).c_str(), EXE_NAME_SIZE);

        strncpy(ret.user, getUserName(pid).c_str(), USER_SIZE);

        strncpy(ret.exeName, getExeName(pid).c_str(), EXE_NAME_SIZE);
        return ret;
    }
} // anonymous namespace

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
        ret.push_back(getProcessInfo(atoi(dirp->d_name)));
    }
    if(closedir(dir)) {
        throw std::runtime_error(std::strerror(errno));
    }
#else
    ret.push_back(getProcessInfo(1));
    ret.push_back(getProcessInfo(2));
    ret.push_back(getProcessInfo(3));
#endif
    return ret;
}

int killProcess(size_t pid) {
    /*
    * can't kill zero
    */
    if (!pid)
        return -1;
#ifdef __linux__
    return kill(pid, 11);
#else
    return -1;
#endif
}

