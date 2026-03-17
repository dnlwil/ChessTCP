#include "common/server_socket.hpp"
#include "common/connection_handler.hpp"
#include <cstring>
#include <array>
#include <thread>

int main()
{
    ServerSocket socket;

    BaseSocket client_socket = socket.accept_connection();

    ServerConnectionHandler handler(std::move(client_socket));

    handler.handle_data_from_client();

    std::cout << "Press ENTER to exit...";
    std::cin.get();

    return 0;
}
