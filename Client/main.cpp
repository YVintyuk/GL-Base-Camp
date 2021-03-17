#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include "processInfoImpl.h"
#include "processInfoCommon.h"
#include "communication.h"

int main(int argc, char const *argv[])
{
    bool debug = false;
    if (argc > 1 && std::string(argv[1]) == "debug") {
        debug = true;
    }

    auto procVector = getProcessInfoVector(); //get process list
    if (debug)
        for (const auto& p : procVector) {
            std::cout << p;
        }

    int sock = connectToServer();
    if (sock < 0) {
        printf("Error");
        return -1;
    }

    size_t procCount = procVector.size();
    send(sock, &procCount, sizeof(procCount), 0);
    send(sock, &procVector[0], procVector.size()*sizeof(processInfo_t), 0); //send all process info

    size_t procpidToKill = 0;
    ssize_t bytesReadFromSocketCount = read(sock, &procpidToKill, sizeof(procpidToKill));
    std::cout << "Server asks to kill process PID: " << procpidToKill << std::endl;

    int infoAboutKill = -1;
    if (procpidToKill) {
        infoAboutKill = kill(procpidToKill, 11);
    }
    send(sock, &infoAboutKill, sizeof(infoAboutKill), 0);
}
