#ifndef WEBSERV__SERVER_HPP_
#define WEBSERV__SERVER_HPP_

#include "master.hpp"

class Server
{
	private:
		int _master_socket_fd;
		int _family;
		int _type;
		int _protocol;
		int _bind_port;
		std::vector<int> _client_socket_fd;

		void Socket();
		void Bind();
		void Listen() const;
		void Accept();
		_Noreturn void ListenLoop();
	public:
		explicit Server(int bind_port = 8000,
						int family = AF_INET, //AF_UNIX
						int type = SOCK_STREAM, //SOCK_DGRAM
						int protocol = 0); // IPPROTO_TCP and IPPROTO_UDP
		Server(const Server &);
		~Server();
		Server &operator=(const Server &);
		void server_start();
};

#endif
