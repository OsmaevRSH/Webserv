#ifndef WEBSERV_MASTER_HPP
#define WEBSERV_MASTER_HPP

#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <fcntl.h>
#include <map>

std::string			get_page_text(const std::string& path_to_file);

#endif
