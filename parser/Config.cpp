/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 16:31:55 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/22 17:19:28 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::string	get_next_word(std::string text, size_t pos)
{
	std::string ret_word;
	size_t		word_len;

	while (pos < text.length() && (	text[pos] == ' ' || 
									text[pos] == '\n'))
		pos++;
	while (pos + length < str.length() && (	str[pos + length] != ' ' || 
											str[pos + length] != '{'|| 
											str[pos + length] != '\n'))
		word_len++;
	ret_word = text.substr(pos, word_len);
	return (ret_word);
}

Config::Config(const std::string& path_to_config)
{
	args.text			= get_page_text(path_to_config);
	
}

Config::t_everywhere()
{
	max_body_size = 0;
	autoindex = false;
}
Config::t_route()
{}
Config::t_server()
{
	port = 0;
}
Config::t_args()
{
	base_pos = 0;
	rel_pos = 0;
	ew = NULL;
	route = NULL;
	server = NULL;

	main_context.push_back("index");
	main_context.push_back("max_body_size");
	main_context.push_back("root");
	main_context.push_back("autoindex");
	main_context.push_back("server");
	main_context.push_back("error_page");

	server_context.push_back("index");
	server_context.push_back("max_body_size");
	server_context.push_back("root");
	server_context.push_back("autoindex");
	server_context.push_back("ip");
	server_context.push_back("port");
	server_context.push_back("server_name");
	server_context.push_back("route");

	route_context.push_back("index");
	route_context.push_back("max_body_size");
	route_context.push_back("root");
	route_context.push_back("autoindex");
	route_context.push_back("ip");
	route_context.push_back("port");
	route_context.push_back("server_name");
	route_context.push_back("route");
}

