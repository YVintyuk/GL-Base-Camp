#pragma once
#ifdef __linux__
#include <netinet/in.h>
#else
#include <winsock2.h>
#endif

#ifdef __linux__
#define BUFFER_CAST
#define READ_TYPE ssize_t
#define SOCKLEN (socklen_t*)
#define READ read
#else// __linux__
#pragma comment(lib, "Ws2_32.lib")
#define READ_TYPE int
#define SOCKLEN
#define READ(x, y, z) recv(x, y, z, 0)
#define BUFFER_CAST (char*)
#endif

#define SERVER_PORT 10000
#define SERVER_IP "127.0.0.1"

int startListeningSocket(sockaddr_in& address);
int connectToServer();
