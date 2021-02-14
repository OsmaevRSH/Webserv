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
#include "master.hpp"

namespace ConfigParser
{
	typedef struct					s_everywhere
	{
		std::vector<std::string>	index;
		size_t						max_body_size;
		std::string					root;
		std::string 				alias;
		bool						autoindex;

		s_everywhere();
	}								t_everywhere;
	typedef struct					s_location
	{
		std::vector<std::string>		block_args;
		std::vector<std::string>		allow_methods;
		std::vector<struct s_location>	locations;
		std::string 					cgi_path;
		std::string 					cgi_extension;
		t_everywhere					ew;
		std::vector<std::string>		users;
		bool 							is_auth;
		s_location();
	}								t_location;
	typedef struct					s_server
	{
		std::string 				ip;
		int							port;
		std::string					server_name;
		std::vector<ConfigParser::t_location>		locations;
		ConfigParser::t_everywhere				ew;
		s_server();
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
	}								t_args;
}

class Parser
{
	private:
		std::vector<ConfigParser::t_server>				_servers;
		std::map<int, std::string>						_error_pages; // Ключ - номер страницы. Значение - путь
		ConfigParser::t_everywhere						_ew;

		//void parse_server();
		//void parse_location();
		void parse(ConfigParser::t_args args);
		void select_dir(ConfigParser::t_args &args, std::string word);
		void server_parse(ConfigParser::t_args args);
		void location_parse(ConfigParser::t_args args);
		void error_page_parse(ConfigParser::t_args args);
		bool unique_ports();

	public:
		Parser(const std::string& path_to_config);
		~Parser();
		const std::vector<ConfigParser::t_server> &getServers() const;
		const std::map<int, std::string> &getErrorPages() const;
		const ConfigParser::t_everywhere &getEw() const;

};

