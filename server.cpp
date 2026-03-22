#include "common/server_socket.hpp"
#include "common/connection_handler.hpp"
#include <cstring>

int main()
{
    ServerSocket socket;

    BaseSocket client_socket = socket.accept_connection();

    ServerConnectionHandler handler(std::move(client_socket));

    handler.start_communication();
    handler.handle_data();

    return 0;
}
