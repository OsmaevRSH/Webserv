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

#include "Parser.hpp"

typedef ConfigParser::t_args 			t_args;
typedef ConfigParser::t_everywhere 		t_everywhere;
typedef ConfigParser::t_location 		t_location;
typedef ConfigParser::t_server 			t_server;

// Utils:
static void 					show_error(const t_args &args, const std::string &message) {
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
	std::cout << message;
	exit(1);
}
static std::string 				get_next_word(std::string text, size_t &pos) {
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
static std::string 				string_parse(t_args args)
{
	std::string str = get_next_word(args.fragment, args.rel_pos);
	if (str == "")
		show_error(args, "Empty argument");
	if (args.rel_pos != args.fragment.length())
		show_error(args, "Excessive argument");
	return str;
}
static std::vector<std::string> pre_block_arg(const t_args &args)
{
	size_t						len = args.fragment.find('{', args.rel_pos) - args.rel_pos - 1;
	size_t						pos = 0;
	std::string					word;
	std::vector<std::string>	ret;

	std::string text = args.fragment.substr(args.rel_pos + 1, len);
	while (!(word = get_next_word(text, pos)).empty())
		ret.push_back(word);
	return ret;
}
static std::string				block_directive(const std::string &text, size_t &pos)
{
	size_t	start = text.find('{', pos);

	start++;
	for (int i(0), count(1); i < static_cast<int>(text.length()); ++i)
	{
		if (text[start + i] == '{')
			count++;
		else if (text[start + i] == '}')
			count--;
		if (count == 0)
		{
			pos = start + i;
			return text.substr(start, i);
		}
	}
	return std::string();
}
static std::string				dir_content(t_args &args)
{
	size_t 	simple = args.fragment.find(';', args.rel_pos);
	size_t 	block = args.fragment.find('{', args.rel_pos);

	if (simple == block)
		show_error(args, "Expected \'{\' or \';\'\n");
	if (block < simple)
		return (block_directive(args.fragment, args.rel_pos));
	else
		return (args.fragment.substr(args.rel_pos, args.fragment.find(';', args.rel_pos) - args.rel_pos));
}

// Non-class parse functions (this functions do not use class variables and methods):
static void 					allow_methods_parse(t_args args)
{
	std::string word;
	while (!(word = get_next_word(args.fragment, args.rel_pos)).empty())
		args.location->allow_methods.push_back(word);
#ifdef CONFIG_DEBUG
	for (int i = 0; i < args.location->allow_methods.size(); i++)
		std::cout << "Allow methods: " << args.location->allow_methods[i] <<"\n";
#endif
	/*
	 * Сделать проверку на наличие существование этих методов!
	 */
}
static void 					index_parse(t_args args) {
	std::string word;
	while (!(word = get_next_word(args.fragment, args.rel_pos)).empty())
		args.ew->index.push_back(word);
	if (args.rel_pos != args.fragment.length())
		show_error(args, "Error in index directive\n");
#ifdef CONFIG_DEBUG
	for (int i = 0; i < args.ew->index.size(); i++)
		std::cout << "Index " << i << ": " << args.ew->index[i] <<"\n";
#endif
}
static void 					autoindex_parse(t_args args) {
	std::string value;

	value = get_next_word(args.fragment, args.rel_pos);
	for (size_t i = 0; i < value.length(); i++)
		value[i] = std::tolower(value[i]);

	if (value == "on")
		args.ew->autoindex = true;
	else if (value == "off")
		args.ew->autoindex = false;
	else
		show_error(args, "Invalid autoindex mode\n");
	if (args.rel_pos != args.fragment.length())
		show_error(args, "Autoindex error\n");
#ifdef CONFIG_DEBUG
	std::cout << "Autoindex:" << args.ew->autoindex << "\n";
#endif
}
static void 					root_parse(t_args args) {
	args.ew->root = get_next_word(args.fragment, args.rel_pos);
	std::ifstream ifs(args.ew->root.c_str());
	if (!ifs.is_open())
		show_error(args, "Root folder is not exist\n");
	if (args.rel_pos != args.fragment.length())
		show_error(args, "Root error\n");
#ifdef CONFIG_DEBUG
	std::cout << "Root:" << args.ew->root << "\n";
#endif
}
static void 					max_body_size_parse(t_args args) {
	int value;
	std::string tmp;

	tmp = get_next_word(args.fragment, args.rel_pos);
	value = atoi(tmp.c_str());
	args.ew->max_body_size = value;
#ifdef CONFIG_DEBUG
	std::cout << "Max_body_size: " << value << "\n";
#endif
}

// Class parse functions:
void Parser::					parse(t_args args) {
	std::string word;
	std::string dir_content;
	std::vector<std::string> context;

	if (args.location)
		context = args.location_context;//
	else if (args.server)
		context = args.server_context;
	else
		context = args.main_context;
	word = get_next_word(args.fragment, args.rel_pos);
	while (word.empty() == false)
	{
		if (std::find(context.begin(), context.end(), word) == context.end())
			show_error(args, "Invalid directive name or invalid context\n");
		select_dir(args, word);
		args.rel_pos++;
		word = get_next_word(args.fragment, args.rel_pos);
	}
}
void Parser::					select_dir(t_args &args, std::string word) {
	t_args new_args;
	new_args.text = args.text;
	new_args.ew = args.ew;
	new_args.base_pos = args.base_pos + args.rel_pos;
	new_args.server = args.server;
	new_args.location = args.location;
	new_args.block_args = pre_block_arg(args);
	new_args.fragment = dir_content(args);
	if (word != "server" && word != "location")
		args.rel_pos += new_args.fragment.length();
	if (word == "server")
		server_parse(new_args);
	else if (word == "location")
		location_parse(new_args);
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
	else if (word == "ip") {
		new_args.server->ip = string_parse(new_args);
#ifdef CONFIG_DEBUG
		std::cout << "Ip: " << new_args.server->ip << "\n";
#endif
	}
	else if (word == "server_name") {
		new_args.server->server_name = string_parse(new_args);
#ifdef CONFIG_DEBUG
		std::cout << "Server_name: " << new_args.server->server_name << "\n";
#endif
	}
	else if (word == "port") {
		new_args.server->port = atoi(string_parse(new_args).c_str());
#ifdef CONFIG_DEBUG
		std::cout << "Port: " << new_args.server->port << "\n";
#endif
	}
	else if (word == "allow")
		allow_methods_parse(new_args);
	else if (word == "cgi")
		args.location->cgi_path = string_parse(new_args);
	else if (word == "alias")
	{
		args.ew->alias = string_parse(new_args);
	}
}
void Parser::					server_parse(t_args args) {
	if (!args.block_args.empty())
		show_error(args, "Server directive must not have block-directive arguments\n");
	t_server *server = new t_server;
	args.server = server;
	args.ew = &(server->ew);
#ifdef CONFIG_DEBUG
	std::cout << "server {\n";
#endif
	parse(args);
	_servers.push_back(*server);
#ifdef CONFIG_DEBUG
	std::cout << "}\n";
#endif
}
void Parser::					error_page_parse(t_args args)
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
		show_error(args, "Expected file of error page\n");
	std::ifstream ifs(tmp.c_str());
	if (!ifs.is_open())
		show_error(args, "File is not exist\n");
	while (!pages.empty())
	{
		_error_pages.insert(std::pair<int, std::string>(pages.back(), tmp));
		pages.pop_back();
	}
#ifdef CONFIG_DEBUG
	for (std::map<int, std::string>::iterator i = _error_pages.begin(); i != _error_pages.end(); ++i)
		std::cout << (*i).first << ":" << (*i).second << "\n";
#endif
}
void Parser::					location_parse(t_args args) {
	t_location *location = new t_location;
	t_location *parent_location = args.location;
	location->block_args = args.block_args;
	args.location = location;
	args.ew = &(location->ew);
#ifdef CONFIG_DEBUG
	std::cout << "location ";
	for (std::vector<std::string>::iterator i = location->block_args.begin(); i < location->block_args.end(); ++i)
		std::cout << *i << " ";
	std::cout << "{\n";
#endif
	parse(args);
	if (parent_location)
		parent_location->locations.push_back(*location);
	else if (args.server)
		args.server->locations.push_back(*location);
	else
		show_error(args, "Nested directive error\n");
#ifdef CONFIG_DEBUG
	std::cout << "}\n";
#endif
}

