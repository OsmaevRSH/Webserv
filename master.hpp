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

// For parser
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>

std::string			get_page_text(const std::string& path_to_file);

