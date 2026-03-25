#pragma once 

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <strings.h>

constexpr int SERVER_PORT = 9999;

class BaseSocket
{
protected:
    int fd;

public:
    BaseSocket() : fd(-1) {};
    BaseSocket(int fd) : fd(fd) {};
    BaseSocket(const BaseSocket &) = delete;
    BaseSocket &operator=(const BaseSocket &) = delete;

    BaseSocket(BaseSocket &&other) noexcept : fd(other.fd)
    {
        other.fd = -1;
    }

    BaseSocket &operator=(BaseSocket &&other) noexcept
    {
        if (this != &other)
        {
            if (fd >= 0)
                close(fd);
            fd = other.fd;
            other.fd = -1;
        }
        return *this;
    }

    ~BaseSocket()
    {
        if (fd >= 0)
        {
            close(fd);
            std::cout << "Socket closed. (fd)\n";
        }
    }

    ssize_t send_data(const void *buf, size_t len);
    ssize_t receive_data(void *buf, size_t len);
};
