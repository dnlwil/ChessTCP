#pragma once

#include "chess_board.hpp"
#include "base_socket.hpp"
#include <vector>
#include <cstring>
#include <span>

constexpr size_t HEADER_SIZE = 3u;
constexpr size_t MOVE_FORMAT_SIZE = 6u;

enum class StateMachine : unsigned char
{
    Init = 0u,
    WaitingForData,
    BoardState,
    WaitingForMove,
    MoveSent,
    Error,
};

class BaseConnectionHandler
{
protected:
    BaseSocket socket;
    ChessBoard board;
    std::vector<unsigned char> rcv_buf;
    std::vector<unsigned char> snd_buf;

public:
    BaseConnectionHandler(BaseSocket socket);
    ~BaseConnectionHandler() = default;
    void build_frame(StateMachine state, std::span<const unsigned char> data = {});
    void send_data();
    void handle_data();
    void parse_frame();
    virtual void handle_frame(StateMachine state, std::vector<unsigned char> &data) = 0;
};

BaseConnectionHandler::BaseConnectionHandler(BaseSocket socket) : socket(std::move(socket))
{
    rcv_buf.reserve(100);
    snd_buf.reserve(100);
}


void BaseConnectionHandler::build_frame(StateMachine state, const std::span<const unsigned char> data)
{
    std::cout << "Setting state = " << static_cast<int>(state) << std::endl;

    snd_buf.clear();
    snd_buf.reserve(HEADER_SIZE + data.size());
    snd_buf.push_back(static_cast<unsigned char>(state));

    unsigned short length = data.size();
    snd_buf.push_back(length & 0xFF);
    snd_buf.push_back((length >> 8) & 0xFF);
    std::cout << "----------------------------------------" << std::endl;
    for (size_t i = 0; i < snd_buf.size(); i++)
        std::cout << "snd_buf[" << i << "] = " << (int)snd_buf[i] << std::endl;
    snd_buf.insert(snd_buf.end(), data.begin(), data.end());
    std::cout << "----------------------------------------" << std::endl;
    for (size_t i = 0; i < snd_buf.size(); i++)
        std::cout << "snd_buf[" << i << "] = " << (int)snd_buf[i] << std::endl;
}

void BaseConnectionHandler::send_data()
{
    socket.send_data(snd_buf.data(), snd_buf.size());
    // std::cout << "Sending " << snd_buf.size() << " bytes\n";
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
        // std::cout << "rcv_buf[0] = " << (int)rcv_buf[0] << std::endl;
         for(size_t i = 0; i < rcv_buf.size(); i++)
             std::cout << "rcv_buf[" << i << "] = " << (int)rcv_buf[i] << std::endl;
        std::cout << "rcv_buf.size() = " << rcv_buf.size() << std::endl;
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
    std::vector<unsigned char> data((rcv_buf.begin() + HEADER_SIZE), (rcv_buf.begin() + HEADER_SIZE + data_length));
    handle_frame(state, data);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------ //

class ServerConnectionHandler : public BaseConnectionHandler
{
public:
    ServerConnectionHandler(BaseSocket socket);
    ~ServerConnectionHandler() = default;

    //void handle_data_from_client();
    void start_communication();
    void handle_frame(StateMachine state, std::vector<unsigned char> &data) override;
};

ServerConnectionHandler::ServerConnectionHandler(BaseSocket socket) : BaseConnectionHandler(std::move(socket))
{

}

void ServerConnectionHandler::start_communication()
{
    build_frame(StateMachine::WaitingForMove);
    send_data();
}

void ServerConnectionHandler::handle_frame(StateMachine state, std::vector<unsigned char> &data)
{
    std::cout << "handle_frame()\n"
              << "state = " << static_cast<int>(state) << std::endl;
    //(void *)data.data();
    switch (state)
    {
    // case StateMachine::WaitingForMove:
    // {
    //     std::array<char, 6> temp_buf{};

    //     std::cout << "Make a move.\n"
    //               << "Format: <Piece_symbol><Start_square><-><End_square>\n"
    //               << "Example: Nb1-c3" << std::endl;

    //     std::cin.getline(temp_buf.data(), MOVE_FORMAT_SIZE);
    //     build_frame(StateMachine::MoveSent, std::span<const unsigned char>(reinterpret_cast<const unsigned char *>(temp_buf.data()), temp_buf.size()));
    //     send_data();
    //     break;
    // }
    case StateMachine::BoardState:
        break;
    case StateMachine::MoveSent:
        std::cout << "handle_frame(); data.size() = " << data.size() << std::endl;
        std::cout << "handle_frame(); data.data() = " << data.data() << std::endl; // todo jakies smieci sie tu wyswietlaja
        build_frame(StateMachine::WaitingForMove);
        send_data();
        break;
    case StateMachine::Error:
        break;
    default:
        std::cout << "Entered default!" << std::endl;
        break;
    }
    rcv_buf.clear();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------ //

class ClientConnectionHandler : public BaseConnectionHandler
{
public:
    ClientConnectionHandler(BaseSocket socket);
    ~ClientConnectionHandler() = default;
    //void handle_data_from_server();
    //void get_data_from_server(int x);
    //void parse_frame();
    void handle_frame(StateMachine state, std::vector<unsigned char> &data) override;
};

ClientConnectionHandler::ClientConnectionHandler(BaseSocket socket) : BaseConnectionHandler(std::move(socket))
{
}

void ClientConnectionHandler::handle_frame(StateMachine state, std::vector<unsigned char> &data)
{
    std::cout << "handle_frame()\n" << "state = " << static_cast<int>(state) << std::endl;
    //(void*)data.data();
    board.draw_board();
    switch(state)
    {
        case StateMachine::WaitingForMove:
        {
            std::string temp_buf;
            
            std::cout << "Make a move.\n"
            << "Format: <Piece_symbol><Start_square><-><End_square>\n" 
            << "Example: Nb1-c3" << std::endl;

            std::getline(std::cin, temp_buf);
            build_frame(StateMachine::MoveSent, std::span<const unsigned char>(reinterpret_cast<const unsigned char*>(temp_buf.data()),temp_buf.size()));
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
