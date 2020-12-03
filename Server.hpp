#ifndef WEBSERV__SERVER_HPP_
#define WEBSERV__SERVER_HPP_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Server
{
	private:
		int _socket_fd;
		int _family;
		int _type;
		int _protocol;
		int _bind_port;
		int _client_socket_fd;

		void Socket();
		void Bind();
		void Listen();
		void Accept();
	public:
		explicit Server(int bind_port = 7654, int family = AF_INET,
						int type = SOCK_STREAM,
						int protocol = 0);
		Server(const Server &);
		~Server();
		Server &operator=(const Server &);
		void server_start();
};

#endif
