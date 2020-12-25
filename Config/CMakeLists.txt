cmake_minimum_required(VERSION 3.17)
project(parser)

set(CMAKE_CXX_STANDARD 98)

add_executable(parser Config_parser.cpp main.cpp ../get_page_text.cpp)