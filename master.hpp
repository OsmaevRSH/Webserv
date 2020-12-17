#ifndef WEBSERV_MASTER_HPP
#define WEBSERV_MASTER_HPP

#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

std::string			get_page_text(const std::string& path_to_file);

#endif
