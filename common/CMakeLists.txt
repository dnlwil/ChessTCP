add_library(common STATIC
    socket/base_socket.cpp
    socket/client_socket.cpp
    socket/server_socket.cpp

    connection/base_connection_handler.cpp
    connection/client_connection_handler.cpp
    connection/server_connection_handler.cpp

    chess/chess_board.cpp
)

target_include_directories(common PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)