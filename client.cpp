#include "common/client_socket.hpp"
#include "common/connection_handler.hpp"
#include <cstring>

int main()
{
    ClientSocket socket;

    BaseSocket server_socket = socket.connect_to_server();

    ClientConnectionHandler handler(std::move(server_socket));

    handler.handle_data();

    return 0;
}