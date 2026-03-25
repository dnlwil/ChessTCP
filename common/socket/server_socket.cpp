#include "server_socket.hpp"

ServerSocket::ServerSocket() : listenfd(-1)
{
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERVER_PORT);
}

ServerSocket::ServerSocket(int port) : listenfd(-1)
{
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
}

ServerSocket::~ServerSocket()
{
    if (listenfd >= 0)
    {
        close(listenfd);
        std::cout << "Socket closed. (listenfd)\n";
    }
}

BaseSocket ServerSocket::accept_connection()
{
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
        throw std::runtime_error("socket() failed!\n");
    else
        std::cout << "Socket created." << std::endl;

    int opt = 1;
    // enable to reuse socket after restart
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        throw std::runtime_error("bind() failed!\n");
    else
        std::cout << "Binding done." << std::endl;

    if (listen(listenfd, SOMAXCONN) < 0)
        throw std::runtime_error("listen() failed!\n");
    else
        std::cout << "Listening..." << std::endl;

    int fd = accept(listenfd, (struct sockaddr *)&servaddr, &servaddrlen);
    if (fd < 0)
    {
        throw std::runtime_error("accept() failed!\n");
    }
    else
    {
        std::cout << "Connection accepted." << std::endl;
        return BaseSocket(fd);
    }
}

