#pragma once

#include <list>
#include "RawUdpSender.h"
#include "RawUdpReceiver.h"

enum SyncMessageType {
	CONNECT,
	DATA,
	ACK,
	ERROR
};

enum SyncStreamerState {
	OPEN,
	CONNECTED,
};

struct SyncMessageHeader {
	SyncMessageType type;
	size_t checksum;
	size_t message_size; // Size of header + payload
};

class SyncMessage {
private:
	SyncMessageHeader header{};
	std::unique_ptr<char[]> payload;
protected:
	size_t calculateChecksum(const SyncMessageHeader& _header, const char* _payload) {
		return _header.message_size + header.type;
	}

public:
	std::unique_ptr<char[]> getBytes() {
		std::unique_ptr<char[]> buffer = std::make_unique<char[]>(header.message_size);
		memcpy(buffer.get(), &header, sizeof(SyncMessageHeader));
		memcpy(buffer.get() + sizeof(SyncMessageHeader), payload.get(), header.message_size - sizeof(SyncMessageHeader));
		return buffer;
	}

	size_t getSize() {
		return header.message_size;
	}
	SyncMessageType getType() {
		return header.type;
	}

	SyncMessage(SyncMessageType _type, const char* _payload, size_t _data_size) {
		if (_data_size > 0) {
			payload = std::make_unique<char[]>(_data_size);
			memcpy(payload.get(), _payload, _data_size);
		}

		header.type = _type;
		header.message_size = _data_size + sizeof(SyncMessageHeader);
		header.checksum = calculateChecksum(header, payload.get());
	}

	SyncMessage(SyncMessage* _sync_message, std::unique_ptr<char[]> _payload) {
		header = _sync_message->header;
		payload = std::move(_payload);
		if (header.checksum != calculateChecksum(header, payload.get())) {
			throw std::runtime_error("Received packet with bad checksum");
		}
	}

	std::string getPayloadAsString() {
		return std::string(payload.get());
	}
};

class SynchronousStreamer {
protected:
	RawUdpSender sender; // TODO: consider creating one for each CONNECT
	RawUdpReceiver receiver;

	SyncStreamerState state;

	std::list<SyncMessage> message_queue;
public:
	SynchronousStreamer(const std::string& _source_ip,
	                    uint16_t _source_port,
	                    const std::string& _dest_ip,
	                    uint16_t _dest_port) : sender(_source_ip, _source_port, _dest_ip, _dest_port),
	                                           receiver(_source_ip, _source_port) {	}

	void serve() {
		while (true) {
			auto raw_message = receiver.receiveMessage();
			auto* sync_message = reinterpret_cast<SyncMessage *>(raw_message.get());

			auto msgType = sync_message->getType();
			switch (state) {
				case OPEN:
					switch (msgType) {
						case CONNECT: {
							state = CONNECTED;
							sendServiceMessage(ACK);
							continue;
						}
						case DATA:
						case ACK:
							// TODO: write log, shouldn't happen
							continue;
						case ERROR:
							return;
					}
					break;
				case CONNECTED:
					switch (msgType) {
						case DATA: {
							auto payload = std::make_unique<char[]>(sync_message->getSize());
							memcpy(payload.get(), raw_message.get() + sizeof(SyncMessage), sync_message->getSize());
							try {
								auto message = SyncMessage(sync_message, std::move(payload));
								std::cout << "Received message; size " << message.getSize() << " " << message.getPayloadAsString() << std::endl;
								sendServiceMessage(ACK);
//								message_queue.push_back(message);
							} catch (const std::runtime_error& e) {
								sendServiceMessage(ERROR);
							}
							continue;
						}
						case CONNECT:
						case ACK:
							// TODO: write log, shouldn't happen
							continue;
							// TODO: send next packet
						case ERROR:
							return;
					}
					break;
			}
		}
	}

	void establishConnection() {
		sendServiceMessage(CONNECT);

		if (getServiceMessage() == ERROR) {
			// TODO: retry again for N times, then throw
			throw std::runtime_error("Can't establish connection");
		}
		state = CONNECTED;
	}

	void sendStringMessage(const std::string& _message) {
		if (state == OPEN) {
			establishConnection();
		}

		sendPayload(_message.c_str(), _message.size());
		if (getServiceMessage() == ERROR) {
			// TODO: retry again for N times, then throw
			throw std::runtime_error("Sending message failed");
		}
	}

	~SynchronousStreamer() {
		sendServiceMessage(ERROR);
	}

protected:
	SyncMessageType getServiceMessage() {
		auto raw_message = receiver.receiveMessage();
		auto* sync_message = reinterpret_cast<SyncMessage *>(raw_message.get());
		return sync_message->getType();
	}

	void sendServiceMessage(SyncMessageType _type) {
		switch (_type) {
			case CONNECT:
			case ACK:
			case ERROR: {
				SyncMessage message(_type, "", 0);
				sender.constructPacketAndSend(message.getBytes().get(), message.getSize());
				break;
			}
			case DATA:
				throw std::runtime_error("Can't send empty data message!");
				break;
		}

	}

	void sendPayload(const char* _data, size_t _data_size) {
		SyncMessage message(DATA, _data, _data_size);
		sender.constructPacketAndSend(message.getBytes().get(), message.getSize());
	}

	std::unique_ptr<char[]> receivePayload() {
		auto raw_message = receiver.receiveMessage();
		auto* sync_header = reinterpret_cast<SyncMessage *>(raw_message.get());
		auto ret = std::make_unique<char[]>(sync_header->getSize());
		memcpy(ret.get(), raw_message.get() + sizeof(SyncMessage), sync_header->getSize());
		return ret;
	}
};
