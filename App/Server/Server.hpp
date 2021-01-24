#pragma once

#include "Parse_input_handler.hpp"
#include "master.hpp"
#include "../../Config/Parser.hpp"
#include "MIME_ERROR.hpp"

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
		std::vector<int> _read_socket_fd;
		std::vector<int> _write_socket_fd;
		std::vector<int> _master_socket_fd;
		std::map<int, Parse_input_handler *> _edited_headers; //map для хранения указателей на класс с обратанным заголовком клиента
		std::map<int, std::string> _request_header; //map для хранения заголовка запроса
		std::map<int, std::string> _request_body; //map для хранения тела запроса
		std::map<int, std::string> _ready_response_to_the_customer; //map для хренения готового ответа для клиента
		std::map<int, std::string> _chunked_end_check; //map для проверки chunked окончания в случае прихода 0
		std::map<int, std::string> _server_client_ip; //map для хренения на какой ip сервера пришло соединение

		void Socket();
		void Bind();
		void Listen() const;
		void Accept(int);
		static void Set_non_blocked(int);
		_Noreturn void ListenLoop();
		void Reset_fd_set();
		void Add_new_fd_to_set();
		void Search_max_fd(int &);
		static void Checkout_call_to_select(const int &);
		void Accept_if_serv_fd_changed();
		void Act_if_readfd_changed(std::vector<int>::iterator &);
		void Act_if_writefd_changed(std::vector<int>::iterator &);
		void Check_read_set();
		void Check_write_set();
		bool Reading_a_request(std::vector<int>::iterator &Iter);
		char *check_input_handler_buffer(char *input_buffer, std::vector<int>::iterator &);
		void Method_selector(const Parse_input_handler &inputHandlers, std::string &handler, std::string &body, std::string &handler_body);
		void read_with_content_length(int size, int fd);
		bool read_with_chunked(int);
	public:
		explicit Server(const serv_vec &, const errp_map &, const ew_str &, MIME_ERROR &, int family = AF_INET, int type = SOCK_STREAM, int protocol = 0);
		~Server();
		void server_start();
};

