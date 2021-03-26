server_objects := Server/main.o Common/processInfoCommon.o Common/communication.o
client_objects := Client/main.o Client/processInfoImpl.o Common/processInfoCommon.o Common/communication.o

# FIXME!
# Adding include flag - hacky but easy
CXXFLAGS := $(CXXFLAGS) -ICommon

# building server
server: $(server_objects)
	g++ -Wall $(server_objects) -o server

# building client
client: $(client_objects)
	g++ -Wall $(client_objects) -o client

clean:
	-rm $(server_objects) $(client_objects) client server

all: client server

test: server client
	./server &
	sleep 1s
	sleep 10m &
	./client
