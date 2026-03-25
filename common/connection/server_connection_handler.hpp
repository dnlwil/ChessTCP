#include "base_connection_handler.hpp"

class ServerConnectionHandler : public BaseConnectionHandler
{
public:
    ServerConnectionHandler(BaseSocket socket) : BaseConnectionHandler(std::move(socket)) {};
    ~ServerConnectionHandler() = default;

    void start_communication();
    void handle_frame(StateMachine state, std::span<const unsigned char> data) override;
};