// Constructors
Parser::						Parser(const std::string &path_to_config) {
	t_args args;
	args.text = get_page_text(path_to_config);
	args.ew = &_ew;
	args.fragment = args.text;
	parse(args);
}

// Getters
const std::vector<t_server> 	&Parser::getServers() const {
	return _servers;
}
const std::map<int, 			std::string> &Parser::getErrorPages() const {
	return _error_pages;
}
const t_everywhere 				&Parser::getEw() const {
	return _ew;
}
t_server::						s_server() { port = 0; }
t_everywhere::					s_everywhere() {
	max_body_size = 0;
	autoindex = false;
}
t_location::					s_location() {}
t_args::			s_args() {
	base_pos = 0;
	rel_pos = 0;
	ew = NULL;
	location = NULL;
	server = NULL;

	main_context.push_back("index");
	main_context.push_back("max_body_size");
	main_context.push_back("root");
	main_context.push_back("alias");
	main_context.push_back("autoindex");
	main_context.push_back("server");
	main_context.push_back("error_page");

	server_context.push_back("index");
	server_context.push_back("max_body_size");
	server_context.push_back("root");
	server_context.push_back("alias");
	server_context.push_back("autoindex");
	server_context.push_back("ip");
	server_context.push_back("port");
	server_context.push_back("server_name");
	server_context.push_back("location");

	location_context.push_back("index");
	location_context.push_back("max_body_size");
	location_context.push_back("root");
	location_context.push_back("alias");
	location_context.push_back("autoindex");
	location_context.push_back("location");
	location_context.push_back("allow");
	location_context.push_back("cgi");
}
