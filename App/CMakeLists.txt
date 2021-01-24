cmake_minimum_required(VERSION 3.17)
project(Webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(Headers)
include_directories(../Parser/Headers)

add_executable(Webserv Server/Server.cpp Server/Server.hpp Main/main.cpp Main/master.hpp Main/utils.cpp Parse_input_handlers/Parse_input_handler.cpp Parse_input_handlers/Parse_input_handler.hpp ../Parser/Parser.cpp)