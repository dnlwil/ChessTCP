#pragma once 

#include "base_socket.hpp"

class ServerSocket
{
private:
    int listenfd;
    struct sockaddr_in servaddr{};
    socklen_t servaddrlen = sizeof(servaddr);

public:
    ServerSocket();
    ServerSocket(int port);
    BaseSocket accept_connection();
    ~ServerSocket();
};
