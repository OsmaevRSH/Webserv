#pragma once

#include "Server.hpp"
#include "Parse_request_headers.hpp"

typedef ConfigParser::t_server t_server;
typedef ConfigParser::t_location t_location;

typedef struct s_output
{
	size_t status_code;
	std::string path_to_file;
	std::string autoindex_page;
	bool attached_location;
	t_location location;
	std::string root;
	std::string alias;
	std::string curent_location;
	int			port;

	s_output() : status_code(200), path_to_file(), autoindex_page(), attached_location(false) {}
} t_output;

typedef struct s_params
{
	t_server root_location;
	std::vector<std::string> allow_methods;
	std::vector<std::string> index;
	std::string root;
	std::string alias;
	std::string curent_location;
	bool autoindex;
	int max_body_size;
} t_params;

class Search_by_configuration
{
	protected:
		Serv_conf _config;
		Parse_request_headers _handler;
		t_output _output;
		MIME_ERROR _mime;
		std::list<Client>::iterator &_iter;

		t_server get_server();
		void setup_global_params(t_params &global_params, t_server &server, bool save_server);
		void update_global_params(t_params &global_params, t_location &location);
		void check_allow_metods(const t_params &param, Parse_request_headers &handlers);
		template<class T>
		void get_path(T &param, Parse_request_headers &handlers, t_params &global_params);
		void recursive_call_with_slash(Parse_request_headers &handlers, t_params &global_params);
		void recursive_call_without_slash(Parse_request_headers &handlers, t_params &global_params);
		bool search_index(t_params &global_params, Parse_request_headers &handlers, const std::string &indexPageDirectory);

		std::string get_date_handler();
		std::string get_first_line();
		std::string get_content_type();
		virtual std::string get_content_length() = 0;
		std::string get_server_name();
		std::string get_last_modified();
		std::string get_allow_methods();
		void Search_path();
		std::string create_autoindex_page(t_params &params, Parse_request_headers &handler);
		std::string get_content_lang(const std::map<std::string, std::string> &headers, const std::string &body);
		void check_users(const t_params &param, Parse_request_headers &handlers);
	public:
		Search_by_configuration(const Serv_conf &conf, const Parse_request_headers &handler, const MIME_ERROR &, std::list<Client>::iterator &);
		~Search_by_configuration();
		void check_body_size(const t_params &param, Parse_request_headers &handlers);
};

bool check_slash(Parse_request_headers &handlers);
bool search_folder(t_params &params, Parse_request_headers &handlers, std::string &directoryForIndexFile);
bool search_file(t_params &params, Parse_request_headers &handlers);

void location_sort(std::vector<t_location> &locations);

template<class T>
t_location *check_path_with_complete_coincidence(T &param, Parse_request_headers &handlers);
template<class T>
t_location *check_simple_location(T &param, Parse_request_headers &handlers);
template<class T>
t_location *check_path_with_simple_regex(T &param, Parse_request_headers &handlers);
