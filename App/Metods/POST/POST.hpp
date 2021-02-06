#ifndef WEBSERV_POST_HPP
#define WEBSERV_POST_HPP

#include "Search_by_configuration.hpp"
#include "Cgi.h"

class POST : public Search_by_configuration
{
	private:
		Cgi *_cgi;
		std::string &_body;
		std::string &_head;
		std::string _handler_body;
		t_data_for_cgi _cgi_struct;
		char **_env;
		std::string get_content_length();
	public:
		POST(const Serv_conf &, std::list<Client>::iterator &Iter, const MIME_ERROR &, std::string &, std::string &, std::string &, char **env);
		~POST();
		void start_processing();
		void get_header_if_error();
		void get_header_if_not_error();
		static bool check_сgi_extension(const std::string &url);
		void init_cgi_struct();
};


#endif