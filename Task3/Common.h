#pragma once

#include <netinet/udp.h> //Provides declarations for udp header
#include <netinet/ip.h>  //Provides declarations for ip header

#define HEADER_SIZE (sizeof(struct iphdr) + sizeof(struct udphdr))
#define PACKET_SIZE 4096

#define SERVER_IP_ADDR "127.0.0.1"
#define SERVER_PORT 9090

#define CLIENT_IP_ADDR "127.0.0.1"
#define CLIENT_PORT 9091
