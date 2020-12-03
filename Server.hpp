#ifndef WEBSERV__SERVER_HPP_
#define WEBSERV__SERVER_HPP_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
	private:
		int _socket_fd;
		int _family;
		int _type;
		int _protocol;

		void Socket();
		void Bind();
	public:
		explicit Server(int family = AF_INET,
						int type = SOCK_STREAM,
						int protocol = 0);
		Server(const Server &);
		~Server();
		Server &operator=(const Server &);
};

#endif
