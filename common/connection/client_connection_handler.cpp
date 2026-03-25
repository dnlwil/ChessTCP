#include "client_connection_handler.hpp"

void ClientConnectionHandler::handle_frame(StateMachine state, std::span<const unsigned char> data)
{
    std::cout << "handle_frame()\n"
              << "state = " << static_cast<int>(state) << std::endl;

    board.draw_board();
    switch (state)
    {
    case StateMachine::WaitingForMove:
    {
        std::string temp_buf;

        std::cout << "Make a move.\n"
                  << "Format: <Piece_symbol><Start_square><-><End_square>\n"
                  << "Example: Nb1-c3" << std::endl;

        std::getline(std::cin, temp_buf);
        build_frame(StateMachine::MoveSent, std::span<const unsigned char>(reinterpret_cast<const unsigned char *>(temp_buf.data()), temp_buf.size()));
        send_data();
        break;
    }
    case StateMachine::BoardState:
        break;
    case StateMachine::MoveSent:
        break;
    case StateMachine::Error:
        break;
    default:
        std::cout << "Entered default!" << std::endl;
        break;
    }
    rcv_buf.clear();
}