/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 16:02:13 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/23 05:00:08 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


//#include "master.hpp"
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

typedef struct					s_everywhere
{
	std::vector<std::string>	index;
	size_t						max_body_size;
	std::string					root;
	bool						autoindex;

	s_everywhere();
}								t_everywhere;
typedef struct					s_route
{
	std::vector<std::string>	allow_methods;
	std::vector<struct s_route>	routes;
	t_everywhere				ew;

	s_route();
}								t_route;
typedef struct					s_server
{
	std::string 				ip;
	int							port;
	std::string					server_name;
	std::vector<t_route>		routes;
	t_everywhere				ew;
	s_server();
}								t_server;
typedef struct					s_args
{
	std::string					text;
	std::string					fragment;
	size_t						base_pos;
	size_t						rel_pos;
	t_everywhere				*ew;
	t_route						*route;
	t_server					*server;
	std::vector<std::string>	main_context;
	std::vector<std::string>	route_context;
	std::vector<std::string>	server_context;

	s_args();
}								t_args;

class Config
{
	private:
		std::vector<t_server>				_servers;		
		std::map<int, std::string>			_error_pages; // Ключ - номер страницы. Значение - путь
		t_everywhere						_ew;
		void parse_server();
		void parse_route();
		void parse(t_args args);
		void select_dir(t_args &args, std::string word);
		void server_parse(t_args args);
		void route_parse(t_args args);
		void index_parse(t_args args);
		void max_body_size_parse(t_args args);
		void autoindex_parse(t_args args);
		void root_parse(t_args args);
	public:
		Config(const std::string& path_to_config);
};

std::string		get_next_word(std::string text, size_t &pos);
void			show_error(const t_args &args);
std::string		dir_content(t_args args);
std::string		get_page_text(const std::string &path_to_file);