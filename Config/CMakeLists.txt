cmake_minimum_required(VERSION 3.17)
project(parser)

set(CMAKE_CXX_STANDARD 98)
include_directories(../App/Headers)
include_directories(./Headers)

add_executable(parser Srcs/Config_parser.cpp Srcs/Config_handler.cpp Test/main.cpp ../App/Srcs/get_page_text.cpp)