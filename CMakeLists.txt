cmake_minimum_required(VERSION 3.16)
project(ChessTCP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(common)

add_executable(client client.cpp)
target_link_libraries(client PRIVATE common)

add_executable(server server.cpp)
target_link_libraries(server PRIVATE common)