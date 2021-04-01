#include "model/processInfo.h"
#ifdef __linux__
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <signal.h>
#else
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <locale>
#pragma comment(lib, "Psapi.lib")
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
#include <iostream>

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

    std::string getCmdLine(const PID_TYPE pid) {
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

    std::string getUserName(const PID_TYPE pid) {
#ifdef __linux__
        /**
         * get user_name and copying to return
         */
        struct stat info {};
        std::string pathToProcessCmdFile = "/proc/" + std::to_string(pid) + "/cmdline";
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


    std::string getExeName(const PID_TYPE pid) {
#ifdef __linux__
        /**
         * Reading exe name info from /proc/pid/exe
         * Since /proc/pid/exe is symbolic link - using readlink to get real name
         */
        std::string pathToExe = "/proc/" + std::to_string(pid) + "/exe";
        return doReadlink(pathToExe);
#else
        TCHAR szProcessName[MAX_PATH] = TEXT("NaE");

        // Get a handle to the process.
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE, pid);

        // Get the process name.
        if (NULL != hProcess)
        {
            HMODULE hMod;
            DWORD cbNeeded;

            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            {
                GetModuleFileNameEx(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
            }
        }
        // Release the handle to the process
        CloseHandle(hProcess);
        std::wstring ws = szProcessName;
        return std::string(ws.begin(), ws.end());
#endif
    }
    /**
     * retrieve info about 1 process
     * @param pid of the process of the process for lookup
     * @return
     */
    processInfo_t getProcessInfo(const PID_TYPE pid) {
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

#ifndef __linux__

void PrintProcessNameAndID(DWORD processID)
{

}
#endif

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

int killProcess(PID_TYPE pid) {
    /*
    * can't kill zero
    */
    if (!pid)
        return -1;
#ifdef __linux__
    return kill(pid, 11);
#else
    HANDLE handle = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (handle == NULL)
    {
        std::cout << "Cannot open process with ID "<< pid << ", error code " << GetLastError() << std::endl;
        return -1;
    }
    if (TerminateProcess(handle, -1))
    {
        return 0;
    }
    else {
        std::cout << "Cannot kill process with ID " << pid << ", error code " << GetLastError() << std::endl;
        return -1;
    }
    CloseHandle(handle);
    return -1;
#endif
}

