cmake_minimum_required(VERSION 3.17)
project(Webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(App/Headers)
include_directories(Config/Headers)

add_executable(Webserv App/Srcs/get_page_text.cpp App/Srcs/Input_handlers.cpp App/Srcs/main.cpp App/Srcs/Server.cpp Config/Srcs/Config_parser.cpp Config/Srcs/Config_handler.cpp)