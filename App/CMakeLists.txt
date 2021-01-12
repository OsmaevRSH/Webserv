cmake_minimum_required(VERSION 3.17)
project(Webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(Headers)
include_directories(../Config/Headers)

add_executable(Webserv Server/Server.cpp Server/Server.hpp Srcs/main.cpp Headers/master.hpp Srcs/get_page_text.cpp Parce_input_handlers/Parce_input_handler.cpp Parce_input_handlers/Parce_input_handler.hpp ../Config/Srcs/Config_parser.cpp)