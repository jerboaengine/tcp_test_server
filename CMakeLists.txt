cmake_minimum_required(VERSION 3.5)

project(tcp_test_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tcp_test_server 
                main.cpp
                server/tcpserver.cpp
                logic/core.cpp
                controller/agent.cpp
                data/source.cpp)

include_directories("include/")

install(TARGETS tcp_test_server
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
