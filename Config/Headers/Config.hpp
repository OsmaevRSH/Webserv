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
	typedef struct					s_route
	{
		std::vector<std::string>	block_args;
		std::vector<std::string>	allow_methods;
		std::vector<struct s_route>	routes;
		t_everywhere				ew;

		s_route();
		//s_route(const s_route &copy);
	}								t_route;
	typedef struct					s_server
	{
		std::string 				ip;
		int							port;
		std::string					server_name;
		std::vector<ConfigParser::t_route>		routes;
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
		t_route						*route;
		t_server					*server;
		std::vector<std::string>	main_context;
		std::vector<std::string>	route_context;
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
		std::string 				path_to_page;
		std::string					ip;
		int 						port;
		std::string 				server_name;
		std::vector<std::string>	allow_methods;
		int 						max_body_size;
	}								t_params;
}

class Config
{
	private:
		std::vector<t_server>				_servers;
		std::map<int, std::string>			_error_pages; // Ключ - номер страницы. Значение - путь
		ConfigParser::t_everywhere			_ew;

		void parse_server();
		void parse_route();
		void parse(ConfigParser::t_args args);
		void select_dir(ConfigParser::t_args &args, std::string word);
		void server_parse(ConfigParser::t_args args);
		void route_parse(ConfigParser::t_args args);
		void error_page_parse(ConfigParser::t_args args);

	public:
		Config(const std::string& path_to_config);
		const std::vector<t_server> &getServers() const;
		const std::map<int, std::string> &getErrorPages() const;
		const ConfigParser::t_everywhere &getEw() const;

		std::string Handler(t_headers headers);
	// Вернуть вектор с серверами
};

