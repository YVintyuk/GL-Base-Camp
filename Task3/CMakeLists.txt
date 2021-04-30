cmake_minimum_required(VERSION 3.12)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

set(COMMUNICATION RawUdpSender.h RawUdpSender.cpp RawUdpReceiver.cpp RawUdpReceiver.h SynchronousStreamer.cpp SynchronousStreamer.h)

add_executable(server server.cpp ${COMMUNICATION})
add_executable(client client.cpp ${COMMUNICATION})
