cmake_minimum_required(VERSION 3.17)
project(parser)

set(CMAKE_CXX_STANDARD 98)
include_directories(../App/Headers)

add_executable(parser Config_parser.cpp Config_handler.cpp main.cpp ../App/Srcs/get_page_text.cpp)