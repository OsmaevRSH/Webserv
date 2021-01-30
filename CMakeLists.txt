cmake_minimum_required(VERSION 3.17)
project(Webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(App/Main)
include_directories(App/Client)
include_directories(App/MIME&ERROR_CODE)
include_directories(App/Parse_input_handlers)
include_directories(App/Server)
include_directories(App/Metods/Search_by_configuration)
include_directories(App/Metods/GET)
include_directories(App/Metods/HEAD)
include_directories(App/Metods/PUT)
include_directories(App/Metods/POST)
include_directories(Cgi)
include_directories(Parser)

add_executable(Webserv  App/Main/utils.cpp
                        App/Parse_input_handlers/Parse_input_handler.cpp
                        App/Main/main.cpp
                        App/Server/Server.cpp
                        App/Metods/Autoindex_generate.cpp
                        App/Server/Server_sys_function.cpp
                        App/Metods/Select_metod.cpp
                        App/MIME&ERROR_CODE/MIME_ERROR.cpp
                        App/MIME&ERROR_CODE/MIME_ERROR.hpp
                        App/Metods/GET/GET.cpp
                        App/Metods/GET/GET.hpp
                        App/Metods/HEAD/HEAD.cpp
                        App/Metods/HEAD/HEAD.hpp
                        App/Metods/Search_by_configuration/Search_by_configuration.cpp
                        App/Metods/Search_by_configuration/Search_by_configuration.hpp
                        Parser/Parser.cpp
                        App/Metods/Search_by_configuration/Header.cpp
                        App/Metods/PUT/PUT.cpp
                        App/Metods/PUT/PUT.hpp
                        App/Main/Color.hpp
                        App/Client/Client.hpp App/Metods/POST/POST.cpp App/Metods/POST/POST.hpp)
