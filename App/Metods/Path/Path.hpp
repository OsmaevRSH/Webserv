#pragma once

#include "Server.hpp"
#include "Parse_input_handler.hpp"

typedef ConfigParser::t_server t_server;

typedef struct s_output
{
	size_t status_code;
	std::string path_to_file;
	std::string autoindex_page;
} t_output;

typedef struct s_params
{
	t_server root_location;
	std::string path_to_page;
	std::vector<std::string> allow_methods;
	std::vector<std::string> index;
	std::string root;
	bool autoindex;
	int max_body_size;
	std::string autoindex_page;
} t_params;

class Path
{
	protected:
		Serv_conf _config;
		Parse_input_handler _handler;
		t_output _output;

		t_server get_server();
		void setup_global_params(t_params &global_params, t_server &server, bool save_server) const;
		void check_allow_metods(const t_params &param, Parse_input_handler &handlers);
		template<class T>
		std::string get_path(T &param, Parse_input_handler &handlers, t_params &global_params);
		std::string recursive_call_with_slash(Parse_input_handler &handlers, t_params &global_params);
		std::string recursive_call_without_slash(Parse_input_handler &handlers, t_params &global_params);
	public:
		Path(const Serv_conf &conf, const Parse_input_handler &handler);
		~Path();

		void Search_path();
		static std::string create_autoindex_page(t_params &params, Parse_input_handler &handler);
};

bool check_slash(Parse_input_handler &handlers);
bool search_folder(t_params &params, Parse_input_handler &handlers);
bool search_file(t_params &params, Parse_input_handler &handlers);

void update_global_params(t_params &global_params, ConfigParser::t_location &location);
void location_sort(std::vector <ConfigParser::t_location> &locations);

template<class T>
ConfigParser::t_location *check_path_with_complete_coincidence(T &param, Parse_input_handler &handlers);
template<class T>
ConfigParser::t_location *check_simple_location(T &param, Parse_input_handler &handlers);