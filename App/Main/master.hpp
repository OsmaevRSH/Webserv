#pragma once

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
#include <ostream>
#include <sys/stat.h>
#include <sstream>
#include <dirent.h>
#include <ctime>

//#define TESTER
#define SERVER_IP_DEBUG
//#define CONFIG_DEBUG
//#define AUTOINDEX_DEBUG

/* For parser */
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>

class Parse_input_handler;

std::string			get_page_text(const std::string& path_to_file);