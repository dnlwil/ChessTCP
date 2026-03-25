#include "base_socket.hpp"

ssize_t BaseSocket::send_data(const void *buf, size_t len)
{
    return send(fd, buf, len, 0);
}

ssize_t BaseSocket::receive_data(void *buf, size_t len)
{
    return recv(fd, buf, len, 0);
}