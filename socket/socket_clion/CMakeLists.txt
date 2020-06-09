cmake_minimum_required(VERSION 3.15)
project(socket_clion)

set(CMAKE_CXX_STANDARD 14)
link_libraries(ws2_32 wsock32)
add_executable(socket_clion main.cpp)