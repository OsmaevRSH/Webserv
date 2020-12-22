/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 16:31:55 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/22 19:18:42 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::string	get_next_word(std::string text, size_t &pos)
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
	pos += word_len;
	return (ret_word);
}

Config::Config(const std::string& path_to_config)
{
	t_args args;
	args.text			= get_page_text(path_to_config);
	args.ew 			= &_ew;
	args.fragment = args.text;
	parse(args);
	parse()
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

void				show_error(t_args args)
{
	size_t pos = args.base_pos + args.rel_pos;
	std::string str = args.text;
	size_t	line = 1;
	size_t	l_pos = 0;
	size_t	c_pos = 0;

	if (str.rfind('\n', pos) != std::string::npos)
		c_pos = pos - str.rfind('\n', pos);
	else
		c_pos = pos;
	while ((l_pos = str.rfind('\n', pos)) != std::string::npos)
		line++;
	std::cout << "\nParser error: " << l_pos << " line, " << c_pos << " character.\n";
	exit(1);
}