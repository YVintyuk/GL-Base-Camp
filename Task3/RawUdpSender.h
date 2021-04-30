#pragma once

#include <cstring>       // memcpy et al
#include <sys/socket.h>  //for socket ofcourse
#include <netinet/udp.h> //Provides declarations for udp header
#include <netinet/ip.h>  //Provides declarations for ip header
#include <arpa/inet.h>   // inet_addr
#include <unistd.h>      // sleep()
#include <iostream>
#include <memory>

#include "Common.h"

/*
	96 bit (12 bytes) pseudo header needed for checksum calculation
*/
struct pseudo_header {
	u_int32_t source_address;
	u_int32_t dest_address;
	u_int8_t placeholder;
	u_int8_t protocol;
	u_int16_t udp_length;
};

/*
	Generic checksum calculation function
*/
uint16_t csum(unsigned short *ptr, int nbytes);


class RawUdpSender {
private:
	int raw_socket = -1;
	std::string source_ip;
	std::string dest_ip;
	uint16_t dest_port;
	uint16_t source_port;
public:
	explicit RawUdpSender(const std::string& _source_ip,
	                      uint16_t _source_port,
	                      const std::string& _dest_ip,
	                      uint16_t _dest_port) {
		//Create a raw socket
		raw_socket = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
		if (raw_socket == -1) {
			throw std::runtime_error("Failed to create socket");
		}

		//IP_HDRINCL to tell the kernel that headers are included in the packet
		int one = 1;
		const int *val = &one;

		if (setsockopt(raw_socket, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0) {
			throw std::runtime_error("Error setting IP_HDRINCL");
		}

		source_ip = _source_ip; // Dummy, can be anything
		dest_ip = _dest_ip;
		dest_port = _dest_port;
		source_port = _source_port;
	}

	void constructPacketAndSend(const char *_data, const size_t _data_size) {
		//Datagram to represent the packet
		static char datagram[PACKET_SIZE] = {0};
		char *payload;

		//IP header (points to buffer)
		auto *iph = (struct iphdr *) datagram;

		//UDP header
		auto *udp_header = (struct udphdr *) (datagram + sizeof(struct ip));

		//Copy data in packet
		payload = datagram + HEADER_SIZE;
		memcpy(payload, _data, _data_size);

		buildIpHeader(iph, _data_size);
		//Ip checksum
		iph->check = csum((unsigned short *) datagram, iph->tot_len);

		//UDP Header
		buildUdpHeader(udp_header, _data_size);
		udp_header->check = calculateUdpCheckSum(udp_header, _data_size);

		dispatchDatagram(datagram, HEADER_SIZE + _data_size);
	}

private:
	uint16_t
	calculateUdpCheckSum(const udphdr *udp_header, size_t _payload_size) const {
		//Now the UDP checksum
		struct pseudo_header psh{};
		psh.source_address = inet_addr(source_ip.c_str());
		psh.dest_address = inet_addr(dest_ip.c_str());
		psh.placeholder = 0;
		psh.protocol = IPPROTO_UDP;
		auto udp_len = sizeof(struct udphdr) + _payload_size;
		psh.udp_length = htons(udp_len);

		size_t psize = HEADER_SIZE + _payload_size;
		std::unique_ptr<char[]> pseudogram = std::make_unique<char[]>(psize);

		memcpy(pseudogram.get(), (char *) &psh, sizeof(struct pseudo_header));
		memcpy(pseudogram.get() + sizeof(struct pseudo_header), udp_header, udp_len);

		return csum((unsigned short *) pseudogram.get(), psize);
	}

	void buildUdpHeader(udphdr *udp_header, size_t payloadSize) const {
		udp_header->source = htons(source_port);
		udp_header->dest = htons(dest_port);
		udp_header->len = htons(sizeof(struct udphdr) + payloadSize);
		udp_header->check = 0;    //leave checksum 0 now, filled later by pseudo header
	}

	void buildIpHeader(iphdr *iph, size_t _payload_size) const {
		static uint16_t packet_id = rand();
		//Fill in the IP Header
		iph->ihl = 5;
		iph->version = 4;
		iph->tos = 0;
		iph->tot_len = HEADER_SIZE + _payload_size;
		iph->id = htonl(++packet_id);
		iph->frag_off = 0;
		iph->ttl = 255;
		iph->protocol = IPPROTO_UDP;
		iph->check = 0;        //Set to 0 before calculating checksum
		iph->saddr = inet_addr(source_ip.c_str());    //Spoof the source ip address
		iph->daddr = inet_addr(dest_ip.c_str());
	}

	void dispatchDatagram(const char *_datagram, size_t _len) const {
		//some address resolution
		struct sockaddr_in sin{};
		sin.sin_family = AF_INET;
		sin.sin_port = htons(dest_port);
		sin.sin_addr.s_addr = inet_addr(dest_ip.c_str());

		if (sendto(raw_socket, (void*)_datagram, _len, 0, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
			throw std::runtime_error("sendto failed");
		} else {
			std::cout << "Packet Send. Length : " << _len << std::endl;
		}
	}

public:
	~RawUdpSender() {
		if (raw_socket > 0) {
			close(raw_socket);
		}
	}
};
