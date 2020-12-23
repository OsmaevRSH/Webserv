/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeldora <jeldora@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 16:31:55 by jeldora           #+#    #+#             */
/*   Updated: 2020/12/23 18:10:20 by jeldora          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(const std::string &path_to_config) {
	t_args args;
	args.text = get_page_text(path_to_config);
	args.ew = &_ew;
	args.fragment = args.text;
	parse(args);
}

void Config::parse(t_args args) {
	std::string word;
	std::string dir_content;
	std::vector<std::string> context;

	if (args.route)
		context = args.route_context;
	else if (args.server)
		context = args.server_context;
	else
		context = args.main_context;
	word = get_next_word(args.fragment, args.rel_pos);
	while (word.empty() == false) {
		if (std::find(context.begin(), context.end(), word) == context.end())
			show_error(args);
		select_dir(args, word);
		word = get_next_word(args.fragment, args.rel_pos);
	}
}

std::string get_next_word(std::string text, size_t &pos) {
	std::string ret_word;
	size_t word_len;

	word_len = 0;
	while (pos < text.length() && text.at(pos) == ' ')
		pos++;
	while (pos + word_len < strlen(text.c_str()) && text[pos + word_len] != ' ') {
		std::cout << text[pos + word_len];
		word_len++;
	}
	ret_word = text.substr(pos, word_len);
	pos += word_len;
	return (ret_word);
}

void Config::select_dir(t_args &args, std::string word) {
	t_args new_args;
	new_args.text = args.text;
	new_args.ew = args.ew;
	new_args.fragment = dir_content(args);
	new_args.base_pos = args.base_pos + args.rel_pos;
	args.rel_pos += new_args.fragment.length() + 2;
	if (word == "server")
		server_parse(new_args);
	else if (word == "route")
		route_parse(new_args);
	else if (word == "index")
		index_parse(new_args);
/*	else if (word == "autoindex")
	else if (word == "root")
	else if (word == "max_body_size")
*/

	args.rel_pos += new_args.fragment.length();
}

void Config::server_parse(t_args args) {
	t_server *server = new t_server;
	args.server = server;
	args.ew = &(server->ew);
	parse(args);
	_servers.push_back(*server);
}

void Config::route_parse(t_args args) {
	t_route *route = new t_route;
	args.route = route;
	args.ew = &(route->ew);
	parse(args);
	if (args.server)
		args.server->routes.push_back(*route);
	else
		args.route->routes.push_back(*route);
}

void Config::index_parse(t_args args) {
	std::string word;
	while ((word = get_next_word(args.fragment, args.rel_pos)) != ";" ||
		   word.empty() != false)
		args.ew->index.push_back(word);
}

void Config::max_body_size_parse(t_args args) {
	size_t value;
	std::string tmp;

	tmp = get_next_word(args.fragment, args.rel_pos);
	value = atoi(tmp.c_str());
	args.ew->max_body_size = value;
}

void Config::autoindex_parse(t_args args) {
	std::string value;

	value = get_next_word(args.fragment, args.rel_pos);
	for (size_t i = 0; i < value.length(); i++)
		value[i] = std::tolower(value[i]);

	if (value == "on")
		args.ew->autoindex = true;
	else if (value == "off")
		args.ew->autoindex = false;
	else
		show_error(args);

	std::cout << args.ew->autoindex << "\n";
}

void Config::root_parse(t_args args) {
	args.ew->root = get_next_word(args.fragment, args.rel_pos);
	std::ifstream ifs(args.ew->root.c_str());
	if (!ifs.is_open())
		show_error(args);
}

void show_error(const t_args &args) {
	size_t pos = args.base_pos + args.rel_pos;
	std::string str = args.text;
	size_t line = 1;
	size_t l_pos = 0;
	size_t c_pos = 0;

	if (str.rfind('\n', pos) != std::string::npos)
		c_pos = pos - str.rfind('\n', pos);
	else
		c_pos = pos;
	while ((l_pos = str.rfind('\n', pos)) != std::string::npos)
		line++;
	std::cout << "\nParser error: " << line << " line, " << c_pos << " character.\n";
	exit(1);
}

t_server::s_server() { port = 0; }

t_everywhere::s_everywhere() {
	max_body_size = 0;
	autoindex = false;
}

t_route::s_route() {}

t_args::s_args() {
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
