#include "client_socket.hpp"

ClientSocket::ClientSocket()
{
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientaddr.sin_port = htons(SERVER_PORT);
}

ClientSocket::ClientSocket(int port)
{
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientaddr.sin_port = htons(port);
}

BaseSocket ClientSocket::connect_to_server()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw std::runtime_error("socket() failed!\n");
    else
        std::cout << "Socket created." << std::endl;

    if (connect(fd, (const struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0)
        throw std::runtime_error("connect() failed!\n");
    else
    {
        std::cout << "Connected to: " << clientaddr.sin_addr.s_addr << ", on port: " << clientaddr.sin_port << std::endl;
        return BaseSocket(fd);
    }
}