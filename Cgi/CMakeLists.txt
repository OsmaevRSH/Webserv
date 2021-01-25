cmake_minimum_required(VERSION 3.17)
project(cgi)

set(CMAKE_CXX_STANDARD 98)
include_directories(../App/Headers)
include_directories(./Headers)

add_executable(cgi Cgi.cpp)