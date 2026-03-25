#include "base_connection_handler.hpp"

class ClientConnectionHandler : public BaseConnectionHandler
{
public:
    ClientConnectionHandler(BaseSocket socket) : BaseConnectionHandler(std::move(socket)) {};
    ~ClientConnectionHandler() = default;

    void handle_frame(StateMachine state, std::span<const unsigned char> data) override;
};