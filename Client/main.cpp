#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "processInfo.h"

#define PORT 8080

int open_socket() {
    int sock = 0;
    struct sockaddr_in serv_addr{};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    return sock;
};

int main(int argc, char const *argv[])
{
  //  int sock = open_socket();
   // if (sock < 0) {
   //     printf("Error");
   //   return -1;
   // }
    auto procVector = getProcessInfoVector(); //get process list
    for (const auto& p : procVector) {
        std::cout << "\tProcess\n\tpid: " << p.pid
                  << "\n\tuser: " << p.user
                  << "\n\tcmd: " << p.command
                  << "\n\targv: " << p.argv << "\n";
    }
    size_t procCount = procVector.size();
//    send(sock, &procCount, sizeof(procCount), 0);
//    send(sock, &procVector, procVector.size()*sizeof(processInfo_t), 0); //send all process info
}