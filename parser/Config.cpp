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

/*
 * index
 * root
 * autoindex
 */

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
	while (word.empty() == false)
	{
		if (std::find(context.begin(), context.end(), word) == context.end())
			show_error(args);
		select_dir(args, word);
		args.rel_pos++;
		word = get_next_word(args.fragment, args.rel_pos);
	}
}

std::string get_next_word(std::string text, size_t &pos) {
	std::string ret_word;
	size_t word_len;

	word_len = 0;
	while (pos < text.length() && strchr(" \n\t\v\r", text[pos]))
		pos++;
	while (pos + word_len < text.length() && !strchr(" \n\t\v\r", text[pos + word_len])) {
		word_len++;
	}
	if (word_len != 0)
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
	args.rel_pos += new_args.fragment.length();
	if (word == "server")
		server_parse(new_args);
	else if (word == "route")
		route_parse(new_args);
	else if (word == "index")
		index_parse(new_args);
	else if (word == "autoindex")
		autoindex_parse(new_args);
	else if (word == "root")
		root_parse(new_args);
	else if (word == "max_body_size")
		max_body_size_parse(new_args);
	else if (word == "error_page")
		error_page_parse(new_args);
}

// Main
void Config::server_parse(t_args args) {
	t_server *server = new t_server;
	args.server = server;
	args.ew = &(server->ew);
	std::cout << "server {\n";
	parse(args);
	_servers.push_back(*server);
	std::cout << "}\n";
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
void Config::error_page_parse(t_args args)
{
	std::string			tmp;
	int 				page;
	std::vector<int>	pages;

	while (!(tmp = get_next_word(args.fragment, args.rel_pos)).empty())
	{
		page = atoi(tmp.c_str());
		if (page == 0)
			break;
		pages.push_back(page);
	}
	if (atoi(tmp.c_str()) != 0)
		show_error(args);
	std::ifstream ifs(tmp.c_str());
	if (!ifs.is_open())
		show_error(args);
	while (!pages.empty())
	{
		_error_pages.insert(std::pair<int, std::string>(pages.back(), tmp));
		pages.pop_back();
	}
	for (std::map<int, std::string>::iterator i = _error_pages.begin(); i != _error_pages.end(); ++i)
		std::cout << (*i).first << ":" << (*i).second << "\n";
}
// Everywhere
void Config::index_parse(t_args args) {
	std::string word;
	while (!(word = get_next_word(args.fragment, args.rel_pos)).empty())
		args.ew->index.push_back(word);
	if (args.rel_pos != args.fragment.length())
		show_error(args);
	for (int i = 0; i < args.ew->index.size(); i++)
		std::cout << "Index " << i << ": " << args.ew->index[i] <<"\n";
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
	if (args.rel_pos != args.fragment.length())
		show_error(args);
	std::cout << "Autoindex:" << args.ew->autoindex << "\n";
}
void Config::root_parse(t_args args) {
	args.ew->root = get_next_word(args.fragment, args.rel_pos);
	std::ifstream ifs(args.ew->root.c_str());
	if (!ifs.is_open())
		show_error(args);
	if (args.rel_pos != args.fragment.length())
		show_error(args);
	std::cout << "Root:" << args.ew->root << "\n";
}
void Config::max_body_size_parse(t_args args) {
	int value;
	std::string tmp;

	tmp = get_next_word(args.fragment, args.rel_pos);
	value = atoi(tmp.c_str());
	args.ew->max_body_size = value;
	std::cout << "Max_body_size: " << value << "\n";
}
// Constructors
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
// Other
void show_error(const t_args &args) {
	size_t pos = args.base_pos + args.rel_pos;
	std::string str = args.text;
	size_t line = 1;
	size_t l_pos = pos;
	size_t c_pos = 0;

	size_t s1 = str.length();
	size_t s2 = str.rfind('\n', pos);
	if (pos == str.rfind('\n', pos))
		c_pos = 0;
	else if (str.rfind('\n', pos) < str.length())
	{
		c_pos = pos - str.rfind('\n', pos) - 1;
		if (c_pos == std::string::npos)
			c_pos = 1;
	}
	else
		c_pos = pos;
	while ((l_pos = str.rfind('\n', l_pos)) < str.length()) {
		line++;
		l_pos--;
	}
	std::cout << "\nParser error: " << line << " line, " << c_pos << " character.\n";
	exit(1);
}