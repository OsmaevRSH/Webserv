#pragma once

#include "Input_handlers.hpp"
#include "shared.hpp"
#include "Config.hpp"

class Server
{
	private:
		int _family;
		int _type;
		int _protocol;
		int _count_servers;
		std::vector<int> _master_socket_fd;
		std::vector<int> _client_socket_fd;
		std::vector<ConfigParser::t_server> _servers_config;
		Config _config;

		void Socket();
		void Bind();
		void Listen() const;
		void Accept(int);
		static void Set_non_blocked(int);
		_Noreturn void ListenLoop();
		static void Reset_fd_set(fd_set &, fd_set &);
		void Add_new_fd_to_set(fd_set &, std::vector<int>::iterator);
		void Search_max_fd(int &);
		static bool Checkout_call_to_select(const int &);
		void Accept_if_serv_fd_changed(fd_set &);
		void Act_if_client_fd_changed(std::vector<int>::iterator &);
	public:
		explicit Server(const std::vector<ConfigParser::t_server> &servers_config, Config &config, int family = AF_INET, int type = SOCK_STREAM, int protocol = 0);
		Server(const Server &);
		~Server();
		Server &operator=(const Server &);
		void server_start();
};

