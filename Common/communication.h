#pragma once
#include <netinet/in.h>

#define SERVER_PORT 10000
#define SERVER_IP "127.0.0.1"

int startListeningSocket(sockaddr_in& address);
int connectToServer();
