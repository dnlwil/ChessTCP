#include "common/client_socket.hpp"
#include "common/connection_handler.hpp"
#include <array>
#include <string>
#include <cstring>
#include <thread>

void receive_data_from_server(BaseSocket &socket)
{
    while (true)
    {
        std::array<char, 100> buf;
        ssize_t n = socket.receive_data(buf.data(), buf.size());
        std::cout << "Number of bytes received:" << n << std::endl;

        if (n == 0)
        {
            std::cout << "Server disconnected." << std::endl;
            break;
        }

        if (n < 0)
        {
            std::cout << "Error receiving message." << std::endl;
            break;
        }

        std::cout.write(buf.data(), n);
        std::cout << std::endl;
        //buf.fill(0);
    }
}

void send_data_to_server(BaseSocket &socket)
{
    while (true)
    {
        std::array<char, 100> buf{};
        std::cout << "Enter text:" << std::endl;
        std::cin.getline(buf.data(), buf.size());
        socket.send_data(buf.data(), std::strlen(buf.data()));

        //buf.fill(0);
    }
}

int main()
{
    ClientSocket socket;
    BaseSocket server_socket = socket.connect_to_server();

    // std::thread t(send_data_to_server, std::ref(server_socket));
    // receive_data_from_server(socket);

    // t.join();
    ClientConnectionHandler handler(std::move(server_socket));
    handler.handle_data_from_server();

    return 0;
}