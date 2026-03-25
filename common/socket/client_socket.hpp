#pragma once

#include "base_socket.hpp"

class ClientSocket
{
private:
    struct sockaddr_in clientaddr{};

public:
    ClientSocket();
    ClientSocket(int port);
    BaseSocket connect_to_server();
};
