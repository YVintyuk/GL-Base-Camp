#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>       // for memcpy
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <memory>

#include "Common.h"

class RawUdpReceiver {
private:
	int raw_socket = -1;
public:
	RawUdpReceiver(const std::string& _listening_ip,
	               uint16_t _listening_port) {
		struct sockaddr_in sockstr{};

		/* do not use IPPROTO_RAW to receive packets */
		if ((raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1) {
			perror("socket");
			throw std::runtime_error("Can't open socket!");
		}

		sockstr.sin_family = AF_INET;
		sockstr.sin_port = htons(_listening_port);
		sockstr.sin_addr.s_addr = inet_addr(_listening_ip.c_str());
		auto socklen = (socklen_t) sizeof(sockstr);

		/* use socklen instead sizeof()  Why had you defined socklen? :-)  */
		if (bind(raw_socket, (struct sockaddr*) &sockstr, socklen) == -1) {
			throw std::runtime_error("Can't bind socket!");
		}
	}

	[[nodiscard]] std::unique_ptr<char[]> receiveMessage() const {
		static char buff[PACKET_SIZE] = {0};
		ssize_t msglen;
		if ((msglen = recv(raw_socket, buff, PACKET_SIZE, 0)) == -1) {
			throw std::runtime_error("Can't recv on socket");
		}

		if (msglen <= HEADER_SIZE) /* msg can't be lesser than header! */
			throw std::runtime_error("Received empty message!\n");

		size_t payloadSize = msglen - HEADER_SIZE; // TODO get payload size from UDP header
		auto ret = std::make_unique<char[]>(payloadSize);
		memcpy(ret.get(), buff + HEADER_SIZE, payloadSize);
		return ret;
	}

	~RawUdpReceiver() {
		if (raw_socket > 0) {
			close(raw_socket);
		}
	}
};
