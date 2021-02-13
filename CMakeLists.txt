cmake_minimum_required(VERSION 3.17)

project(Webserv)

set(CMAKE_CXX_STANDARD 11)

include_directories(App/Main)
include_directories(App/Client)
include_directories(App/MimeType_And_ErrorPageCode)
include_directories(App/Parse_request_headers)
include_directories(App/Server)
include_directories(App/Metods/Search_by_configuration)
include_directories(App/Metods/GET)
include_directories(App/Metods/HEAD)
include_directories(App/Metods/PUT)
include_directories(App/Metods/POST)
include_directories(App/Cgi)
include_directories(App/Parser)

add_executable(Webserv  App/Main/utils.cpp
                        App/Parse_request_headers/Parse_request_headers.cpp
                        App/Main/main.cpp
                        App/Server/Server.cpp
                        App/Metods/Autoindex_generate.cpp
                        App/Metods/Select_metod.cpp
                        App/MimeType_And_ErrorPageCode/GetMimeTypeAndErrorPageCode.cpp
                        App/Metods/GET/GET.cpp
                        App/Metods/HEAD/HEAD.cpp
                        App/Metods/Search_by_configuration/Search_by_configuration.cpp
                        App/Metods/Search_by_configuration/Header.cpp
                        App/Metods/PUT/PUT.cpp
                        App/Metods/POST/POST.cpp
                        App/Cgi/Cgi.cpp
                        App/Cgi/meta_variables.cpp
                        App/Parser/Parser.cpp)

