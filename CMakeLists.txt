cmake_minimum_required(VERSION 3.17)
project(Webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(App/Headers)
include_directories(Config/Headers)
include_directories(App/Server)
include_directories(App/MIME_types)

add_executable(Webserv  App/Srcs/get_page_text.cpp
        App/Parce_input_handlers/Parce_input_handler.cpp
                        App/Srcs/main.cpp
                        App/Server/Server.cpp
                        Config/Srcs/Config_parser.cpp
                        Config/Srcs/Config_handler.cpp
                        Config/Srcs/Autoindex_generate.cpp
                        Config/Srcs/Check_allow_metods.cpp
                        App/Srcs/Check_input_handler_buffer.cpp
                        App/Server/Server_sys_function.cpp
                        App/Metods/GET/GET.cpp
                        App/Metods/GET/GET.hpp
                        App/Metods/Select_metod.cpp
                        App/MIME_types/MIME.cpp
                        App/MIME_types/MIME.hpp)