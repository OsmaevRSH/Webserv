cmake_minimum_required(VERSION 3.17)
project(Webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(Webserv Server.cpp Server.hpp main.cpp master.hpp get_page_text.cpp)