#ifndef WEBSERV__SERVER_HPP_
#define WEBSERV__SERVER_HPP_

#include "master.hpp"

class Server
{
	private:
		int _family;
		int _type;
		int _protocol;
		int _count_servers;
		std::vector<int> _master_socket_fd;
		std::vector<int> _client_socket_fd;
		std::vector<std::map<std::string, std::string> > _servers_config;

		void Socket();
		void Bind();
		void Listen() const;
		void Accept(int);
		void setNonBlocked(int);
		void ListenLoop();
	public:
		explicit Server(const std::vector<std::map<std::string, std::string> > &servers_config,
						int family = AF_INET,
						int type = SOCK_STREAM,
						int protocol = 0);
		Server(const Server &);
		~Server();
		Server &operator=(const Server &);
		void server_start();
};

#endif
