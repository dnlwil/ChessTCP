#pragma once

#include "chess_board.hpp"
#include "base_socket.hpp"
#include <vector>
#include <cstring>

constexpr size_t HEADER_SIZE = 3u;

enum class StateMachine : unsigned char
{
    WaitingForData = 0xFF,
    BoardState = 0xFE,
    WaitingForMove = 0xFD,
    Move = 0xFC,
    Error = 0xFB,
};

class BaseConnectionHandler
{
protected:
    BaseSocket socket;
    ChessBoard board;
    bool running;
    std::vector<unsigned char> rcv_buf;
    std::vector<unsigned char> snd_buf;

public:
    BaseConnectionHandler(BaseSocket socket);
    ~BaseConnectionHandler();
    void build_frame(StateMachine state, const std::vector<unsigned char> &data = {});
};

BaseConnectionHandler::BaseConnectionHandler(BaseSocket socket) : socket(std::move(socket)), rcv_buf(100), snd_buf(100)
{

}

BaseConnectionHandler::~BaseConnectionHandler()
{

}

void BaseConnectionHandler::build_frame(StateMachine state, const std::vector<unsigned char> &data)
{
    std::cout << "Setting state = " << static_cast<int>(state) << std::endl;

    snd_buf.clear();
    snd_buf.reserve(3 + data.size());
    snd_buf.push_back(static_cast<unsigned char>(state));

    unsigned short length = data.size();
    snd_buf.push_back(length & 0xFF);
    snd_buf.push_back((length >> 8) & 0xFF);

    snd_buf.insert(snd_buf.end(), data.begin(), data.end());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------ //

class ServerConnectionHandler : public BaseConnectionHandler
{
public:
    ServerConnectionHandler(BaseSocket socket);
    ~ServerConnectionHandler();

    void handle_data_from_client();
    void send_data_to_client();
};

ServerConnectionHandler::ServerConnectionHandler(BaseSocket socket) : BaseConnectionHandler(std::move(socket))
{

}

ServerConnectionHandler::~ServerConnectionHandler()
{

}

void ServerConnectionHandler::handle_data_from_client()
{
    StateMachine state = StateMachine::WaitingForMove;

    while (true)
    {
        switch (state)
        {
        case StateMachine::WaitingForMove:
            build_frame(StateMachine::WaitingForMove);
            send_data_to_client(); // always send at least 3 bytes, state + length of data
            state = StateMachine::WaitingForData;
            break;

        case StateMachine::WaitingForData:
            std::cout << "Entered WaitingForData" << std::endl;
            std::cout << "Press ENTER to exit...";
            std::cin.get();
            exit(-1);
            break;

        case StateMachine::BoardState:

            break;

        case StateMachine::Move:

            break;

        case StateMachine::Error:

            break;

        default:
            std::cout << "Entered default." << std::endl;
            break;
        }
    }
}

void ServerConnectionHandler::send_data_to_client()
{
    socket.send_data(snd_buf.data(), snd_buf.size());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------ //

class ClientConnectionHandler : public BaseConnectionHandler
{
public:
    ClientConnectionHandler(BaseSocket socket);
    ~ClientConnectionHandler();
    void handle_data_from_server();
    void get_data_from_server(int x);
    void parse_frame();
    void handle_frame(StateMachine state, std::vector<unsigned char> &data);
};

ClientConnectionHandler::ClientConnectionHandler(BaseSocket socket) : BaseConnectionHandler(std::move(socket))
{
}

ClientConnectionHandler::~ClientConnectionHandler()
{
}


void ClientConnectionHandler::handle_data_from_server()
{
    while (true)
    {
        unsigned char tmp_buf[1024]{};
        ssize_t bytes = socket.receive_data(tmp_buf, sizeof(tmp_buf));
        //std::cout << sizeof(tmp_buf) << std::endl;
        if (bytes == 0)
        {
            std::cout << "Server disconnected." << std::endl;
            exit(-1);
            break;
        }

        if (bytes < 0)
        {
            std::cout << "Error receiving message." << std::endl;
            exit(-1);
            break;
        }

        rcv_buf.insert(rcv_buf.end(),tmp_buf,tmp_buf+bytes);

        parse_frame();

    }
}

void ClientConnectionHandler::parse_frame()
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

void ClientConnectionHandler::handle_frame(StateMachine state, std::vector<unsigned char> &data)
{
    std::cout << "handle_frame()" << std::endl;
    (void*)data.data();
    switch(state)
    {
        case StateMachine::WaitingForMove:

            break;

        default:

            break;
    }
}

    // StateMachine state = StateMachine::WaitingForData;

    // while(true)
    // {
    //     switch (state)
    //     {
    //     case StateMachine::WaitingForData:
    //         std::cout << "Waiting for message type..." << std::endl;
    //         get_data_from_server(1);
    //         state = static_cast<StateMachine>(rcv_buf[0]);
    //         std::cout << "Received data type = " << static_cast<int>(rcv_buf[0]) << std::endl;
    //         break;

    //     case StateMachine::BoardState:
    //         get_data_from_server(64);
    //         clear_console();
    //         board.update_board(rcv_buf);
    //         board.draw_board();
    //         state = StateMachine::WaitingForData;
    //         break;

    //     case StateMachine::WaitingForMove:

    //         break;

    //     case StateMachine::Move:

    //         break;

    //     case StateMachine::Error:

    //         break;

    //     default:
    //         std::cout << "Entered default." << std::endl;
    //         break;
    //     }
    // }

    void ClientConnectionHandler::get_data_from_server(int x)
{
    ssize_t received_bytes = 0;
    ssize_t n = 0;

    while (received_bytes < x)
    {
        n = socket.receive_data((rcv_buf.data() + received_bytes), 1);
        if (n == 0)
        {
            std::cout << "Server disconnected." << std::endl;
            exit(-1);
            break;
        }

        if (n < 0)
        {
            std::cout << "Error receiving message." << std::endl;
            exit(-1);
            break;
        }
        received_bytes += n;
        std::cout << "received byte number: " << (int)received_bytes << ", ";
    }

}