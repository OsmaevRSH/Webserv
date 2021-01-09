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
		fd_set _readfds;
		fd_set _writefds;
		std::vector<int> _master_socket_fd;
		std::vector<int> _read_socket_fd;
		std::vector<int> _write_socket_fd;
		std::vector<ConfigParser::t_server> _servers_config;
		Config _config;
		std::map<int, std::vector<std::string> > _request_to_client;

		void Socket();
		void Bind();
		void Listen() const;
		void Accept(int);
		static void Set_non_blocked(int);
		_Noreturn void ListenLoop();
		void Reset_fd_set();
		void Add_new_fd_to_set();
		void Search_max_fd(int &);
		static bool Checkout_call_to_select(const int &);
		void Accept_if_serv_fd_changed();
		void Act_if_readfd_changed(std::vector<int>::iterator &);
		void Act_if_writefd_changed(std::vector<int>::iterator &);
	public:
		explicit Server(const std::vector<ConfigParser::t_server> &servers_config, Config &config, int family = AF_INET, int type = SOCK_STREAM, int protocol = 0);
		Server(const Server &);
		~Server();
		Server &operator=(const Server &);
		void server_start();
};

