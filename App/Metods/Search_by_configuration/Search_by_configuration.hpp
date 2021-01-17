#pragma once

#include "Server.hpp"
#include "Parse_input_handler.hpp"

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

	s_output() : status_code(200), path_to_file(), autoindex_page(), attached_location(false) {}
} t_output;

typedef struct s_params
{
	t_server root_location;
	std::vector<std::string> allow_methods;
	std::vector<std::string> index;
	std::string root;
	bool autoindex;
	int max_body_size;
} t_params;

class Search_by_configuration
{
	protected:
		Serv_conf _config;
		Parse_input_handler _handler;
		t_output _output;
		MIME_ERROR _mime;

		t_server get_server();
		void setup_global_params(t_params &global_params, t_server &server, bool save_server);
		void update_global_params(t_params &global_params, t_location &location);
		void check_allow_metods(const t_params &param, Parse_input_handler &handlers);
		template<class T>
		void get_path(T &param, Parse_input_handler &handlers, t_params &global_params);
		void recursive_call_with_slash(Parse_input_handler &handlers, t_params &global_params);
		void recursive_call_without_slash(Parse_input_handler &handlers, t_params &global_params);
		bool search_index(t_params &global_params, Parse_input_handler &handlers);

		std::string get_date_handler();
		std::string get_first_line();
		std::string get_content_type();
		virtual std::string get_content_length() = 0;
		std::string get_server_name();
		std::string get_last_modified();
		std::string get_allow_metods();
		void Search_path();
		std::string create_autoindex_page(t_params &params, Parse_input_handler &handler);
	public:
		Search_by_configuration(const Serv_conf &conf, const Parse_input_handler &handler, const MIME_ERROR &);
		~Search_by_configuration();
};

bool check_slash(Parse_input_handler &handlers);
bool search_folder(t_params &params, Parse_input_handler &handlers);
bool search_file(t_params &params, Parse_input_handler &handlers);

void location_sort(std::vector<t_location> &locations);

template<class T>
t_location *check_path_with_complete_coincidence(T &param, Parse_input_handler &handlers);
template<class T>
t_location *check_simple_location(T &param, Parse_input_handler &handlers);