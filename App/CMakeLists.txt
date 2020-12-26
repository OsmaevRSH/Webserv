cmake_minimum_required(VERSION 3.17)
project(Webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(Webserv Srcs/Server.cpp Headers/Server.hpp Srcs/main.cpp Headers/master.hpp Srcs/get_page_text.cpp Srcs/Input_handlers.cpp Headers/Input_handlers.hpp)