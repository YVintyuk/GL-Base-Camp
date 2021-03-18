#include <stdio.h>
#include <string.h>
#include <iostream>
#include "processInfoImpl.h"

#ifdef __linux__
#include <unistd.h>
#include "processInfoCommon.h"
#include "communication.h"
#else

#include "../Common/processInfoCommon.h"
#include "../Common/communication.h"
#endif // __linux__

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
    send(sock, BUFFER_CAST&procCount, sizeof(procCount), 0);
    send(sock, BUFFER_CAST&procVector[0], procVector.size()*sizeof(processInfo_t), 0); //send all process info

    size_t procpidToKill = 0;
    READ_TYPE bytesReadFromSocketCount = READ(sock, BUFFER_CAST&procpidToKill, sizeof(procpidToKill));
    std::cout << "Server asks to kill process PID: " << procpidToKill << std::endl;

    int infoAboutKill = killProcess(procpidToKill);
    send(sock, BUFFER_CAST&infoAboutKill, sizeof(infoAboutKill), 0);
}
