#pragma once

#include "Server.hpp"
#include "Parse_input_handler.hpp"

namespace ConfigHandler
{
	typedef struct s_params
	{
		ConfigParser::t_server root_location;
		std::string path_to_page;
		std::vector<std::string> allow_methods;
		std::vector<std::string> index;
		std::string root;
		bool autoindex;
		int max_body_size;
		std::string autoindex_page;
	} t_params;
}

class Path_to_file
{
	private:
		Serv_conf _config;
		Parse_input_handler _handler;

		ConfigParser::t_server get_server();
		void setup_global_params(ConfigHandler::t_params &global_params, ConfigParser::t_server &server, bool save_server);
		bool check_allow_metods(const ConfigHandler::t_params &param, Parse_input_handler &handlers);
		static std::string create_autoindex_page(ConfigHandler::t_params &params, Parse_input_handler &handler);
		template<class T>
		std::string get_path(T &param, Parse_input_handler &handlers, ConfigHandler::t_params &global_params);
		std::string recursive_call_with_slash(Parse_input_handler &handlers, ConfigHandler::t_params &global_params);
		std::string recursive_call_without_slash(Parse_input_handler &handlers, ConfigHandler::t_params &global_params);
	public:
		Path_to_file(const Serv_conf &conf, const Parse_input_handler &handler)
				: _config(conf), _handler(handler) {};
		Path_to_file(const Path_to_file &copy):_config(copy._config), _handler(copy._handler) {};
		~Path_to_file() {};

		std::string Path();
};
