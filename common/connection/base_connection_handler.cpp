#include "base_connection_handler.hpp"

void BaseConnectionHandler::build_frame(StateMachine state, const std::span<const unsigned char> data)
{
    snd_buf.clear();
    snd_buf.reserve(HEADER_SIZE + data.size());
    snd_buf.push_back(static_cast<unsigned char>(state));

    unsigned short length = data.size();
    snd_buf.push_back(length & 0xFF);
    snd_buf.push_back((length >> 8) & 0xFF);

    snd_buf.insert(snd_buf.end(), data.begin(), data.end());
}

void BaseConnectionHandler::send_data()
{
    socket.send_data(snd_buf.data(), snd_buf.size());
}

void BaseConnectionHandler::handle_data()
{
    while (true)
    {
        unsigned char tmp_buf[1024]{};
        std::cout << "Waiting for data..." << std::endl;
        ssize_t bytes = socket.receive_data(tmp_buf, sizeof(tmp_buf));
        std::cout << "number of received bytes = " << bytes << std::endl;
        if (bytes == 0)
        {
            std::cout << "Disconnected." << std::endl;
            exit(-1);
            break;
        }

        if (bytes < 0)
        {
            std::cout << "Error receiving message." << std::endl;
            exit(-1);
            break;
        }

        rcv_buf.insert(rcv_buf.end(), tmp_buf, tmp_buf + bytes);

        parse_frame();
    }
}

void BaseConnectionHandler::parse_frame()
{
    if (rcv_buf.size() < HEADER_SIZE)
        return;

    unsigned short data_length = static_cast<unsigned char>(rcv_buf[1]) | (static_cast<unsigned char>(rcv_buf[2]) << 8);

    if (rcv_buf.size() < (HEADER_SIZE + data_length))
        return;

    StateMachine state = static_cast<StateMachine>(rcv_buf[0]);
    // std::vector<unsigned char> data((rcv_buf.begin() + HEADER_SIZE), (rcv_buf.begin() + HEADER_SIZE + data_length));
    std::span<const unsigned char> data((rcv_buf.begin() + HEADER_SIZE), data_length);
    handle_frame(state, data);
}
