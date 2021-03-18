#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "processInfoCommon.h"
#include "communication.h"
#else
#include <winsock2.h>
#include "../Common/processInfoCommon.h"
#include "../Common/communication.h"
#endif // __linux__

size_t findProcessforKilling(const std::vector<processInfo_t> &processInfoVector);

int main(int argc, char const *argv[])
{
    bool debug = false;
    if (argc > 1 && std::string(argv[1]) == "debug") {
        debug = true;
    }

    sockaddr_in address{};
    int addrlen = sizeof(address);
    int sock = -1;
    int server_fd = startListeningSocket(address);
    if (server_fd < 0) {
        printf("Error");
        return -1;
    };

    if ((sock = accept(server_fd, (struct sockaddr *)&address,
                       SOCKLEN&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    /**
     * Receiving the process count
     */
    size_t procCount = 0;
    READ_TYPE bytesReadFromSocketCount = READ(sock, BUFFER_CAST&procCount, sizeof(procCount));
    std::cout << "Going to receive " << procCount << " processes\n";
    /**
     * Done receiving process count
     */

    /**
     * Receiving process list
     */
    bytesReadFromSocketCount = 0;
    READ_TYPE bufferSize =  procCount * sizeof(processInfo_t);
    auto* buffer = static_cast<char *>(malloc(bufferSize));
    while (bytesReadFromSocketCount < bufferSize) {
        bytesReadFromSocketCount += READ(
                sock, BUFFER_CAST(buffer + bytesReadFromSocketCount), bufferSize - bytesReadFromSocketCount);
    }

    auto* procInfoPtr = reinterpret_cast<processInfo_t*>(buffer);
    std::vector<processInfo_t> procInfo;
    procInfo.assign(procInfoPtr, procInfoPtr + procCount);
    if (debug)
        for (const auto& p : procInfo) {
            std::cout << p;
        }
    size_t processpidToKill = findProcessforKilling(procInfo);
    send(sock, BUFFER_CAST&processpidToKill, sizeof(processpidToKill), 0);

    int clientResponse = 0;
    bytesReadFromSocketCount = READ(sock, BUFFER_CAST&clientResponse, sizeof(clientResponse));
    std::cout << "Client responded with: " << clientResponse << std::endl;

}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

size_t findProcessforKilling(const std::vector<processInfo_t> &processInfoVector) {
    for (const auto& p : processInfoVector) {
        if (hasEnding(p.exeName, "sleep")) {
            return p.pid;
        }
    }
    return 0;
}
