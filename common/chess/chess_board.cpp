#include "chess_board.hpp"

void ChessBoard::draw_board()
{
    for (unsigned char i = 0u; i < BOARD_SIZE; i++)
    {
        std::cout << board[i] << " ";
        if (((i + 1) % ROW) == 0u)
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

void clear_console()
{
    std::cout << "\033[2J\033[H";
}