#include "server_connection_handler.hpp"

void ServerConnectionHandler::start_communication()
{
    build_frame(StateMachine::WaitingForMove);
    send_data();
}

void ServerConnectionHandler::handle_frame(StateMachine state, std::span<const unsigned char> data)
{
    std::cout << "handle_frame()\n"
              << "state = " << static_cast<int>(state) << std::endl;

    switch (state)
    {
    case StateMachine::BoardState:
        break;
    case StateMachine::MoveSent:
    {
        build_frame(StateMachine::WaitingForMove);
        send_data();
        break;
    }
    case StateMachine::Error:
        break;
    default:
        std::cout << "Entered default!" << std::endl;
        break;
    }
    rcv_buf.clear();
}
