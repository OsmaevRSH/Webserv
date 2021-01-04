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

#define SERVER_DEBUG
//#define CONFIG_DEBUG

// For parser
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>

typedef struct	s_headers
{
	std::string Accept_Charsets;
	std::string Accept_Language;
	std::string Allow;
	std::string Authorization;
	std::string Content_Language;
	std::string Content_Length;
	std::string Content_Location;
	std::string Content_Type;
	std::string Date;
	std::string Host;
	std::string Last_Modified;
	std::string Location;
	std::string Referer;
	std::string Retry_After;
	std::string Server;
	std::string Transfer_Encoding;
	std::string User_Agent;
	std::string WWW_Authenticate;
}				t_headers;

std::string			get_page_text(const std::string& path_to_file);

