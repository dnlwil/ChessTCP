#pragma once

#include "../chess/chess_board.hpp"
#include "../socket/base_socket.hpp"
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
    BaseConnectionHandler(BaseSocket socket) : socket(std::move(socket))
    {
        rcv_buf.reserve(100);
        snd_buf.reserve(100);
    }

    ~BaseConnectionHandler() = default;
    void build_frame(StateMachine state, std::span<const unsigned char> data = {});
    void send_data();
    void handle_data();
    void parse_frame();
    virtual void handle_frame(StateMachine state, std::span<const unsigned char> data) = 0;
};