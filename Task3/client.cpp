#include <unistd.h>      // sleep()
#include "SynchronousStreamer.h"

int main ()
{
	SynchronousStreamer sender(CLIENT_IP_ADDR, CLIENT_PORT, SERVER_IP_ADDR, SERVER_PORT);
	//loop if you want to flood :)
	while (1)
	{
		std::string data = "alskjdfhskdfhskdlfjhsdkfjaslkdfh";
		//Send the packet
		sender.sendStringMessage(data);
		// sleep for 1 seconds
		sleep(1);
	}

	return 0;
}

//Complete