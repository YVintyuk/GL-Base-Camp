#include <iostream>

#include "SynchronousStreamer.h"

int main() {
	SynchronousStreamer receiver(SERVER_IP_ADDR, SERVER_PORT, CLIENT_IP_ADDR, CLIENT_PORT);
	receiver.serve();
}