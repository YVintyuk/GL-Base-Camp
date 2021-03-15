#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <vector>

#include "processInfoCommon.h"

int open_socket (sockaddr_in& address) {
    int server_fd, new_socket, valread;
    int opt = 1;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 10000
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 10000
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

int main(int argc, char const *argv[])
{
    sockaddr_in address{};
    int addrlen = sizeof(address);
    int sock = -1;
    int server_fd = open_socket(address);
    if (server_fd < 0) {
        printf("Error");
        return -1;
    };

    /**
     * Receiving the process count
     */
    size_t procCount = 0;
    if ((sock = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    ssize_t bytesReadFromSocketCount = read(sock, &procCount, sizeof(procCount));
    std::cout << "Going to receive " << procCount << " processes\n";
    /**
     * Done receiving process count
     */

    /**
     * Receiving process list
     */
    bytesReadFromSocketCount = 0;
    size_t bufferSize =  procCount * sizeof(processInfo_t);
    auto* buffer = static_cast<char *>(malloc(bufferSize));
    while (bytesReadFromSocketCount < bufferSize) {
        bytesReadFromSocketCount += read(
                sock, buffer + bytesReadFromSocketCount, bufferSize - bytesReadFromSocketCount);
    }

    auto* procInfoPtr = reinterpret_cast<processInfo_t*>(buffer);
    std::vector<processInfo_t> procInfo;
    procInfo.assign(procInfoPtr, procInfoPtr + procCount);
    for (const auto& p : procInfo) {
        std::cout << p;
    }
}