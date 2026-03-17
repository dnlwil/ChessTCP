#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <cstring>

constexpr unsigned char BOARD_SIZE = 64u;
constexpr unsigned char ROW = 8u;
constexpr std::array<unsigned char, BOARD_SIZE> INIT_BOARD =
{
'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
};

void clear_console()
{
    std::cout << "\033[2J\033[H";
}

class ChessBoard
{
private:
    std::array<unsigned char, BOARD_SIZE> board;

public:
    ChessBoard();
    void draw_board();
    std::array<unsigned char, BOARD_SIZE> get_board();
    void update_board(const std::vector<unsigned char> &data);
};


ChessBoard::ChessBoard() : board(INIT_BOARD)
{

}

void ChessBoard::draw_board()
{
    for (unsigned char i = 0u; i < BOARD_SIZE; i++)
    {
        std::cout << (int)board[i] << " ";
        if(((i+1) % ROW) == 0u)
            std::cout << std::endl;
    }
}

void ChessBoard::update_board(const std::vector<unsigned char> &data)
{
    std::memcpy(board.data(), data.data(), BOARD_SIZE);
}

std::array<unsigned char, BOARD_SIZE> ChessBoard::get_board()
{
    return board;
}