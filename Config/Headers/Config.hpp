/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 16:02:13 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/23 18:07:48 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "shared.hpp"
#include "Input_handlers.hpp"

namespace ConfigParser
{
	typedef struct					s_everywhere
	{
		std::vector<std::string>	index;
		size_t						max_body_size;
		std::string					root;
		bool						autoindex;

		s_everywhere();
		//s_everywhere(const s_everywhere &copy);
	}							t_everywhere;
	typedef struct					s_location
	{
		std::vector<std::string>	block_args;
		std::vector<std::string>	allow_methods;
		std::vector<struct s_location>	locations;
		t_everywhere				ew;

		s_location();
		//s_location(const s_location &copy);
	}								t_location;
	typedef struct					s_server
	{
		std::string 				ip;
		int							port;
		std::string					server_name;
		std::vector<ConfigParser::t_location>		locations;
		ConfigParser::t_everywhere				ew;
		s_server();
		//s_server(const s_server &copy);
	}								t_server;
	typedef struct					s_args
	{
		std::string					text;
		std::string					fragment;
		size_t						base_pos;
		size_t						rel_pos;
		std::vector<std::string>	block_args;
		t_everywhere				*ew;
		t_location					*location;
		t_server					*server;
		std::vector<std::string>	main_context;
		std::vector<std::string>	location_context;
		std::vector<std::string>	server_context;
		bool 						debug;
		s_args();
		//s_args(const s_args &copy);
	}								t_args;
}

namespace ConfigHandler
{
	typedef struct					s_params
	{
		ConfigParser::t_server		root_location;
		std::string 				path_to_page;
		std::vector<std::string>	allow_methods;
		std::vector<std::string>	index;
		std::string					root;
		bool						autoindex;
		int 						max_body_size;
		std::string					autoindex_page;
	}								t_params;
}

class Config
{
	private:
		std::vector<ConfigParser::t_server>				_servers;
		std::map<int, std::string>						_error_pages; // Ключ - номер страницы. Значение - путь
		ConfigParser::t_everywhere						_ew;

		std::map<std::string, std::string>				___cache;	//Ключ - url, значение - путь

		void parse_server();
		void parse_location();
		void parse(ConfigParser::t_args args);
		void select_dir(ConfigParser::t_args &args, std::string word);
		void server_parse(ConfigParser::t_args args);
		void location_parse(ConfigParser::t_args args);
		void error_page_parse(ConfigParser::t_args args);

		//func for config_handlers
		ConfigParser::t_server get_server(t_headers &);
		template<class T>
		std::string get_path(T &, Input_handlers &, ConfigHandler::t_params &);
		void setup_global_params(ConfigHandler::t_params &global_params, ConfigParser::t_server &, bool);
		std::string recursive_call_with_slash(Input_handlers &handlers, ConfigHandler::t_params &global_params);
		std::string recursive_call_without_slash(Input_handlers &handlers, ConfigHandler::t_params &global_params);
		friend bool search_index(ConfigHandler::t_params &global_params, Input_handlers &handlers);

		bool check_allow_metods(const ConfigHandler::t_params &, Input_handlers &);

		//autoindex
		static std::string create_autoindex_page(ConfigHandler::t_params &, Input_handlers &);
	public:
		Config(const std::string& path_to_config);
		const std::vector<ConfigParser::t_server> &getServers() const;
		const std::map<int, std::string> &getErrorPages() const;
		const ConfigParser::t_everywhere &getEw() const;

		std::string Handler(t_headers &headers, Input_handlers &handlers);
};

