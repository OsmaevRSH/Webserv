#ifndef WEBSERV_POST_HPP
#define WEBSERV_POST_HPP
#include "Search_by_configuration.hpp"
#include "Cgi.h"

//typedef struct				s_data_for_cgi
//{
//	std::string 			server_ip;
//	std::string 			client_ip;
//	std::string 			path_info; // Путь относительно директории сервера
//	std::string 			path_translated; // Полный путь в файловой системе
//	std::string 			script_name; // Виртуальный путь к скрипту (http путь)
//
//	int 					port;
//	std::string				body;
//	Parse_input_handler		headers;
//}							t_data_for_cgi;

class POST : public Search_by_configuration
{
	private:
		std::string &_body;
		std::string &_head;
		std::string _handler_body;
		t_data_for_cgi _cgi_struct;
		char **_env;
		std::string get_content_length();
	public:
		POST(const Serv_conf &, const Parse_input_handler &, const MIME_ERROR &, std::string &, std::string &, std::string &, char **env);
		void start_processing();
};


#endif