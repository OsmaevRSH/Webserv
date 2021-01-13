cmake_minimum_required(VERSION 3.17)
project(Webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(App/Headers)
include_directories(Config/Headers)
include_directories(App/Server)
include_directories(App/MIME_types)
include_directories(App/Parse_input_handlers)

add_executable(Webserv App/Main/utils.cpp
                        App/Parse_input_handlers/Parse_input_handler.cpp
        App/Main/main.cpp
                        App/Server/Server.cpp
                        Config/Srcs/Config_parser.cpp
                        App/Metods/Autoindex_generate.cpp
                        App/Server/Server_sys_function.cpp
                        App/Metods/GET/GET.cpp
                        App/Metods/GET/GET.hpp
                        App/Metods/Select_metod.cpp
                        App/MIME_types/MIME.cpp
                        App/MIME_types/MIME.hpp
                        App/Metods/HEAD/HEAD.cpp
                        App/Metods/HEAD/HEAD.hpp
                        App/Metods/Path/Path.cpp
                        App/Metods/Path/Path.hpp)