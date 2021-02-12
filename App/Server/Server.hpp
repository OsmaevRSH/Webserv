#pragma once

#include "Parse_request_headers.hpp"
#include "master.hpp"
#include "Parser.hpp"
#include "GetMimeTypeAndErrorPageCode.hpp"
#include "Client.hpp"

typedef std::vector<ConfigParser::t_server> serv_vec;
typedef std::map<int, std::string> errp_map;
typedef ConfigParser::t_everywhere ew_str;

struct Serv_conf
{
	serv_vec _servers;
	errp_map _error_pages;
	ew_str _ew;

	Serv_conf(const serv_vec &servers, const errp_map &errorPages, const ew_str &ew)
			: _servers(servers), _error_pages(errorPages), _ew(ew) {}
};

class Server
{
	private:
		int _family;
		int _type;
		int _protocol;
		fd_set _readfds;
		fd_set _writefds;
		Serv_conf _config;
		MIME_ERROR _mime;
		std::list<Client> _clients;
		std::vector<int> _master_socket_fd;

		char **env;

		void Socket();
		void Bind();
		void Listen() const;
		void Accept(int);
		static void Set_non_blocked(int);
		void ListenLoop();
		void Reset_fd_set();
		void Add_new_fd_to_set();
		void Search_max_fd(int &);
		static void Checkout_call_to_select(const int &);
		void Accept_if_serv_fd_changed();
		void Act_if_readfd_changed(std::list<Client>::iterator &);
		void Act_if_writefd_changed(std::list<Client>::iterator &);
		void Check_read_set();
		void Check_write_set();
		bool Reading_a_request(std::list<Client>::iterator &Iter);
		static char *check_input_handler_buffer(char *input_buffer, std::list<Client>::iterator &);
		void Method_selector(std::string &handler, std::string &body, std::list<Client>::iterator &Iter);
		static bool read_with_content_length(int size, std::list<Client>::iterator &);
		static bool read_with_chunked(std::list<Client>::iterator &);
	public:
		explicit Server(const serv_vec &, const errp_map &, const ew_str &, MIME_ERROR &, char **env, int family = AF_INET, int type = SOCK_STREAM, int protocol = 0);
		~Server();
		void server_start();
};

