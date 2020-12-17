#ifndef WEBSERV__SERVER_HPP_
#define WEBSERV__SERVER_HPP_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Server
{
	private:
		int _socket_fd;
		int _family;
		int _type;
		int _protocol;
		int _bind_port;
		int _client_socket_fd; //cast to vector

		void Socket();
		void Bind();
		void Listen();
		void Accept();
	public:
		explicit Server(int bind_port = 7655,
						int family = AF_INET, //AF_UNIX
						int type = SOCK_STREAM, //SOCK_DGRAM
						int protocol = 0); // IPPROTO_TCP and IPPROTO_UDP
		Server(const Server &);
		~Server();
		Server &operator=(const Server &);
		void server_start();
};

#endif
